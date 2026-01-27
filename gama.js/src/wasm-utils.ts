export type Ptr = number;
export type CharPtr = Ptr;
export type DoublePtr = Ptr;
export type U32Ptr = Ptr;

const utf8Decoder = new TextDecoder("utf-8");

export function takeString(mem: WebAssembly.Memory, ptr: CharPtr): string {
  if (!ptr || ptr === 0) return "";

  const buffer = mem.buffer;
  const view = new Uint8Array(buffer);

  let end = ptr;
  const maxSearch = 1024;
  while (view[end] !== 0 && (end - ptr) < maxSearch && end < view.length) {
    end++;
  }

  const bytes = view.slice(ptr, end);

  return utf8Decoder.decode(bytes);
}

export function takeStringLen(mem: WebAssembly.Memory, ptr: CharPtr, len: number): string {
  if (!ptr || ptr === 0 || len === 0) return "";
  const buffer = mem.buffer;
  const view = new Uint8Array(buffer, ptr, len);
  return utf8Decoder.decode(view);
}


export function setDoublePtr(mem: WebAssembly.Memory, ptr: DoublePtr, data: Float64Array | number[]): void {
  // For aligned pointers, create a direct view and use the highly optimized .set() method.
  if (ptr % 8 === 0) {
    const wasmView = new Float64Array(mem.buffer, ptr, data.length);
    wasmView.set(data);
    return;
  }

  // For unaligned pointers, we must copy the data byte by byte.
  const dataArray = (data instanceof Float64Array) ? data : new Float64Array(data);
  const sourceBytes = new Uint8Array(dataArray.buffer);
  const destinationBytes = new Uint8Array(mem.buffer, ptr, dataArray.byteLength);
  destinationBytes.set(sourceBytes);
}

export function setU32Ptr(mem: WebAssembly.Memory, ptr: U32Ptr, data: Uint32Array | number[]): void {
  // For aligned pointers, create a direct view and use the highly optimized .set() method.
  if (ptr % 4 === 0) {
    const wasmView = new Uint32Array(mem.buffer, ptr, data.length);
    wasmView.set(data);
    return;
  }

  // For unaligned pointers, we must copy the data byte by byte.
  const dataArray = (data instanceof Uint32Array) ? data : new Uint32Array(data);
  const sourceBytes = new Uint8Array(dataArray.buffer);
  const destinationBytes = new Uint8Array(mem.buffer, ptr, dataArray.byteLength);
  destinationBytes.set(sourceBytes);
}

// Reads an array of doubles from WASM memory.
export function getDoublePtr(mem: WebAssembly.Memory, ptr: DoublePtr, length: number): Float64Array {
  // Use a fast direct view for aligned pointers; otherwise, create an aligned copy.
  return (ptr % 8 === 0)
    ? new Float64Array(mem.buffer, ptr, length)
    : new Float64Array(new Uint8Array(mem.buffer, ptr, length * 8).slice().buffer);
}

export function getU32Ptr(mem: WebAssembly.Memory, ptr: U32Ptr, length: number): Uint32Array {
  // Use a fast direct view for aligned pointers; otherwise, create an aligned copy.
  return (ptr % 4 === 0)
    ? new Uint32Array(mem.buffer, ptr, length)
    : new Uint32Array(new Uint8Array(mem.buffer, ptr, length * 4).slice().buffer);
}

// Writes an array of gmColor (uint32_t) values to WASM memory.
export function setGmColorPtr(mem: WebAssembly.Memory, ptr: Ptr, data: Uint32Array | number[]): void {
  // For aligned pointers, create a direct view and use the highly optimized .set() method.
  if (ptr % 4 === 0) {
    const wasmView = new Uint32Array(mem.buffer, ptr, data.length);
    wasmView.set(data);
    return;
  }

  // For unaligned pointers, we must copy the data byte by byte.
  const dataArray = (data instanceof Uint32Array) ? data : new Uint32Array(data);
  const sourceBytes = new Uint8Array(dataArray.buffer);
  const destinationBytes = new Uint8Array(mem.buffer, ptr, dataArray.byteLength);
  destinationBytes.set(sourceBytes);
}

// Reads an array of gmColor (uint32_t) values from WASM memory.
export function getGmColorPtr(mem: WebAssembly.Memory, ptr: Ptr, length: number): Uint32Array {
  // Use a fast direct view for 4-byte aligned pointers; otherwise, create an aligned copy.
  return (ptr % 4 === 0)
    ? new Uint32Array(mem.buffer, ptr, length)
    : new Uint32Array(new Uint8Array(mem.buffer, ptr, length * 4).slice().buffer);
}
