import { GmColor } from "./color";
import GamaWASI from "./wasi.js";

import { CharPtr, DoublePtr, U32Ptr, getDoublePtr, getGmColorPtr, Ptr, setDoublePtr, takeString, setU32Ptr } from "./wasm-utils";
import { readYieldResult } from "./sab";
import { GmKeyCode } from "./keyboard";


export type WorkerSuccessMessage = {
  error: string | null,
  ok: boolean,
};

export type WorkerInitMessage = {
  wasmData: ArrayBuffer,
  instanceId: string,
};
export type WorkerInitResponse = WorkerSuccessMessage;
export type WorkerStartMessage = {
  buffer: SharedArrayBuffer,
};

export type WorkerMessage = {
  type: string;
  msg: any,
};

export type Triangle = {
  a: [number, number],
  b: [number, number],
  c: [number, number],
  col: GmColor,
};



const d: {
  mod: WebAssembly.Module | null,
  inst: WebAssembly.Instance | null,
  buff: SharedArrayBuffer | null,
  buff32: Int32Array | null,
  cmds: any[],
  size: [number, number],
  mem: WebAssembly.Memory | null,
  running: boolean,
  mouse: {
    x: number;
    y: number;
    down: boolean;
  },
  keyboard: {
    down: Set<GmKeyCode>
  }
  last_t: number,
  image_counter: number,
  audio_counter: number,
} = { mod: null, inst: null, buff: null, cmds: [], size: [500, 500], mem: null, running: true, mouse: { x: 0, y: 0, down: false }, last_t: 0, keyboard: { down: new Set<GmKeyCode> }, image_counter: 1, buff32: null, audio_counter: 1 };


let state: number = 1;

function runGama() {
  console.info("Running gama.main");
  const ret = (d.inst!.exports.gama_run as () => number)();
  if (ret == 0)
    console.info("main function returned 0");
  else
    console.error(`main function returned ${ret}`);

}

self.onmessage = (msg: MessageEvent<any>) => {
  if (state == 1) {
    state = 0; // deactivate
    let data = msg.data as WorkerInitMessage;
    const wasi = new GamaWASI();
    const wasmImports = { wasi_snapshot_preview1: wasi.importObject, gapi: gapi };

    WebAssembly.instantiate(data.wasmData, wasmImports).then(function({ module: mod, instance: inst }) {
      d.mod = mod;
      d.inst = inst;
      d.mem = (d.inst.exports.memory as WebAssembly.Memory);
      wasi.setInstance(inst);

      state = 2;
      self.postMessage({
        ok: true,
        error: null,
      } as WorkerInitResponse);

    });
  } else if (state == 2) {
    state = 3;
    let data = msg.data as WorkerStartMessage;
    d.buff = data.buffer;
    d.buff32 = new Int32Array(d.buff);
    setTimeout(runGama, 0);
  }
}

const draw_gapi = {
  draw_line: (x1: number, y1: number, x2: number, y2: number, size: number, c: GmColor) => {
    d.cmds.push(['line', x1, y1, x2, y2, size, c],);
  },
  draw_rect: (x: number, y: number, w: number, h: number, c: GmColor) => {
    d.cmds.push(['rect', x, y, w, h, c]);
  },
  draw_rounded_rect: (x: number, y: number, w: number, h: number, rad: number, col: GmColor) => {
    d.cmds.push([
      'roundrect',
      x, y,
      w, h,
      rad,
      col
    ]);
  },

  draw_circle: (x: number, y: number, rad: number, col: GmColor) => {
    d.cmds.push([
      'circle',
      x, y,
      rad,
      col,
    ]);
  },
  draw_triangle: (x1: number, y1: number, x2: number, y2: number, x3: number, y3: number, col: GmColor) => {
    d.cmds.push([
      'triangle',
      x1, y1,
      x2, y2,
      x3, y3,
      col
    ]);
  },
  draw_text: (x: number, y: number, size: number, txt: CharPtr, font: CharPtr, style: number, col: GmColor) => {
    d.cmds.push([
      'text',
      x, y,
      size,
      takeString(d.mem!, txt),
      takeString(d.mem!, font),
      style,
      col as GmColor,
    ]);
  },
  draw_triangles: (ntriangles: number, points_ptr: Ptr, colors_ptr: Ptr) => {
    const triangles: Triangle[] = [];
    const points = getDoublePtr(d.mem!, points_ptr, ntriangles * 6);
    const colors = getGmColorPtr(d.mem!, colors_ptr, ntriangles);

    for (let i = 0; i < ntriangles; i++) {
      const p_off = i * 6;
      triangles.push({
        a: [points[p_off + 0], points[p_off + 1]],
        b: [points[p_off + 2], points[p_off + 3]],
        c: [points[p_off + 4], points[p_off + 5]],
        col: colors[i],
      });
    }
    d.cmds.push([
      'triangles',
      triangles,
    ]);
  },
  clear: () => {
    d.cmds.push(['clear']);
  },
  snap: (handle: number) => {
    d.cmds.push(['snap', handle]);
  }
};

