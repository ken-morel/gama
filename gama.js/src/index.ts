/**
 * Declared global variable representing the compiled Web Worker code as a string.
 * This is defined during the esbuild process.
 */
declare const WORKER_CODE: string;

import { GmColor, gmcToCss } from "./color";
import { getKeyCode, GmKeyCode } from "./keyboard";
import { writeYieldResult, YieldResult } from "./sab";
import type { WorkerInitMessage, WorkerInitResponse, WorkerStartMessage, Triangle } from "./worker";

/**
 * Creates a Blob URL for the Web Worker code.
 * This URL is used to instantiate the Worker.
 */
const WORKER_URL = URL.createObjectURL(new Blob([WORKER_CODE], {
  type: "application/javascript",
}));

/**
 * Represents a 2D position with x and y coordinates.
 */
type Pos = {
  x: number;
  y: number;
};

/**
 * Configuration options for loading a Gama instance.
 */
export type GamaConfig = {
  /** The HTMLCanvasElement to render the game on. */
  canvas: HTMLCanvasElement;
  /** The path to the compiled .wasm game file. */
  src: string;
  /** Optional callback function to execute when the Gama instance has successfully loaded. */
  onLoad?: () => void;
  /** Optional callback function to execute if the Gama instance crashes. */
  onCrash?: (error: Error) => void;
};


/**
 * The main class for interacting with a Gama game instance in the browser.
 * This class manages the Web Worker, canvas rendering, and input handling.
 */
export default class Gama {
  /** The Web Worker instance running the WASM game. */
  #worker: Worker;
  /** A unique identifier for this Gama instance. */
  uuid: string;
  /** Offscreen canvases used for double buffering. */
  canvas: {
    front: OffscreenCanvas;
    back: OffscreenCanvas;
  };
  /** 2D rendering contexts for the offscreen canvases. */
  ctx: {
    front: OffscreenCanvasRenderingContext2D;
    back: OffscreenCanvasRenderingContext2D;
  };
  /** Stores information about the game window's size and offset for coordinate conversion. */
  window: {
    side: number;
    offset: Pos;
  } = {
      side: 500,
      offset: { x: 0, y: 0 },
    };
  /** SharedArrayBuffer for efficient communication with the Web Worker. */
  buffer: SharedArrayBuffer;
  /** A 32-bit integer view of the SharedArrayBuffer. */
  #buffer32: Int32Array;
  /** The main canvas's 2D rendering context that displays the final output. */
  output: CanvasRenderingContext2D | null = null;
  /** AudioContext for Web Audio API. */
  audioContext: AudioContext | null = null;
  /** Stores decoded AudioBuffers by their handle. */
  audioBuffers: Map<number, AudioBuffer> = new Map();
  /** Stores currently playing AudioBufferSourceNodes by their handle. */
  audioPlaying: Map<number, AudioBufferSourceNode> = new Map();

  /** Stores input state (keyboard, mouse) to be sent to the Web Worker. */
  private yielding: YieldResult;
  /** Determines how the canvas resizes ("natural" for aspect ratio, "fixed" for specific dimensions). */
  sizemode: "natural" | "fixed" = "natural";
  /** The target frames per second for the game. */
  fpsTarget: number = 30;
  /** The current calculated frames per second. */
  fps: number;
  /** Timestamp of the last frame. */
  #lastT: number;
  /** Accumulated delay for frame rate capping. */
  #delayT: number = 0;
  /** Stores OffscreenCanvas instances for loaded images, mapped by their ID. */
  images: { [key: number]: OffscreenCanvas } = {};
  /** Alpha value for FPS smoothing calculation. */
  static FPS_ALPHA: number = 0.8;

