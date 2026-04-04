import { GmColor } from "./color";
import GamaWASI from "./wasi.js";

import { CharPtr, DoublePtr, U32Ptr, getDoublePtr, getGmColorPtr, Ptr, setDoublePtr, takeString, setU32Ptr } from "./wasm-utils";
import { readYieldResult } from "./sab";
import { GmKeyCode } from "./keyboard";

/**
 * Message type for a successful or failed worker initialization.
 */
export type WorkerSuccessMessage = {
  /** Error message if initialization failed. */
  error: string | null;
  /** True if initialization was successful. */
  ok: boolean;
};

/**
 * Message sent from the main thread to the worker to initialize the WASM module.
 */
export type WorkerInitMessage = {
  /** The ArrayBuffer containing the WebAssembly binary data. */
  wasmData: ArrayBuffer;
  /** A unique identifier for the Gama instance. */
  instanceId: string;
};

/**
 * Response sent from the worker to the main thread after WASM initialization.
 */
export type WorkerInitResponse = WorkerSuccessMessage;

/**
 * Message sent from the main thread to the worker to start the game loop.
 */
export type WorkerStartMessage = {
  /** The SharedArrayBuffer used for communication. */
  buffer: SharedArrayBuffer;
};

/**
 * Generic message structure for worker communication.
 */
export type WorkerMessage = {
  /** The type of the message. */
  type: string;
  /** The payload of the message. */
  msg: any;
};

/**
 * Represents a single triangle's data for drawing.
 */
export type Triangle = {
  /** First vertex coordinates [x, y]. */
  a: [number, number];
  /** Second vertex coordinates [x, y]. */
  b: [number, number];
  /** Third vertex coordinates [x, y]. */
  c: [number, number];
  /** Color of the triangle. */
  col: GmColor;
};

/**
 * Global state object for the Web Worker.
 * Stores WebAssembly module, instance, memory, communication buffers,
 * command queues, and input states.
 */
const d: {
  mod: WebAssembly.Module | null;
  inst: WebAssembly.Instance | null;
  buff: SharedArrayBuffer | null;
  buff32: Int32Array | null;
  cmds: any[];
  size: [number, number];
  mem: WebAssembly.Memory | null;
  running: boolean;
  mouse: {
    x: number;
    y: number;
    down: boolean;
  };
  keyboard: {
    down: Set<GmKeyCode>;
  };
  last_t: number;
  image_counter: number;
  audio_counter: number;
} = {
  mod: null,
  inst: null,
  buff: null,
  cmds: [],
  size: [500, 500],
  mem: null,
  running: true,
  mouse: { x: 0, y: 0, down: false },
  last_t: 0,
  keyboard: { down: new Set<GmKeyCode>() },
  image_counter: 1,
  buff32: null,
  audio_counter: 1,
};

/**
 * Current state of the worker.
 * 1: Initial (waiting for WASM data)
 * 2: Initialized (WASM loaded, waiting for SharedArrayBuffer)
 * 3: Running (game loop started)
 */
let state: number = 1;

/**
 * Calls the `gama_run` export from the WebAssembly instance.
 * This is the entry point for the C game code.
 */
function runGama(): void {
  console.info("Running gama.main");
  const ret = (d.inst!.exports.gama_run as () => number)();
  if (ret == 0) console.info("main function returned 0");
  else console.error(`main function returned ${ret}`);
}

/**
 * Main message handler for the Web Worker.
 * Processes initialization and start messages from the main thread.
 * @param msg The MessageEvent received from the main thread.
 */
self.onmessage = (msg: MessageEvent<any>): void => {
  if (state == 1) {
    state = 0; // Deactivate during initialization
    let data = msg.data as WorkerInitMessage;
    const wasi = new GamaWASI();
    const wasmImports = { wasi_snapshot_preview1: wasi.importObject, gapi: gapi };

    WebAssembly.instantiate(data.wasmData, wasmImports).then(
      function({ module: mod, instance: inst }) {
        d.mod = mod;
        d.inst = inst;
        d.mem = inst.exports.memory as WebAssembly.Memory;
        wasi.setInstance(inst);

        state = 2;
        self.postMessage({
          ok: true,
          error: null,
        } as WorkerInitResponse);
      },
    );
  } else if (state == 2) {
    state = 3;
    let data = msg.data as WorkerStartMessage;
    d.buff = data.buffer;
    d.buff32 = new Int32Array(d.buff);
    setTimeout(runGama, 0); // Start the WASM game loop
  }
};

