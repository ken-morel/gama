export type Ptr = number;
export type CharPtr = Ptr;
export type DoublePtr = Ptr;
export declare function takeString(mem: WebAssembly.Memory, ptr: CharPtr): string;
export declare function takeStringLen(mem: WebAssembly.Memory, ptr: CharPtr, len: number): string;
export declare function setDoublePtr(mem: WebAssembly.Memory, ptr: DoublePtr, data: Float64Array | number[]): void;
export declare function getDoublePtr(mem: WebAssembly.Memory, ptr: DoublePtr, length: number): Float64Array;
export declare function setGmColorPtr(mem: WebAssembly.Memory, ptr: Ptr, data: Uint32Array | number[]): void;
export declare function getGmColorPtr(mem: WebAssembly.Memory, ptr: Ptr, length: number): Uint32Array;