  /**
   * Private constructor to create a Gama instance. Use `Gama.create` factory method.
   * @param w The Web Worker instance.
   * @param uuid The unique ID of the instance.
   */
  private constructor(w: Worker, uuid: string) {
    this.uuid = uuid;
    this.yielding = {
      keyboard: {
        down: new Set<GmKeyCode>()
      },
      mouse: {
        down: false,
        x: 0,
        y: 0,
      }
    };
    this.#worker = w;
    this.canvas = {
      front: new OffscreenCanvas(500, 500),
      back: new OffscreenCanvas(500, 500)
    };
    this.ctx = {
      front: this.canvas.front.getContext('2d')!,
      back: this.canvas.back.getContext('2d')!
    };
    this.buffer = new SharedArrayBuffer(1024);
    this.#buffer32 = new Int32Array(this.buffer);
    this.#worker.onerror = this.workerError;

    this.#worker.onmessage = null;

    this.fps = this.fpsTarget;
    this.#lastT = Date.now();
  }

  /**
   * Factory method to asynchronously create and initialize a Gama instance.
   * @param wasmPath The URL or path to the compiled WebAssembly game file.
   * @param uuid A unique identifier for the Gama instance.
   * @returns A Promise that resolves to a new Gama instance.
   */
  public static async create(wasmPath: string, uuid: string): Promise<Gama> {
    const fetchResponse = await fetch(wasmPath);
    const wasmDataBuffer = await fetchResponse.arrayBuffer();
    const worker = new Worker(WORKER_URL, { type: 'module' });

    return new Promise(function(resolve, reject) {
      worker.onerror = (e) => {
        reject(e);
      };

      worker.postMessage({
        wasmData: wasmDataBuffer,
        instanceId: uuid,
      } as WorkerInitMessage, [wasmDataBuffer]);

      worker.onmessage = (msg: MessageEvent<WorkerInitResponse>) => {
        const gama = new Gama(worker, uuid);
        if (msg.data.ok)
          resolve(gama);
        else
          reject(msg.data.error);
      };
    });
  }

