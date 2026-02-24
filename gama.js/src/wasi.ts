/**
 * @fileoverview A simplified WASI (WebAssembly System Interface) implementation for Gama.
 * This module provides a basic WASI environment for WebAssembly modules running in a Web Worker,
 * focusing on `stdin`, `stdout`, `stderr` and common system calls.
 * All file system and network access is deliberately stubbed to enhance security and simplify the environment.
 */

import { takeStringLen } from './wasm-utils';

// WASI constants (defined in https://github.com/WebAssembly/wasi-libc/blob/main/libc-bottom-half/headers/public/wasi/api.h)
/** No error occurred. System call completed successfully. */
const WASI_ESUCCESS = 0;
/** Bad file descriptor. */
const WASI_EBADF = 8;
/** Invalid argument. */
const WASI_EINVAL = 28;
/** Function not implemented. */
const WASI_ENOSYS = 52;

/** File type: Character device. */
const WASI_FILETYPE_CHARACTER_DEVICE = 2;
/** File descriptor flag: Append mode. */
const WASI_FDFLAG_APPEND = 1;

/**
 * Implements a simplified WASI environment for a WebAssembly instance.
 * It stubs many WASI functions to `ENOSYS` and provides basic `stdin`, `stdout`, `stderr` support.
 */
export default class GamaWASI {
  /** The WebAssembly instance associated with this WASI implementation. */
  instance: WebAssembly.Instance | null = null;
  /** Getter for the WebAssembly memory buffer. */
  mem: () => ArrayBuffer = () => { throw new Error("WASI instance not set") };
  /** Getter for a DataView of the WebAssembly memory buffer. */
  view: () => DataView = () => { throw new Error("WASI instance not set") };

  /**
   * Creates a new GamaWASI instance.
   */
  constructor() { }

  /**
   * Sets the WebAssembly instance and memory accessor functions once the WASM module is instantiated.
   * @param inst The instantiated WebAssembly instance.
   */
  setInstance(inst: WebAssembly.Instance): void {
    this.instance = inst;
    this.mem = () => (this.instance!.exports.memory as WebAssembly.Memory).buffer;
    this.view = () => new DataView(this.mem());
  }

