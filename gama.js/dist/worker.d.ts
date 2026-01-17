import { GmColor } from "./color";
export type WorkerSuccessMessage = {
    error: string | null;
    ok: boolean;
};
export type WorkerInitMessage = {
    wasmData: ArrayBuffer;
    instanceId: string;
};
export type WorkerInitResponse = WorkerSuccessMessage;
export type WorkerStartMessage = {
    buffer: SharedArrayBuffer;
};
export type WorkerMessage = {
    type: string;
    msg: any;
};
export type Triangle = {
    a: [number, number];
    b: [number, number];
    c: [number, number];
    col: GmColor;
};
