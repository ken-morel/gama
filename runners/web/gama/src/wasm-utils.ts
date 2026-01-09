export type Ptr = number;
export type CharPtr = Ptr;
export type DoublePtr = Ptr;

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

export function setDoublePtr(mem: WebAssembly.Memory, ptr: DoublePtr, val: number) {
  // Safety check for alignment
  if (ptr % 8 === 0) {
    new Float64Array(mem.buffer)[ptr / 8] = val;
  } else {
    new DataView(mem.buffer).setFloat64(ptr, val, true);
  }
}
