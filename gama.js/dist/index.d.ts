import { GmColor } from "./color";
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
    #private;
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
    };
    /** SharedArrayBuffer for efficient communication with the Web Worker. */
    buffer: SharedArrayBuffer;
    /** The main canvas's 2D rendering context that displays the final output. */
    output: CanvasRenderingContext2D | null;
    /** AudioContext for Web Audio API. */
    audioContext: AudioContext | null;
    /** Stores decoded AudioBuffers by their handle. */
    audioBuffers: Map<number, AudioBuffer>;
    /** Stores currently playing AudioBufferSourceNodes by their handle. */
    audioPlaying: Map<number, AudioBufferSourceNode>;
    /** Stores input state (keyboard, mouse) to be sent to the Web Worker. */
    private yielding;
    /** Determines how the canvas resizes ("natural" for aspect ratio, "fixed" for specific dimensions). */
    sizemode: "natural" | "fixed";
    /** The target frames per second for the game. */
    fpsTarget: number;
    /** The current calculated frames per second. */
    fps: number;
    /** Stores OffscreenCanvas instances for loaded images, mapped by their ID. */
    images: {
        [key: number]: OffscreenCanvas;
    };
    /** Alpha value for FPS smoothing calculation. */
    static FPS_ALPHA: number;
    /**
     * Private constructor to create a Gama instance. Use `Gama.create` factory method.
     * @param w The Web Worker instance.
     * @param uuid The unique ID of the instance.
     */
    private constructor();
    /**
     * Factory method to asynchronously create and initialize a Gama instance.
     * @param wasmPath The URL or path to the compiled WebAssembly game file.
     * @param uuid A unique identifier for the Gama instance.
     * @returns A Promise that resolves to a new Gama instance.
     */
    static create(wasmPath: string, uuid: string): Promise<Gama>;
    /**
     * Starts the Gama game loop. This should be called after `Gama.create`
     * and after attaching the instance to an HTMLCanvasElement.
     */
    start(): Promise<void>;
    /**
     * Handles messages received from the Web Worker.
     * Dispatches commands for resizing, drawing, input, etc.
     * @param msg The MessageEvent received from the worker.
     */
    private handleWorkerMessage;
    /**
     * Handles errors occurring in the Web Worker.
     * @param e The ErrorEvent from the worker.
     */
    private workerError;
    /**
     * Generator function that controls the main rendering loop on the main thread.
     * It handles double buffering, frame rate capping, and rendering updates.
     */
    private yield;
    /**
     * Executes a drawing command received from the Web Worker.
     * @param _cmd The drawing command array.
     */
    private drawCmd;
    /**
     * Sets the stroke style for drawing operations.
     * @param col The color to use for the stroke.
     */
    _stroke(col: GmColor): void;
    /**
     * Sets the fill style for drawing operations.
     * @param col The color to use for filling.
     */
    _fill(col: GmColor): void;
    /**
     * Converts Gama's normalized (-1 to 1) coordinates to canvas pixel coordinates.
     * @param x The normalized x-coordinate.
     * @param y The normalized y-coordinate.
     * @returns An array [x, y] in canvas pixel coordinates.
     */
    _c_coord(x: number, y: number): [number, number];
    /**
     * Applies the window offset to JavaScript pixel coordinates.
     * @param x The raw JavaScript x-coordinate.
     * @param y The raw JavaScript y-coordinate.
     * @returns An array [x, y] with offset applied.
     */
    _js_offset(x: number, y: number): [number, number];
    /**
     * Removes the window offset from JavaScript pixel coordinates.
     * @param x The raw JavaScript x-coordinate.
     * @param y The raw JavaScript y-coordinate.
     * @returns An array [x, y] with offset removed.
     */
    _js_unoffset(x: number, y: number): [number, number];
    /**
     * Converts canvas pixel coordinates to Gama's normalized (-1 to 1) coordinates.
     * @param x The canvas pixel x-coordinate.
     * @param y The canvas pixel y-coordinate.
     * @returns An array [x, y] in normalized Gama coordinates.
     */
    _js_coord(x: number, y: number): [number, number];
    /**
     * Converts a Gama dimension (e.g., radius, width) to canvas pixel units.
     * @param v The Gama dimension value.
     * @returns The dimension in canvas pixel units.
     */
    _c_one(v: number): number;
    /**
     * Converts a JavaScript dimension (e.g., mouse movement) to Gama's normalized units.
     * @param v The JavaScript dimension value.
     * @returns The dimension in normalized Gama units.
     */
    private _js_one;
    /**
     * Calculates the canvas pixel coordinates and dimensions for a rectangle, centered.
     * @param x The normalized center x-coordinate.
     * @param y The normalized center y-coordinate.
     * @param w The normalized width.
     * @param h The normalized height.
     * @returns An array [canvasX, canvasY, canvasWidth, canvasHeight].
     */
    private _c_rect;
    setFullscreen(fs: boolean): void;
    /**
     * Sets the background color of the output canvas.
     * @param col The Gama color to set.
     */
    setBackground(col: GmColor): void;
    /**
     * Sets the title of the browser window/tab.
     * @param msg The new title string.
     */
    setTitle(msg: string): void;
    /**
     * Resizes the internal and output canvases to new dimensions.
     * @param width The new width in pixels.
     * @param height The new height in pixels.
     */
    resize(width: number, height: number): void;
    /**
     * Attaches the Gama instance to a provided HTMLCanvasElement for rendering.
     * Binds keyboard and mouse input event listeners to the canvas.
     * @param canv The HTMLCanvasElement to attach to.
     */
    attach(canv: HTMLCanvasElement): void;
    /**
     * Updates the size of the canvas based on its current dimensions in the DOM.
     * Used when `sizemode` is "natural".
     */
    updateSize(): void;
    bindKeyboard(elt: EventTarget): void;
    /**
     * Binds mouse and touch event listeners to an HTMLElement (typically the canvas).
     * Updates the internal mouse state (position, clicks) for the Web Worker.
     * @param elt The HTMLElement to bind listeners to.
     */
    bindMouse(elt: HTMLElement): void;
}
export {};
