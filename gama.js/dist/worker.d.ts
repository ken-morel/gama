import { GmColor } from "./color";
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