/**
 * GAPI functions related to 2D drawing.
 * These functions enqueue drawing commands to be sent to the main thread.
 */
const draw_gapi = {
  /**
   * Draws a line.
   * @param x1 Start X coordinate.
   * @param y1 Start Y coordinate.
   * @param x2 End X coordinate.
   * @param y2 End Y coordinate.
   * @param size Line thickness.
   * @param c Color of the line.
   */
  draw_line: (x1: number, y1: number, x2: number, y2: number, size: number, c: GmColor): void => {
    d.cmds.push(['line', x1, y1, x2, y2, size, c]);
  },
  /**
   * Draws a rectangle.
   * @param x Center X coordinate.
   * @param y Center Y coordinate.
   * @param w Width.
   * @param h Height.
   * @param c Color.
   */
  draw_rect: (x: number, y: number, w: number, h: number, c: GmColor): void => {
    d.cmds.push(['rect', x, y, w, h, c]);
  },
  /**
   * Draws a rounded rectangle.
   * @param x Center X coordinate.
   * @param y Center Y coordinate.
   * @param w Width.
   * @param h Height.
   * @param rad Corner radius.
   * @param col Color.
   */
  draw_rounded_rect: (x: number, y: number, w: number, h: number, rad: number, col: GmColor): void => {
    d.cmds.push([
      'roundrect',
      x, y,
      w, h,
      rad,
      col,
    ]);
  },
  /**
   * Draws a circle.
   * @param x Center X coordinate.
   * @param y Center Y coordinate.
   * @param rad Radius.
   * @param col Color.
   */
  draw_circle: (x: number, y: number, rad: number, col: GmColor): void => {
    d.cmds.push([
      'circle',
      x, y,
      rad,
      col,
    ]);
  },
  /**
   * Draws a triangle.
   * @param x1 Vertex 1 X coordinate.
   * @param y1 Vertex 1 Y coordinate.
   * @param x2 Vertex 2 X coordinate.
   * @param y2 Vertex 2 Y coordinate.
   * @param x3 Vertex 3 X coordinate.
   * @param y3 Vertex 3 Y coordinate.
   * @param col Color.
   */
  draw_triangle: (x1: number, y1: number, x2: number, y2: number, x3: number, y3: number, col: GmColor): void => {
    d.cmds.push([
      'triangle',
      x1, y1,
      x2, y2,
      x3, y3,
      col,
    ]);
  },

  /**
   * Draws a triangle.
   * @param x1 Vertex 1 X coordinate.
   * @param y1 Vertex 1 Y coordinate.
   * @param x2 Vertex 2 X coordinate.
   * @param y2 Vertex 2 Y coordinate.
   * @param x3 Vertex 3 X coordinate.
   * @param y3 Vertex 3 Y coordinate.
   * @param col Color.
   */
  draw_text: (x: number, y: number, h: number, txt: CharPtr, font: CharPtr, style: number, color: GmColor): void => {
    d.cmds.push([
      'text',
      x, y, h,
      takeString(d.mem!, txt), takeString(d.mem!, font),
      style, color
    ]);
  },

  /** Draws multiple triangles.
   * @param ntriangles Number of triangles.
   * @param points_ptr Pointer to the array of vertex coordinates (x1,y1,x2,y2,x3,y3).
   * @param colors_ptr Pointer to the array of colors.
   */
  draw_triangles: (ntriangles: number, points_ptr: Ptr, colors_ptr: Ptr): void => {
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
  /** Enqueues a command to clear the drawing canvas. */
  clear: (): void => {
    d.cmds.push(['clear']);
  },
  /**
   * Enqueues a command to snap (screenshot) the current canvas content
   * and store it as an image with a given handle.
   * @param handle The ID to assign to the new image.
   */
  snap: (handle: number): void => {
    d.cmds.push(['snap', handle]);
  },
  /**
   * Blits a raw RGB pixel buffer to the screen.
   * @param data_ptr Pointer to the RGB data in WASM memory.
   * @param width Width of the buffer.
   * @param height Height of the buffer.
   * @param x Center X coordinate.
   * @param y Center Y coordinate.
   * @param tw Target width (0 for natural).
   * @param th Target height (0 for natural).
   */
  blit: (
    data_ptr: Ptr,
    width: number,
    height: number,
    x: number,
    y: number,
    tw: number,
    th: number,
  ): void => {
    const buffer = d.mem!.buffer;
    const size = width * height * 3;
    const bytes = new Uint8Array(buffer, data_ptr, size);
    // Convert RGB to RGBA for ImageData
    const rgba = new Uint8ClampedArray(width * height * 4);
    for (let i = 0; i < width * height; i++) {
      rgba[i * 4 + 0] = bytes[i * 3 + 0];
      rgba[i * 4 + 1] = bytes[i * 3 + 1];
      rgba[i * 4 + 2] = bytes[i * 3 + 2];
      rgba[i * 4 + 3] = 255;
    }
    d.cmds.push(['blit', rgba, width, height, x, y, tw, th]);
  },
};

/**
 * GAPI functions related to image management.
 */
const image_gapi = {
  /**
   * Creates an image from raw pixel data.
   * @param data_ptr Pointer to the raw RGBA pixel data in WASM memory.
   * @param width Width of the image.
   * @param height Height of the image.
   * @returns A handle (ID) for the newly created image, or 1 on failure.
   */
  create_image: (data_ptr: CharPtr, width: number, height: number): number => {
    if (data_ptr === 0 || width === 0 || height === 0) return 1;

    const buffer = (d.inst!.exports.memory as WebAssembly.Memory).buffer;
    const view = new Uint8Array(buffer);
    const size = width * height * 4;
    const bytes = view.slice(data_ptr, data_ptr + size);
    const id = d.image_counter++;
    const data = new Uint8ClampedArray(size);
    for (let i = 0; i < size; i++) data[i] = bytes[i];
    self.postMessage({ type: 'create-image', id, data, size: [width, height] });
    return id;
  },
  /**
   * Draws a previously created image.
   * @param handle The handle (ID) of the image to draw.
   * @param x Center X coordinate.
   * @param y Center Y coordinate.
   * @param width Width to draw.
   * @param height Height to draw.
   */
  draw_image: (handle: number, x: number, y: number, width: number, height: number): void => {
    d.cmds.push(['image', handle, x, y, width, height]);
  },
  /**
   * Draws a part of a previously created image.
   * @param handle The handle (ID) of the image to draw from.
   * @param sx Source X coordinate of the sub-rectangle.
   * @param sy Source Y coordinate of the sub-rectangle.
   * @param sw Source width of the sub-rectangle.
   * @param sh Source height of the sub-rectangle.
   * @param x Center X coordinate for drawing.
   * @param y Center Y coordinate for drawing.
   * @param w Width to draw.
   * @param h Height to draw.
   */
  draw_image_part: (
    handle: number,
    sx: number,
    sy: number,
    sw: number,
    sh: number,
    x: number,
    y: number,
    w: number,
    h: number,
  ): void => {
    d.cmds.push(['image-part', handle, sx, sy, sw, sh, x, y, w, h]);
  },
};

/**
 * GAPI functions related to audio management.
 */
const audio_gapi = {
  /**
   * Creates an audio buffer from raw PCM data.
   * @param data_ptr Pointer to the raw Float32Array PCM data in WASM memory.
   * @param frame_count_bigint Number of frames in the audio.
   * @param channels Number of audio channels.
   * @param sample_rate Sample rate of the audio.
   * @returns A handle (ID) for the newly created audio buffer.
   */
  create_audio: (data_ptr: Ptr, frame_count_bigint: bigint, channels: number, sample_rate: number): number => {
    const frame_count = Number(frame_count_bigint);
    const handle = d.audio_counter++;

    const wasm_buffer = (d.inst!.exports.memory as WebAssembly.Memory).buffer;
    const pcm_data_view = new Float32Array(wasm_buffer, data_ptr, frame_count * channels);
    const pcm_data_copy = pcm_data_view.slice(); // Copy data to avoid WASM memory issues
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
  /**
   * Plays a previously created audio buffer.
   * @param handle The handle (ID) of the audio to play.
   * @param loop 1 to loop, 0 otherwise.
   * @returns 0 on success, 1 on failure.
   */
  play_audio: (handle: number, loop: number): number => {
    self.postMessage({ type: 'play-audio', handle, loop: loop !== 0 });
    return 0;
  },
  /**
   * Stops a playing audio buffer.
   * @param handle The handle (ID) of the audio to stop.
   * @returns 0 on success, 1 on failure.
   */
  stop_audio: (handle: number): number => {
    self.postMessage({ type: 'stop-audio', handle });
    return 0;
  },
  /**
   * Frees a previously created audio buffer.
   * @param handle The handle (ID) of the audio to free.
   * @returns 0 on success, 1 on failure.
   */
  free_audio: (handle: number): number => {
    self.postMessage({ type: 'free-audio', handle });
    return 0;
  },
};

/**
 * The main GAPI (Gama API) object exposed to the WebAssembly module.
 * It combines drawing, image, and audio functionalities.
 */
const gapi = {
  ...draw_gapi,
  ...image_gapi,
  ...audio_gapi,
  /**
   * Initializes the Gama application in the worker.
   * @param width Initial window width.
   * @param height Initial window height.
   * @param title Pointer to the title string in WASM memory.
   */
  init: (width: number, height: number, title: CharPtr): void => {
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
    console.info(`gm_init called, with dimensions ${width}x${height} and title:
${txt}
`);
  },
  /**
   * Logs a message to the worker's console.
   * @param txt Pointer to the message string in WASM memory.
   */
  log: (txt: CharPtr): void => {
    console.log(takeString(d.mem!, txt));
  },
  /**
   * Logs an error message to the worker's console.
   * @param txt Pointer to the message string in WASM memory.
   */
  log_error: (txt: CharPtr): void => {
    console.error(takeString(d.mem!, txt));
  },
  /**
   * Logs a warning message to the worker's console.
   * @param txt Pointer to the message string in WASM memory.
   */
  log_warning: (txt: CharPtr): void => {
    console.warn(takeString(d.mem!, txt));
  },
  /**
   * Signals the worker to quit the Gama application.
   */
  quit: (): void => {
    d.running = false;
    console.info("gm_quit called");
    self.postMessage({
      type: 'quit',
    });
  },
  /**
   * Sets the background color of the canvas.
   * @param col The Gama color.
   */
  set_background_color: (col: GmColor): void => {
    self.postMessage({
      type: 'set-background-color',
      color: col,
    });
  },
  /**
   * Gets the current canvas size.
   * @param width_ptr Pointer to write the width to in WASM memory.
   * @param height_ptr Pointer to write the height to in WASM memory.
   */
  get_size: (width_ptr: U32Ptr, height_ptr: U32Ptr): void => {
    setU32Ptr(d.mem!, width_ptr, [d.size[0]]);
    setU32Ptr(d.mem!, height_ptr, [d.size[1]]);
  },
  /**
   * Gets the current mouse position.
   * @param x_ptr Pointer to write the x-coordinate to in WASM memory.
   * @param y_ptr Pointer to write the y-coordinate to in WASM memory.
   * @returns Always 0.
   */
  mouse_get: (x_ptr: DoublePtr, y_ptr: DoublePtr): number => {
    setDoublePtr(d.mem!, x_ptr, [d.mouse.x]);
    setDoublePtr(d.mem!, y_ptr, [d.mouse.y]);
    return 0;
  },
  /**
   * Checks if the mouse button is currently down.
   * @returns 1 if mouse is down, 0 otherwise.
   */
  mouse_down: (): number => d.mouse.down ? 1 : 0,
  /**
   * Resizes the canvas.
   * @param width New width.
   * @param height New height.
   */
  resize: (width: number, height: number): void => {
    self.postMessage({ type: 'resize', size: [width, height] });
  },
  /**
   * Toggles fullscreen mode.
   * @param full 1 to enable, 0 to disable.
   */
  fullscreen: (full: number): void => {
    self.postMessage({ type: 'fullscreen', fullscreen: full !== 0 });
  },
  /**
   * Checks if the application is still running.
   * @returns 1 if running, 0 otherwise.
   */
  runs: (): number => d.running ? 1 : 0,
  /**
   * Checks if a specific key is currently down.
   * @param t Key type component.
   * @param k Key code component.
   * @returns 1 if the key is down, 0 otherwise.
   */
  key_down: (t: number, k: number): number => {
    return d.keyboard.down.has(String.fromCodePoint(t, k)) ? 1 : 0;
  },
  /**
   * Yields control back to the main thread for rendering and updates input state.
   * @param dt_ptr Pointer to write the delta time to in WASM memory.
   * @returns 1 if the game should continue, 0 if it should quit.
   */
  yield: (dt_ptr: CharPtr): number => {
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
