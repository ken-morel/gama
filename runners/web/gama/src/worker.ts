import { createDocumentRegistry } from "typescript";
import { GmColor } from "./color";
import GamaWASI from "../../wasi.js";

import { CharPtr, DoublePtr, setDoublePtr, takeString } from "./wasm-utils";
import { readYieldResult } from "./sab";


export type WorkerSuccessMessage = {
  error: string | null,
  ok: boolean,
};

export type WorkerInitMessage = {
  wasmData: ArrayBuffer,
};
export type WorkerInitResponse = WorkerSuccessMessage;
export type WorkerStartMessage = {
  buffer: SharedArrayBuffer,
};

export type WorkerMessage = {
  type: string;
  msg: any,
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
    down: string[]
  }
  last_t: number,
  image_counter: number
} = { mod: null, inst: null, buff: null, cmds: [], size: [500, 500], mem: null, running: true, mouse: { x: 0, y: 0, down: false }, last_t: 0, keyboard: { down: [] }, image_counter: 1, buff32: null };


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



const gapi = {
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
  log: function(txt: CharPtr) {
    console.log(takeString(d.mem!, txt));
  },
  quit: () => {
    d.running = false;
    console.info("gm_quit called");
  },
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
      'draw/circle',
      x, y,
      rad,
      col,
    ]);
  },
  draw_triangle: (x1: number, y1: number, x2: number, y2: number, x3: number, y3: number, col: GmColor) => {
    d.cmds.push([
      'draw/triangle',
      x1, y1,
      x2, y2,
      x3, y3,
      col
    ]);
  },
  draw_text: (x: number, y: number, size: number, txt: CharPtr, font: CharPtr, style: number, col: GmColor) => {
    d.cmds.push([
      'draw/text',
      x, y,
      size,
      takeString(d.mem!, txt),
      takeString(d.mem!, font),
      style,
      col as GmColor,
    ]);
  },
  set_background_color: (col: GmColor) => {
    self.postMessage({
      type: 'set-background-color',
      color: col,
    });
  },
  mouse_get: (x_ptr: DoublePtr, y_ptr: DoublePtr) => {
    setDoublePtr(d.mem!, x_ptr, d.mouse.x);
    setDoublePtr(d.mem!, y_ptr, d.mouse.y);
    return 0;
  },
  mouse_down: () => d.mouse.down ? 1 : 0 as number,

  resize: (width: number, height: number) => {
    self.postMessage({ type: 'resize', size: [width, height] });
  },
  fullscreen: (full: number) => {
    self.postMessage({ type: 'fullscreen', fullscreen: full != 0 });
  },
  runs: () => d.running ? 1 : 0 as number,
  key_down: (t: number, k: number) => {
    return d.keyboard.down.includes(String.fromCodePoint(t, k)) ? 1 : 0;
  },
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
    setDoublePtr(d.mem!, dt_ptr, dt);
    d.last_t = now;
    return d.running ? 1 : 0 as number;
  },
};
