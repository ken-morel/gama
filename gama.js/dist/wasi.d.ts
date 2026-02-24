/**
 * @fileoverview A simplified WASI (WebAssembly System Interface) implementation for Gama.
 * This module provides a basic WASI environment for WebAssembly modules running in a Web Worker,
 * focusing on `stdin`, `stdout`, `stderr` and common system calls.
 * All file system and network access is deliberately stubbed to enhance security and simplify the environment.
 */
/**
 * Implements a simplified WASI environment for a WebAssembly instance.
 * It stubs many WASI functions to `ENOSYS` and provides basic `stdin`, `stdout`, `stderr` support.
 */
export default class GamaWASI {
    /** The WebAssembly instance associated with this WASI implementation. */
    instance: WebAssembly.Instance | null;
    /** Getter for the WebAssembly memory buffer. */
    mem: () => ArrayBuffer;
    /** Getter for a DataView of the WebAssembly memory buffer. */
    view: () => DataView;
    /**
     * Creates a new GamaWASI instance.
     */
    constructor();
    /**
     * Sets the WebAssembly instance and memory accessor functions once the WASM module is instantiated.
     * @param inst The instantiated WebAssembly instance.
     */
    setInstance(inst: WebAssembly.Instance): void;
    /**
     * Provides the WASI import object to be passed to `WebAssembly.instantiate`.
     * This object contains the implementations for all WASI functions.
     */
    get importObject(): WebAssembly.Imports;
    /**
     * Reads an array of WASI I/O vectors from WebAssembly memory.
     * @param iovs_ptr Pointer to the array of `wasi_iovec_t` structures.
     * @param iovs_len Number of `wasi_iovec_t` structures.
     * @returns An array of objects, each containing a `Uint8Array` view of the buffer and its offset.
     */
    private readIOVs;
}
