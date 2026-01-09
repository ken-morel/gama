declare const WORKER_CODE: string;

import { GmColor, gmcToCss } from "./color";
import type { WorkerSuccessMessage, WorkerInitMessage, WorkerInitResponse, WorkerStartMessage } from "./worker";

const WORKER_URL = URL.createObjectURL(new Blob([WORKER_CODE], {
  type: "application/javascript",
}));

type Pos = {
  x: number,
  y: number,
};


export default class Gama {
  worker: Worker;
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
  };
  buffer: SharedArrayBuffer;
  buffer32: Int32Array;
  output: CanvasRenderingContext2D | null;
  private constructor(w: Worker) {
    this.worker = w;
    this.canvas.front = new OffscreenCanvas(500, 500);
    this.canvas.back = new OffscreenCanvas(500, 500);
    this.ctx.front = this.canvas.front.getContext('2d')!;
    this.ctx.back = this.canvas.back.getContext('2d')!;
    this.window = {
      side: 500,
      offset: { x: 0, y: 0 },
    };
    this.buffer = new SharedArrayBuffer(1024);
    this.buffer32 = new Int32Array(this.buffer);
    w.onerror = this.#workerError;
    w.onmessage = null;
    this.output = null;
  }
  attach(canv: HTMLCanvasElement) {
    this.output = canv.getContext('2d');
  }
  public static create(wasmPath: string): Promise<Gama> {
    return new Promise(async function(resolve, reject) {
      const fetchResponse = await fetch(wasmPath);
      const wasmDataBuffer = await fetchResponse.arrayBuffer();

      const worker = new Worker(WORKER_URL, { type: 'module' });


      worker.onerror = (e) => {
        reject(e);
      };

      worker.postMessage({
        wasmData: wasmDataBuffer,
      } as WorkerInitMessage, [wasmDataBuffer]);

      worker.onmessage = (msg: MessageEvent<WorkerInitResponse>) => {
        const gama = new Gama(worker);
        if (msg.data.ok)
          resolve(gama);
        else
          reject(msg.data.error);
      }
    });

  }
  public async start() {
    this.worker.onmessage = this.#handleWorkerMessage;
    this.worker.postMessage({
      buffer: this.buffer,
    } as WorkerStartMessage);
  }
  async #handleWorkerMessage(msg: MessageEvent) {
    switch (msg.data.type as string) {
      case "resize":
        this.resize(msg.data.size[0] as number, msg.data.size[1] as number);
        break;
      case 'set-title':
        this.setTitle(msg.data.title);
        break;
      case 'set-background':
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
        const promise = this.yield();
        // the worker can start right away
        Atomics.store(this.buffer32, 0, 1);
        Atomics.notify(this.buffer32, 0);
        // and then wait till we finish drawing, and since two functions
        // dont run at the same time...
        await promise;
        break;
    }
  }
  #workerError(e: ErrorEvent) {
    console.error("Error running gama web worker: ", e);
  }

  yield(): Promise<void> {
    // do buffer options synchroniously and draw latter on animation frame
    this.ctx.back.clearRect(0, 0, this.canvas.back.width, this.canvas.back.height);
    this.ctx.back.drawImage(this.canvas.front, 0, 0);
    this.ctx.front.clearRect(0, 0, this.canvas.front.width, this.canvas.front.height);
    return new Promise(function(resolve) {
      requestAnimationFrame(function() {
        this.output?.drawImage(this.canvas, 0, 0);
        resolve();
      });
    });
  }
  drawCmd(cmd: []) { }
  setFullscreen(fs: boolean) {
    if (fs) {
      if (this.output)
        this.output.canvas.requestFullscreen();
    } else {
      if (document.fullscreenElement)
        document.exitFullscreen();
    }
  }
  setBackground(col: GmColor) {
    if (this.output)
      this.output.canvas.style.backgroundColor = gmcToCss(col);

  }

  setTitle(msg: string) {
    try {
      document.querySelector('title')!.innerHTML = msg;
    } catch (e) { console.error(e); }
  }

  resize(width: number, height: number) {
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
}