const image_gapi = {
  create_image: (data_ptr: CharPtr, width: number, height: number) => {
    if (data_ptr * width * height == 0) return 1;

    const buffer = (d.inst!.exports.memory as WebAssembly.Memory).buffer;
    const view = new Uint8Array(buffer);
    const size = width * height * 4;
    const bytes = view.slice(data_ptr, data_ptr + size);
    const id = d.image_counter++;
    const data = new Uint8ClampedArray(size);
    for (let i = 0; i < size; i++)
      data[i] = bytes[i];
    self.postMessage({ type: 'create-image', id, data, size: [width, height] });
    return id;
  },
  draw_image: (handle: number, x: number, y: number, width: number, height: number) => {
    d.cmds.push([
      'image',
      handle,
      x, y,
      width, height,
    ]);
  },
  draw_image_part: (handle: number, sx: number, sy: number, sw: number, sh: number, x: number, y: number, w: number, h: number) => {
    d.cmds.push([
      'image-part',
      handle,
      sx, sy, sw, sh,
      x, y, w, h,
    ]);
  },
};

const audio_gapi = {
  create_audio: (data_ptr: Ptr, frame_count_bigint: bigint, channels: number, sample_rate: number): number => {
    const frame_count = Number(frame_count_bigint);
    const handle = d.audio_counter++;

    const wasm_buffer = (d.inst!.exports.memory as WebAssembly.Memory).buffer;
    const pcm_data_view = new Float32Array(wasm_buffer, data_ptr, frame_count * channels);
    const pcm_data_copy = pcm_data_view.slice();
    self.postMessage({
      type: 'create-audio',
      handle,
      pcm_data: pcm_data_copy,
      channels,
      sample_rate,
      frame_count,
    });

    return handle;
  },
  play_audio: (handle: number, loop: number): number => {
    self.postMessage({ type: 'play-audio', handle, loop: loop !== 0 });
    return 0;
  },
  stop_audio: (handle: number): number => {
    self.postMessage({ type: 'stop-audio', handle });
    return 0;
  },
  free_audio: (handle: number): number => {
    self.postMessage({ type: 'free-audio', handle });
    return 0;
  },
};

const gapi = {
  ...draw_gapi,
  ...image_gapi,
  ...audio_gapi,
  init: (width: number, height: number, title: CharPtr) => {
    self.postMessage({
      type: 'resize',
      size: [width, height],
    });
    const txt = takeString(d.mem!, title);
    self.postMessage({
      type: 'set-title',
      title: txt,
    });
    d.last_t = Date.now();
    console.info(`gm_init called, with dimensions ${width}x${height} and title: \`${txt}\``)
  },
  log: (txt: CharPtr) => {
    console.log(takeString(d.mem!, txt));
  },
  log_error: (txt: CharPtr) => {
    console.error(takeString(d.mem!, txt));
  },
  log_warning: (txt: CharPtr) => {
    console.warn(takeString(d.mem!, txt));
  },
  quit: () => {
    d.running = false;
    console.info("gm_quit called");
    self.postMessage({
      type: 'quit',
    });
  },

  set_background_color: (col: GmColor) => {
    self.postMessage({
      type: 'set-background-color',
      color: col,
    });
  },
  get_size: (width: U32Ptr, height: U32Ptr) => {
    setU32Ptr(d.mem!, width, [d.size[0]]);
    setU32Ptr(d.mem!, height, [d.size[1]]);
  },
  mouse_get: (x_ptr: DoublePtr, y_ptr: DoublePtr) => {
    setDoublePtr(d.mem!, x_ptr, [d.mouse.x]);
    setDoublePtr(d.mem!, y_ptr, [d.mouse.y]);
    return 0;
  },
  mouse_down: () => d.mouse.down ? 1 : 0,

  resize: (width: number, height: number) => {
    self.postMessage({ type: 'resize', size: [width, height] });
  },
  fullscreen: (full: number) => {
    self.postMessage({ type: 'fullscreen', fullscreen: full != 0 });
  },
  runs: () => d.running ? 1 : 0,
  key_down: (t: number, k: number) => {
    return d.keyboard.down.has(String.fromCodePoint(t, k)) ? 1 : 0;
  },

  yield: (dt_ptr: CharPtr) => {
    self.postMessage({
      type: 'draw',
      cmds: d.cmds,
    });
    d.cmds = [];

    Atomics.store(d.buff32!, 0, 0);

    self.postMessage({
      type: 'yield',
    } as WorkerMessage);

    Atomics.wait(d.buff32!, 0, 0);

    const data = readYieldResult(d.buff!, 1);
    d.mouse = data.mouse;
    d.keyboard = data.keyboard;


    const now = Date.now();
    const dt = (now - d.last_t) / 1000;
    setDoublePtr(d.mem!, dt_ptr, [dt]);
    d.last_t = now;
    return d.running ? 1 : 0;
  },

};