  /**
   * Provides the WASI import object to be passed to `WebAssembly.instantiate`.
   * This object contains the implementations for all WASI functions.
   */
  get importObject(): WebAssembly.Imports {
    return {
      // --- Stubs for unused/unsupported functions ---
      /**
       * Returns the sizes of the command-line arguments.
       * Always returns 0 for argc and 0 for argument buffer size, as args are not passed.
       * @param argc_ptr Pointer to write the argument count to.
       * @param argv_buf_size_ptr Pointer to write the argument buffer size to.
       * @returns `WASI_ESUCCESS`.
       */
      args_sizes_get: (argc_ptr: number, argv_buf_size_ptr: number): number => {
        this.view().setUint32(argc_ptr, 0, true);
        this.view().setUint32(argv_buf_size_ptr, 0, true);
        return WASI_ESUCCESS;
      },
      /**
       * Writes command-line arguments. No arguments are provided.
       * @param argv Pointer to an array of argument string pointers.
       * @param argv_buf Pointer to the buffer for argument strings.
       * @returns `WASI_ESUCCESS`.
       */
      args_get: (argv: number, argv_buf: number): number => WASI_ESUCCESS,
      /**
       * Returns the sizes of the environment variables.
       * Always returns 0 for count and 0 for buffer size, as env vars are not passed.
       * @param count_ptr Pointer to write the environment variable count to.
       * @param buf_size_ptr Pointer to write the environment variable buffer size to.
       * @returns `WASI_ESUCCESS`.
       */
      environ_sizes_get: (count_ptr: number, buf_size_ptr: number): number => {
        this.view().setUint32(count_ptr, 0, true);
        this.view().setUint32(buf_size_ptr, 0, true);
        return WASI_ESUCCESS;
      },
      /**
       * Writes environment variables. No environment variables are provided.
       * @param environ Pointer to an array of environment string pointers.
       * @param environ_buf Pointer to the buffer for environment strings.
       * @returns `WASI_ESUCCESS`.
       */
      environ_get: (environ: number, environ_buf: number): number => WASI_ESUCCESS,
      /**
       * Returns the current value of a clock.
       * @param id The clock ID.
       * @param precision The desired precision.
       * @param time_ptr Pointer to write the current time to.
       * @returns `WASI_ESUCCESS`.
       */
      clock_time_get: (id: number, precision: bigint, time_ptr: number): number => {
        this.view().setBigUint64(time_ptr, BigInt(Date.now()) * 1000000n, true);
        return WASI_ESUCCESS;
      },
      /**
       * Terminates the process with a given exit code.
       * @param code The exit code.
       */
      proc_exit: (code: number): void => {
        console.warn(`WASM proc_exit called with code: ${code}. Terminating worker.`);
        (self as unknown as Worker).terminate();
      },
      /**
       * Fills a buffer with random data.
       * @param buf Pointer to the buffer in WASM memory.
       * @param len Length of the buffer.
       * @returns `WASI_ESUCCESS`.
       */
      random_get: (buf: number, len: number): number => {
        crypto.getRandomValues(new Uint8Array(this.mem(), buf, len));
        return WASI_ESUCCESS;
      },
      /**
       * Yields the current turn of execution. (Stubbed)
       * @returns `WASI_ESUCCESS`.
       */
      sched_yield: (): number => WASI_ESUCCESS,

      // --- Stdio handling ---
      /**
       * Writes data to a file descriptor.
       * Supports `stdout` (fd=1) and `stderr` (fd=2) by logging to the console.
       * @param fd The file descriptor.
       * @param iovs_ptr Pointer to an array of `wasi_iovec_t` structures.
       * @param iovs_len Number of `wasi_iovec_t` structures.
       * @param nwritten_ptr Pointer to write the number of bytes written to.
       * @returns `WASI_ESUCCESS` or `WASI_EBADF`.
       */
      fd_write: (fd: number, iovs_ptr: number, iovs_len: number, nwritten_ptr: number): number => {
        if (fd !== 1 && fd !== 2) return WASI_EBADF;

        const iovs = this.readIOVs(iovs_ptr, iovs_len);
        const text = iovs.map(iov => new TextDecoder().decode(iov.buffer)).join('');

        if (fd === 1) console.warn(`[stdout] ${text}`);
        if (fd === 2) console.error(`[stderr] ${text}`);

        const nwritten = iovs.reduce((sum, iov) => sum + iov.buffer.length, 0);
        this.view().setUint32(nwritten_ptr, nwritten, true);
        return WASI_ESUCCESS;
      },
      /**
       * Reads data from a file descriptor.
       * Supports `stdin` (fd=0) by providing dummy input.
       * @param fd The file descriptor.
       * @param iovs_ptr Pointer to an array of `wasi_iovec_t` structures.
       * @param iovs_len Number of `wasi_iovec_t` structures.
       * @param nread_ptr Pointer to write the number of bytes read to.
       * @returns `WASI_ESUCCESS` or `WASI_EBADF`.
       */
      fd_read: (fd: number, iovs_ptr: number, iovs_len: number, nread_ptr: number): number => {
        if (fd !== 0) return WASI_EBADF;

        const funnyMessages = ["hello from gama", "gama the game", "gamawin!!"];
        const input = funnyMessages[Math.floor(Math.random() * funnyMessages.length)];
        const encodedInput = new TextEncoder().encode(input + '\n');

        const iovs = this.readIOVs(iovs_ptr, iovs_len);
        let bytesWritten = 0;
        for (const iov of iovs) {
          const write_len = Math.min(iov.buffer.length, encodedInput.length - bytesWritten);
          if (write_len === 0) break;

          const dest = new Uint8Array(this.mem(), iov.offset, iov.buffer.length);
          dest.set(encodedInput.slice(bytesWritten, bytesWritten + write_len));
          bytesWritten += write_len;
        }

        this.view().setUint32(nread_ptr, bytesWritten, true);
        return WASI_ESUCCESS;
      },
      /**
       * Returns information about a file descriptor.
       * @param fd The file descriptor.
       * @param buf_ptr Pointer to write the `wasi_fdstat_t` structure to.
       * @returns `WASI_ESUCCESS` or `WASI_EBADF`.
       */
      fd_fdstat_get: (fd: number, buf_ptr: number): number => {
        if (fd > 2) return WASI_EBADF;

        this.view().setUint8(buf_ptr, WASI_FILETYPE_CHARACTER_DEVICE);
        this.view().setUint16(buf_ptr + 2, WASI_FDFLAG_APPEND, true);
        this.view().setBigUint64(buf_ptr + 8, 0n, true);
        this.view().setBigUint64(buf_ptr + 16, 0n, true);
        return WASI_ESUCCESS;
      },

      // --- All other functions are stubbed to return ENOSYS (not implemented) ---
      /** Stub for `fd_close`. @param fd The file descriptor. @returns `WASI_ENOSYS` or `WASI_ESUCCESS`. */
      fd_close: (fd: number): number => (fd > 2 ? WASI_ENOSYS : WASI_ESUCCESS),
      /** Stub for `fd_seek`. @returns `WASI_ENOSYS`. */
      fd_seek: (): number => WASI_ENOSYS,
      /** Stub for `fd_tell`. @returns `WASI_ENOSYS`. */
      fd_tell: (): number => WASI_ENOSYS,
      /** Stub for `fd_sync`. @returns `WASI_ENOSYS`. */
      fd_sync: (): number => WASI_ENOSYS,
      /** Stub for `fd_datasync`. @returns `WASI_ENOSYS`. */
      fd_datasync: (): number => WASI_ENOSYS,
      /** Stub for `fd_filestat_get`. @returns `WASI_ENOSYS`. */
      fd_filestat_get: (): number => WASI_ENOSYS,
      /** Stub for `fd_prestat_get`. @returns `WASI_EBADF`. */
      fd_prestat_get: (): number => WASI_EBADF, // No preopened dirs
      /** Stub for `fd_prestat_dir_name`. @returns `WASI_EINVAL`. */
      fd_prestat_dir_name: (): number => WASI_EINVAL,

      // Path functions are not supported
      /** Stub for `path_open`. @returns `WASI_ENOSYS`. */
      path_open: (): number => WASI_ENOSYS,
      /** Stub for `path_filestat_get`. @returns `WASI_ENOSYS`. */
      path_filestat_get: (): number => WASI_ENOSYS,
      /** Stub for `path_unlink_file`. @returns `WASI_ENOSYS`. */
      path_unlink_file: (): number => WASI_ENOSYS,

      // And the rest...
      /** Stub for `fd_pwrite`. @returns `WASI_ENOSYS`. */
      fd_pwrite: (): number => WASI_ENOSYS,
      /** Stub for `fd_pread`. @returns `WASI_ENOSYS`. */
      fd_pread: (): number => WASI_ENOSYS,
      /** Stub for `fd_renumber`. @returns `WASI_ENOSYS`. */
      fd_renumber: (): number => WASI_ENOSYS,
      /** Stub for `fd_allocate`. @returns `WASI_ENOSYS`. */
      fd_allocate: (): number => WASI_ENOSYS,
      /** Stub for `fd_advise`. @returns `WASI_ENOSYS`. */
      fd_advise: (): number => WASI_ENOSYS,
      /** Stub for `fd_readdir`. @returns `WASI_ENOSYS`. */
      fd_readdir: (): number => WASI_ENOSYS,
      /** Stub for `fd_filestat_set_size`. @returns `WASI_ENOSYS`. */
      fd_filestat_set_size: (): number => WASI_ENOSYS,
      /** Stub for `fd_filestat_set_times`. @returns `WASI_ENOSYS`. */
      fd_filestat_set_times: (): number => WASI_ENOSYS,
      /** Stub for `fd_fdstat_set_flags`. @returns `WASI_ENOSYS`. */
      fd_fdstat_set_flags: (): number => WASI_ENOSYS,
      /** Stub for `path_create_directory`. @returns `WASI_ENOSYS`. */
      path_create_directory: (): number => WASI_ENOSYS,
      /** Stub for `path_filestat_set_times`. @returns `WASI_ENOSYS`. */
      path_filestat_set_times: (): number => WASI_ENOSYS,
      /** Stub for `path_link`. @returns `WASI_ENOSYS`. */
      path_link: (): number => WASI_ENOSYS,
      /** Stub for `path_readlink`. @returns `WASI_ENOSYS`. */
      path_readlink: (): number => WASI_ENOSYS,
      /** Stub for `path_remove_directory`. @returns `WASI_ENOSYS`. */
      path_remove_directory: (): number => WASI_ENOSYS,
      /** Stub for `path_rename`. @returns `WASI_ENOSYS`. */
      path_rename: (): number => WASI_ENOSYS,
      /** Stub for `path_symlink`. @returns `WASI_ENOSYS`. */
      path_symlink: (): number => WASI_ENOSYS,
      /** Stub for `poll_oneoff`. @returns `WASI_ENOSYS`. */
      poll_oneoff: (): number => WASI_ENOSYS,
      /** Stub for `sock_recv`. @returns `WASI_ENOSYS`. */
      sock_recv: (): number => WASI_ENOSYS,
      /** Stub for `sock_send`. @returns `WASI_ENOSYS`. */
      sock_send: (): number => WASI_ENOSYS,
      /** Stub for `sock_shutdown`. @returns `WASI_ENOSYS`. */
      sock_shutdown: (): number => WASI_ENOSYS,
    };
  }

  /**
   * Reads an array of WASI I/O vectors from WebAssembly memory.
   * @param iovs_ptr Pointer to the array of `wasi_iovec_t` structures.
   * @param iovs_len Number of `wasi_iovec_t` structures.
   * @returns An array of objects, each containing a `Uint8Array` view of the buffer and its offset.
   */
  private readIOVs(iovs_ptr: number, iovs_len: number): { buffer: Uint8Array; offset: number }[] {
    const iovs = [];
    for (let i = 0; i < iovs_len; i++) {
      const ptr = this.view().getUint32(iovs_ptr + i * 8, true);
      const len = this.view().getUint32(iovs_ptr + i * 8 + 4, true);
      iovs.push({ buffer: new Uint8Array(this.mem(), ptr, len), offset: ptr });
    }
    return iovs;
  }
}