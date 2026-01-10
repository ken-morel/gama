// A simplified WASI implementation that only handles stdio (stdin, stdout, stderr)
// and stubs out all file system and network access.

import { takeStringLen } from './wasm-utils';

// WASI constants
const WASI_ESUCCESS = 0;   // No error occurred. System call completed successfully.
const WASI_EBADF = 8;      // Bad file descriptor.
const WASI_EINVAL = 28;    // Invalid argument.
const WASI_ENOSYS = 52;    // Function not implemented.

const WASI_FILETYPE_CHARACTER_DEVICE = 2;
const WASI_FDFLAG_APPEND = 1;

export default class GamaWASI {
  instance: WebAssembly.Instance | null = null;
  mem: () => ArrayBuffer = () => { throw new Error("WASI instance not set") };
  view: () => DataView = () => { throw new Error("WASI instance not set") };

  constructor() { }

  setInstance(inst: WebAssembly.Instance) {
    this.instance = inst;
    this.mem = () => (this.instance!.exports.memory as WebAssembly.Memory).buffer;
    this.view = () => new DataView(this.mem());
  }

  get importObject() {
    return {
      // --- Stubs for unused/unsupported functions ---
      args_sizes_get: (argc_ptr: number, argv_buf_size_ptr: number) => {
        this.view().setUint32(argc_ptr, 0, true);
        this.view().setUint32(argv_buf_size_ptr, 0, true);
        return WASI_ESUCCESS;
      },
      args_get: (argv: number, argv_buf: number) => WASI_ESUCCESS,
      environ_sizes_get: (count_ptr: number, buf_size_ptr: number) => {
        this.view().setUint32(count_ptr, 0, true);
        this.view().setUint32(buf_size_ptr, 0, true);
        return WASI_ESUCCESS;
      },
      environ_get: (environ: number, environ_buf: number) => WASI_ESUCCESS,
      clock_time_get: (id: number, precision: bigint, time_ptr: number) => {
        this.view().setBigUint64(time_ptr, BigInt(Date.now()) * 1000000n, true);
        return WASI_ESUCCESS;
      },
      proc_exit: (code: number) => {
        // A proper implementation should probably terminate the worker.
        // For now, just log it.
        console.warn(`WASM proc_exit called with code: ${code}. Execution will continue.`);
      },
      random_get: (buf: number, len: number) => {
        crypto.getRandomValues(new Uint8Array(this.mem(), buf, len));
        return WASI_ESUCCESS;
      },
      sched_yield: () => WASI_ESUCCESS,

      // --- Stdio handling ---
      fd_write: (fd: number, iovs_ptr: number, iovs_len: number, nwritten_ptr: number) => {
        if (fd !== 1 && fd !== 2) return WASI_EBADF;

        const iovs = this.readIOVs(iovs_ptr, iovs_len);
        const text = iovs.map(iov => new TextDecoder().decode(iov.buffer)).join('');

        if (fd === 1) console.log(`[stdout] ${text}`);
        if (fd === 2) console.error(`[stderr] ${text}`);
        
        const nwritten = text.length; // Note: This is not byte-perfect but good enough for logging
        this.view().setUint32(nwritten_ptr, nwritten, true);
        return WASI_ESUCCESS;
      },
      fd_read: (fd: number, iovs_ptr: number, iovs_len: number, nread_ptr: number) => {
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
      fd_fdstat_get: (fd: number, buf_ptr: number) => {
        if (fd > 2) return WASI_EBADF;

        // All stdio handles are character devices
        this.view().setUint8(buf_ptr, WASI_FILETYPE_CHARACTER_DEVICE);
        this.view().setUint16(buf_ptr + 2, WASI_FDFLAG_APPEND, true);
        this.view().setBigUint64(buf_ptr + 8, 0n, true);
        this.view().setBigUint64(buf_ptr + 16, 0n, true);
        return WASI_ESUCCESS;
      },
      
      // --- All other functions are stubbed to return ENOSYS (not implemented) ---
      fd_close: (fd: number) => (fd > 2 ? WASI_ENOSYS : WASI_ESUCCESS),
      fd_seek: () => WASI_ENOSYS,
      fd_tell: () => WASI_ENOSYS,
      fd_sync: () => WASI_ENOSYS,
      fd_datasync: () => WASI_ENOSYS,
      fd_filestat_get: () => WASI_ENOSYS,
      fd_prestat_get: () => WASI_EBADF, // No preopened dirs
      fd_prestat_dir_name: () => WASI_EINVAL,

      // Path functions are not supported
      path_open: () => WASI_ENOSYS,
      path_filestat_get: () => WASI_ENOSYS,
      path_unlink_file: () => WASI_ENOSYS,
      
      // And the rest...
      fd_pwrite: () => WASI_ENOSYS,
      fd_pread: () => WASI_ENOSYS,
      fd_renumber: () => WASI_ENOSYS,
      fd_allocate: () => WASI_ENOSYS,
      fd_advise: () => WASI_ENOSYS,
      fd_readdir: () => WASI_ENOSYS,
      fd_filestat_set_size: () => WASI_ENOSYS,
      fd_filestat_set_times: () => WASI_ENOSYS,
      fd_fdstat_set_flags: () => WASI_ENOSYS,
      path_create_directory: () => WASI_ENOSYS,
      path_filestat_set_times: () => WASI_ENOSYS,
      path_link: () => WASI_ENOSYS,
      path_readlink: () => WASI_ENOSYS,
      path_remove_directory: () => WASI_ENOSYS,
      path_rename: () => WASI_ENOSYS,
      path_symlink: () => WASI_ENOSYS,
      poll_oneoff: () => WASI_ENOSYS,
      sock_recv: () => WASI_ENOSYS,
      sock_send: () => WASI_ENOSYS,
      sock_shutdown: () => WASI_ENOSYS,
    };
  }

  private readIOVs(iovs_ptr: number, iovs_len: number) {
      const iovs = [];
      for (let i = 0; i < iovs_len; i++) {
          const ptr = this.view().getUint32(iovs_ptr + i * 8, true);
          const len = this.view().getUint32(iovs_ptr + i * 8 + 4, true);
          iovs.push({ buffer: new Uint8Array(this.mem(), ptr, len), offset: ptr });
      }
      return iovs;
  }
}