  /**
   * Starts the Gama game loop. This should be called after `Gama.create`
   * and after attaching the instance to an HTMLCanvasElement.
   */
  public async start(): Promise<void> {
    this.#worker.onmessage = (e) => this.handleWorkerMessage(e);

    if (!this.audioContext) {
      this.audioContext = new AudioContext();
    }
    this.#worker.postMessage({
      buffer: this.buffer,
    } as WorkerStartMessage);
  }

  /**
   * Handles messages received from the Web Worker.
   * Dispatches commands for resizing, drawing, input, etc.
   * @param msg The MessageEvent received from the worker.
   */
  private async handleWorkerMessage(msg: MessageEvent): Promise<void> {
    switch (msg.data.type as string) {
      case "resize":
        const [w, h] = msg.data.size;
        if (w == 0 && h == 0) {
          this.sizemode = "natural";
          this.updateSize();
        } else {
          this.sizemode = "fixed";
          this.resize(w, h);
        }
        break;
      case 'set-title':
        this.setTitle(msg.data.title);
        break;
      case 'set-background-color':
        this.setBackground(msg.data.color);
        break;
      case 'fullscreen':
        this.setFullscreen(msg.data.fullscreen);
        break;
      case 'draw':
        for (const cmd of msg.data.cmds) {
          this.drawCmd(cmd);
        }
        break;
      case 'yield':
        const gen = this.yield();
        await gen.next();
        // the worker can start right away
        Atomics.store(this.#buffer32, 0, 1);
        Atomics.notify(this.#buffer32, 0);

        writeYieldResult(this.buffer, 1, this.yielding);
        this.yielding.keyboard.down.clear();
        // and then wait till we finish drawing, and since two functions
        // dont run at the same time...
        await gen.next();
        break;
      case 'create-image':
        {
          const { id, data, size: [width, height] } = msg.data;
          const canv = new OffscreenCanvas(width, height);
          const imdata = new ImageData(data, width, height);

          const ctx = canv.getContext('2d')!;
          ctx.putImageData(imdata, 0, 0);

          this.images[id] = canv;
        }
        break;
      case 'create-audio':
        {
          if (!this.audioContext) {
            console.error("AudioContext not initialized.");
            break;
          }
          const { handle, pcm_data, channels, sample_rate, frame_count } = msg.data;
          const audioBuffer = this.audioContext.createBuffer(
            channels,
            frame_count,
            sample_rate
          );

          // Copy PCM data to the AudioBuffer
          if (audioBuffer.copyToChannel) { // Modern way
            for (let i = 0; i < channels; i++) {
              audioBuffer.copyToChannel(pcm_data.slice(i * frame_count, (i + 1) * frame_count), i);
            }
          } else { // Fallback for older browsers
            for (let i = 0; i < channels; i++) {
              const channelData = audioBuffer.getChannelData(i);
              for (let j = 0; j < frame_count; j++) {
                channelData[j] = pcm_data[j * channels + i]; // Interleaved data
              }
            }
          }

          this.audioBuffers.set(handle, audioBuffer);
        }
        break;
      case 'play-audio':
        {
          if (!this.audioContext) {
            console.error("AudioContext not initialized.");
            break;
          }
          const { handle, loop } = msg.data;
          const audioBuffer = this.audioBuffers.get(handle);

          if (!audioBuffer) {
            console.warn(`Attempted to play unknown audio handle: ${handle}`);
            break;
          }

          const source = this.audioContext.createBufferSource();
          source.buffer = audioBuffer;
          source.loop = loop;
          source.connect(this.audioContext.destination);
          source.start(0);

          if (loop) { // Store looping sounds to be able to stop them
            this.audioPlaying.set(handle, source);
          } else { // Automatically remove non-looping sounds when they finish
            source.onended = () => {
              source.disconnect();
              // If it was a one-shot and not explicitly stopped, it won't be in audioPlaying Map
            };
          }
        }
        break;
      case 'stop-audio':
        {
          const { handle } = msg.data;
          const source = this.audioPlaying.get(handle);
          if (source) {
            source.stop();
            source.disconnect();
            this.audioPlaying.delete(handle);
          }
        }
        break;
      case 'free-audio':
        {
          const { handle } = msg.data;
          // Stop any currently playing instances of this audio
          const playingSource = this.audioPlaying.get(handle);
          if (playingSource) {
            playingSource.stop();
            playingSource.disconnect();
            this.audioPlaying.delete(handle);
          }
          this.audioBuffers.delete(handle);
        }
        break;
    }
  }

  /**
   * Handles errors occurring in the Web Worker.
   * @param e The ErrorEvent from the worker.
   */
  private workerError(e: ErrorEvent): void {
    console.error("Error running gama web worker: ", e);
  }

  /**
   * Generator function that controls the main rendering loop on the main thread.
   * It handles double buffering, frame rate capping, and rendering updates.
   */
  private async* yield(): AsyncGenerator<void, void, void> {
    // do buffer options synchroniously and draw latter on animation frame
    this.ctx.back.clearRect(0, 0, this.canvas.back.width, this.canvas.back.height);
    this.ctx.back.drawImage(this.canvas.front, 0, 0);
    this.ctx.front.clearRect(0, 0, this.canvas.front.width, this.canvas.front.height);
    const now = Date.now();
    const elapsed = now - this.#lastT;

    const targetFrameTime = 1000 / this.fpsTarget;
    const delay = targetFrameTime - elapsed;
    this.#delayT += (delay - this.#delayT) / 10;

    if (this.#delayT > 1)
      await new Promise(resolve => setTimeout(resolve, this.#delayT));


    const frameEndTime = Date.now();
    const frameDuration = frameEndTime - this.#lastT;

    if (frameDuration > 0) {
      const cfps = 1000 / frameDuration;
      this.fps = (Gama.FPS_ALPHA * this.fps) + ((1 - Gama.FPS_ALPHA) * cfps);
    }
    this.#lastT = frameEndTime;

    yield;
    await new Promise<void>((resolve) => {
      requestAnimationFrame(() => {
        this.output?.clearRect(0, 0, this.output.canvas.width, this.output.canvas.height);
        this.output?.drawImage(this.canvas.back, 0, 0);
        resolve();
      });
    });
    yield;
  }

  /**
   * Executes a drawing command received from the Web Worker.
   * @param _cmd The drawing command array.
   */
  private drawCmd(_cmd: any[]): void {
    const [cmd, ...args] = _cmd;
    const ctx = this.ctx.front;
    switch (cmd) {
      case 'line': {
        const [x1, y1, x2, y2, s, c] = args as number[];
        ctx.beginPath();
        this._stroke(c);
        ctx.moveTo(...this._c_coord(x1, y1));
        ctx.lineTo(...this._c_coord(x2, y2));
        ctx.closePath();
        ctx.stroke();
        break;
      } case 'rect': {
        let [x, y, w, h, c] = args as number[];
        this._fill(c);
        [x, y] = this._c_coord(x, y);
        [w, h] = [this._c_one(w), this._c_one(h)];
        ctx.fillRect(x - w / 2, y - h / 2, w, h);
        break;
      } case 'roundrect': {
        let [x, y, w, h, r, c] = args as number[];
        [w, h] = [this._c_one(w), this._c_one(h)];
        let [topX, topY] = this._c_coord(x, y);
        topX -= w / 2; topY -= h / 2; // Center it
        r = this._c_one(r);
        if (w < 2 * r) r = w / 2;
        if (h < 2 * r) r = h / 2;
        this._fill(c);
        ctx.beginPath();
        ctx.moveTo(topX + r, topY);
        ctx.arcTo(topX + w, topY, topX + w, topY + h, r);
        ctx.arcTo(topX + w, topY + h, topX, topY + h, r);
        ctx.arcTo(topX, topY + h, topX, topY, r);
        ctx.arcTo(topX, topY, topX + w, topY, r);
        ctx.closePath();
        ctx.fill();
        break;
      } case 'triangle': {
        const [x1, y1, x2, y2, x3, y3, col] = args as number[];

        this._fill(col);
        this.ctx.front.lineWidth = 0;

        ctx.beginPath();
        ctx.moveTo(...this._c_coord(x1, y1));
        ctx.lineTo(...this._c_coord(x2, y2));
        ctx.lineTo(...this._c_coord(x3, y3));
        ctx.closePath();
        ctx.fill();
        ctx.stroke();
        break;
      } case 'triangles': {
        const triangles = args[0];
        for (const { a, b, c, col } of triangles as Triangle[]) {
          ctx.beginPath();
          this._fill(col);
          this.ctx.front.lineWidth = 0;
          ctx.moveTo(...this._c_coord(...a));
          ctx.lineTo(...this._c_coord(...b));
          ctx.lineTo(...this._c_coord(...c));
          ctx.closePath();
          ctx.fill();
        }
        break;
      } case 'circle': {
        const [x, y, r, c] = args as [number, number, number, GmColor];
        ctx.beginPath();
        ctx.arc(...this._c_coord(x, y), this._c_one(r), 0, 2 * Math.PI);
        this._fill(c);
        ctx.fill();
        break;
      }
      case 'text': {
        const [x, y, s, txt, font, style, c] = args as [number, number, number, string, string, number, GmColor];
        ctx.font = this._c_one(s).toFixed(0) + "px '" + font + "'";
        this._fill(c);
        ctx.textAlign = 'center';
        ctx.textBaseline = 'middle';
        ctx.fillText(txt, ...this._c_coord(x, y));
        break;
      } case 'image': {
        const [handle, x, y, w, h] = args as [number, number, number, number, number];
        ctx.drawImage(this.images[handle], ...this._c_rect(x, y, w, h));
        break;
      } case 'image-part': {
        const [handle, sx, sy, sw, sh, x, y, w, h] = args as [number, number, number, number, number, number, number, number, number, number];
        ctx.drawImage(this.images[handle], sx, sy, sw, sh, ...this._c_rect(x, y, w, h));
        break;
      }
    }
  }

  /**
   * Sets the stroke style for drawing operations.
   * @param col The color to use for the stroke.
   */
  _stroke(col: GmColor): void {
    this.ctx.front.strokeStyle = gmcToCss(col);
  }

  /**
   * Sets the fill style for drawing operations.
   * @param col The color to use for filling.
   */
  _fill(col: GmColor): void {
    this.ctx.front.fillStyle = gmcToCss(col);
  }


  /**
   * Converts Gama's normalized (-1 to 1) coordinates to canvas pixel coordinates.
   * @param x The normalized x-coordinate.
   * @param y The normalized y-coordinate.
   * @returns An array [x, y] in canvas pixel coordinates.
   */
  _c_coord(x: number, y: number): [number, number] {
    let norm_x = (x + 1.0) * 0.5;
    let norm_y = (1.0 - y) * 0.5; // Invert Y-axis for screen coordinates

    return [norm_x * this.window.side + this.window.offset.x, norm_y * this.window.side +
      this.window.offset.y];
  }

  /**
   * Applies the window offset to JavaScript pixel coordinates.
   * @param x The raw JavaScript x-coordinate.
   * @param y The raw JavaScript y-coordinate.
   * @returns An array [x, y] with offset applied.
   */
  _js_offset(x: number, y: number): [number, number] {
    return [x + this.window.offset.x, y + this.window.offset.y];
  }

  /**
   * Removes the window offset from JavaScript pixel coordinates.
   * @param x The raw JavaScript x-coordinate.
   * @param y The raw JavaScript y-coordinate.
   * @returns An array [x, y] with offset removed.
   */
  _js_unoffset(x: number, y: number): [number, number] {
    return [x - this.window.offset.x, y - this.window.offset.y];
  }

  /**
   * Converts canvas pixel coordinates to Gama's normalized (-1 to 1) coordinates.
   * @param x The canvas pixel x-coordinate.
   * @param y The canvas pixel y-coordinate.
   * @returns An array [x, y] in normalized Gama coordinates.
   */
  _js_coord(x: number, y: number): [number, number] {
    let norm_x = (x - this.window.offset.x) / this.window.side;
    let norm_y = (y - this.window.offset.y) / this.window.side;

    return [(norm_x * 2) - 1.0, 1.0 - (norm_y * 2)];
  }

  /**
   * Converts a Gama dimension (e.g., radius, width) to canvas pixel units.
   * @param v The Gama dimension value.
   * @returns The dimension in canvas pixel units.
   */
  _c_one(v: number): number {
    return v * this.window.side * 0.5;
  }

  /**
   * Converts a JavaScript dimension (e.g., mouse movement) to Gama's normalized units.
   * @param v The JavaScript dimension value.
   * @returns The dimension in normalized Gama units.
   */
  private _js_one(v: number): number {
    return (v * 2) / this.window.side;
  }

  /**
   * Calculates the canvas pixel coordinates and dimensions for a rectangle, centered.
   * @param x The normalized center x-coordinate.
   * @param y The normalized center y-coordinate.
   * @param w The normalized width.
   * @param h The normalized height.
   * @returns An array [canvasX, canvasY, canvasWidth, canvasHeight].
   */
  private _c_rect(x: number, y: number, w: number, h: number): [number, number, number, number] {
    let [gx, gy] = this._c_coord(x, y);

    let gw = w * 0.5 * this.window.side;
    let gh = h * 0.5 * this.window.side;

    return [gx - gw / 2, gy - gh / 2, gw, gh];
  }
  public setFullscreen(fs: boolean) {
    if (fs) {
      if (this.output?.canvas)
        this.output.canvas.requestFullscreen();
    } else {
      if (document.fullscreenElement)
        document.exitFullscreen();
    }
  }

  /**
   * Sets the background color of the output canvas.
   * @param col The Gama color to set.
   */
  public setBackground(col: GmColor): void {
    if (this.output) {
      this.output.canvas.style.backgroundColor = gmcToCss(col);
      this.output.canvas.style.background = gmcToCss(col);
    } else
      console.error("Gama instance has no output canvas attached.");
  }

  /**
   * Sets the title of the browser window/tab.
   * @param msg The new title string.
   */
  public setTitle(msg: string): void {
    try {
      document.querySelector('title')!.innerHTML = msg;
    } catch (e) { console.error(e); }
  }

  /**
   * Resizes the internal and output canvases to new dimensions.
   * @param width The new width in pixels.
   * @param height The new height in pixels.
   */
  public resize(width: number, height: number): void {
    this.canvas.front.width = width;
    this.canvas.front.height = height;
    this.canvas.back.width = width;
    this.canvas.back.height = height;

    if (this.output) {
      this.output.canvas.width = width;
      this.output.canvas.height = height;
    }
    this.window.side = Math.min(width, height);
    this.window.offset.x = (width - this.window.side) / 2;
    this.window.offset.y = (height - this.window.side) / 2;
  }

  /**
   * Attaches the Gama instance to a provided HTMLCanvasElement for rendering.
   * Binds keyboard and mouse input event listeners to the canvas.
   * @param canv The HTMLCanvasElement to attach to.
   */
  public attach(canv: HTMLCanvasElement): void {
    this.output = canv.getContext('2d');

    const cb = () => { if (this.sizemode == "natural") this.updateSize(); };

    try {
      window.addEventListener('resize', cb);
    } catch (e) { }
    canv.addEventListener('resize', cb);
    this.bindKeyboard(canv);
    this.bindMouse(canv);
  }

  /**
   * Updates the size of the canvas based on its current dimensions in the DOM.
   * Used when `sizemode` is "natural".
   */
  public updateSize(): void {
    if (this.output) {
      const rect = this.output.canvas.getBoundingClientRect();
      this.resize(rect.width, rect.height);
    }
  }


  public bindKeyboard(elt: EventTarget): void {
    elt.addEventListener('keydown', e => {
      this.yielding.keyboard.down.add(getKeyCode((e as KeyboardEvent).key));
    });
  }

  /**
   * Binds mouse and touch event listeners to an HTMLElement (typically the canvas).
   * Updates the internal mouse state (position, clicks) for the Web Worker.
   * @param elt The HTMLElement to bind listeners to.
   */
  public bindMouse(elt: HTMLElement): void {
    // Mouse optimization: Don't spread (...) arrays excessively in high-freq events
    elt.addEventListener('mousemove', e => {
      const r = elt.getBoundingClientRect();
      const coords = this._js_coord(e.clientX - r.x, e.clientY - r.y);
      this.yielding.mouse.x = coords[0];
      this.yielding.mouse.y = coords[1];
    });
    elt.addEventListener('mousedown', e => {
      const r = elt.getBoundingClientRect();
      const coords = this._js_coord(e.clientX - r.x, e.clientY - r.y);
      this.yielding.mouse.x = coords[0];
      this.yielding.mouse.y = coords[1];
      this.yielding.mouse.down = true;
    });
    elt.addEventListener('mouseup', e => {
      const r = elt.getBoundingClientRect();
      const coords = this._js_coord(e.clientX - r.x, e.clientY - r.y);
      this.yielding.mouse.x = coords[0];
      this.yielding.mouse.y = coords[1];
      this.yielding.mouse.down = false;
    });

    const touchpos = (e: TouchEvent) => {
      const r = elt.getBoundingClientRect();
      return this._js_coord(
        e.touches[0].clientX - r.x,
        e.touches[0].clientY - r.y
      );
    };

    elt.addEventListener('touchmove', e => {
      const coords = touchpos(e);
      this.yielding.mouse.x = coords[0];
      this.yielding.mouse.y = coords[1];
    });
    elt.addEventListener('touchstart', e => {
      const coords = touchpos(e);
      this.yielding.mouse.x = coords[0];
      this.yielding.mouse.y = coords[1];
      this.yielding.mouse.down = true;

    });
    const handle = () => {
      this.yielding.mouse.down = false;
    };
    elt.addEventListener('touchcancel', handle);
    elt.addEventListener('touchend', handle);
  }

}
