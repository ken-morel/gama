import { GmColor } from "./color";
type Pos = {
    x: number;
    y: number;
};
export default class Gama {
    #private;
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
    };
    buffer: SharedArrayBuffer;
    output: CanvasRenderingContext2D | null;
    private yielding;
    sizemode: "natural" | "fixed";
    fpsTarget: number;
    fps: number;
    images: {
        [key: number]: OffscreenCanvas;
    };
    static FPS_ALPHA: number;
    private constructor();
    static create(wasmPath: string, uuid: string): Promise<Gama>;
    start(): Promise<void>;
    private handleWorkerMessage;
    private workerError;
    private yield;
    private drawCmd;
    _stroke(col: GmColor): void;
    _fill(col: GmColor): void;
    _c_coord(x: number, y: number): [number, number];
    _js_offset(x: number, y: number): [number, number];
    _js_unoffset(x: number, y: number): [number, number];
    _js_coord(x: number, y: number): [number, number];
    _c_one(v: number): number;
    private _js_one;
    private _c_rect;
    setFullscreen(fs: boolean): void;
    setBackground(col: GmColor): void;
    setTitle(msg: string): void;
    resize(width: number, height: number): void;
    attach(canv: HTMLCanvasElement): void;
    updateSize(): void;
    bindKeyboard(elt: EventTarget): void;
    bindMouse(elt: HTMLElement): void;
}
export {};
