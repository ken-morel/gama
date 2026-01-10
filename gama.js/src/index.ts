declare const WORKER_CODE: string;

import { GmColor, gmcToCss } from "./color";
import { getKeyCode, GmKeyCode } from "./keyboard";
import { writeYieldResult, YieldResult } from "./sab";
import type { WorkerInitMessage, WorkerInitResponse, WorkerStartMessage, Triangle } from "./worker";

const WORKER_URL = URL.createObjectURL(new Blob([WORKER_CODE], {
  type: "application/javascript",
}));

type Pos = {
  x: number,
  y: number,
};


export default class Gama {
  #worker: Worker;
  uuid: string;
  canvas: {
    front: OffscreenCanvas;
    back: OffscreenCanvas;
  };
  ctx: {
    front: OffscreenCanvasRenderingContext2D;
    back: OffscreenCanvasRenderingContext2D;
  };
  window: {
    side: number;
    offset: Pos;
  } = {
      side: 500,
      offset: { x: 0, y: 0 },
    };
  buffer: SharedArrayBuffer;
  #buffer32: Int32Array;
  output: CanvasRenderingContext2D | null = null;

  private yielding: YieldResult;
  sizemode: "natural" | "fixed" = "natural";
  fpsTarget: number = 30;
  fps: number;
  #lastT: number;
  #delayT: number = 0;

  static FPS_ALPHA: number = 0.8;

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
      }
    });

  }
  public async start() {
    this.#worker.onmessage = (e) => this.handleWorkerMessage(e);
    this.#worker.postMessage({
      buffer: this.buffer,
    } as WorkerStartMessage);
  }
  private async handleWorkerMessage(msg: MessageEvent) {
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
        // and then wait till we finish drawing, and since two functions
        // dont run at the same time...
        await gen.next();
        break;
    }
  }
  private workerError(e: ErrorEvent) {
    console.error("Error running gama web worker: ", e);
  }

  private async* yield() {
    // do buffer options synchroniously and draw latter on animation frame
    this.ctx.back.clearRect(0, 0, this.canvas.back.width, this.canvas.back.height);
    this.ctx.back.drawImage(this.canvas.front, 0, 0);
    this.ctx.front.clearRect(0, 0, this.canvas.front.width, this.canvas.front.height);
    const now = Date.now();
    const elapsed = now - this.#lastT;

    const targetFrameTime = 1000 / this.fpsTarget;
    const delay = targetFrameTime - elapsed;
    this.#delayT += (delay - this.#delayT) / 10;

    if (this.#delayT)
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
  private drawCmd(_cmd: any[]) {
    const [cmd, ...args] = _cmd;
    const ctx = this.ctx.front;
    switch (cmd) {
      case 'line':
        var [x1, y1, x2, y2, s, c] = args as number[];
        ctx.beginPath();
        this._stroke(c);
        ctx.moveTo(...this._c_coord(x1, y1));
        ctx.lineTo(...this._c_coord(x2, y2));
        ctx.closePath();
        ctx.stroke();
        break;
      case 'rect':
        var [x, y, w, h, c] = args as number[];
        this._fill(c);
        var [x, y] = this._c_coord(x, y);
        var [w, h] = [this._c_one(w), this._c_one(h)];
        ctx.fillRect(x - w / 2, y - h / 2, w, h);
        break;
      case 'roundrect':
        var [x, y, w, h, r, c] = args as number[];
        var [w, h] = [this._c_one(w), this._c_one(h)];
        var [topX, topY] = this._c_coord(x, y);
        topX -= w / 2; topY -= h / 2; // Center it
        var r = this._c_one(r);
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
      case 'triangle':
        var [x1, y1, x2, y2, x3, y3, col] = args as number[];

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
      case 'triangles': {
        var triangles = args[0];
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
      }
        break;
      case 'circle':
        var [x, y, r, c] = args as [number, number, number, GmColor];
        ctx.beginPath();
        ctx.arc(...this._c_coord(x, y), this._c_one(r), 0, 2 * Math.PI);
        this._fill(c);
        ctx.fill();
        break;
      case 'text':
        var [x, y, s, txt, font, style, c] = args as [number, number, number, string, string, number, GmColor];
        ctx.font = this._c_one(s).toFixed(0) + "px '" + font + "'";
        this._fill(c);
        ctx.textAlign = 'center';
        ctx.textBaseline = 'middle';
        ctx.fillText(txt, ...this._c_coord(x, y));
        break;

    }
  }

  _stroke(col: GmColor) {
    this.ctx.front.strokeStyle = gmcToCss(col);
  }
  _fill(col: GmColor) {
    this.ctx.front.fillStyle = gmcToCss(col);
  }


  _c_coord(x: number, y: number): [number, number] {
    let norm_x = (x + 1.0) * 0.5
    let norm_y = (1.0 - y) * 0.5 // Invert Y-axis for screen coordinates

    return [norm_x * this.window.side + this.window.offset.x, norm_y * this.window.side +
      this.window.offset.y];
  }
  _js_offset(x: number, y: number): [number, number] {
    return [x + this.window.offset.x, y + this.window.offset.y];
  }
  _js_unoffset(x: number, y: number): [number, number] {
    return [x - this.window.offset.x, y - this.window.offset.y];
  }

  _js_coord(x: number, y: number): [number, number] {
    let norm_x = (x - this.window.offset.x) / this.window.side
    let norm_y = (y - this.window.offset.y) / this.window.side

    return [(norm_x * 2) - 1.0, 1.0 - (norm_y * 2)]
  }

  _c_one(v: number): number {
    return v * this.window.side * 0.5
  }

  private _js_one(v: number): number {
    return (v * 2) / this.window.side;
  }

  private _c_rect(x: number, y: number, w: number, h: number): [number, number, number, number] {
    let [gx, gy] = this._c_coord(x, y)

    let gw = w * 0.5 * this.window.side
    let gh = h * 0.5 * this.window.side

    return [gx - gw / 2, gy - gh / 2, gw, gh];
  }
  public setFullscreen(fs: boolean) {
    if (fs) {
      if (this.output)
        this.output.canvas.requestFullscreen();
    } else {
      if (document.fullscreenElement)
        document.exitFullscreen();
    }
  }
  public setBackground(col: GmColor) {
    if (this.output) {
      this.output.canvas.style.backgroundColor = gmcToCss(col);
      this.output.canvas.style.background = gmcToCss(col);
    } else
      console.error("Gama instance has no output");
  }

  public setTitle(msg: string) {
    try {
      document.querySelector('title')!.innerHTML = msg;
    } catch (e) { console.error(e); }
  }

  public resize(width: number, height: number) {
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
  public attach(canv: HTMLCanvasElement) {
    this.output = canv.getContext('2d');

    const cb = () => { if (this.sizemode == "natural") this.updateSize(); };

    try {
      window.addEventListener('resize', cb);
    } catch (e) { }
    canv.addEventListener('resize', cb);
    this.bindKeyboard(canv);
    this.bindMouse(canv);
  }
  public updateSize() {
    if (this.output) {
      const rect = this.output.canvas.getBoundingClientRect();
      this.resize(rect.width, rect.height);
    }
  }


  public bindKeyboard(elt: EventTarget) {
    elt.addEventListener('keydown', e => {
      this.yielding.keyboard.down.add(getKeyCode((e as KeyboardEvent).key));
    });
    elt.addEventListener('keyup', e => {
      const code = getKeyCode((e as KeyboardEvent).key);
      this.yielding.keyboard.down.delete(code)
    });
  }
  public bindMouse(elt: HTMLElement) {
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
