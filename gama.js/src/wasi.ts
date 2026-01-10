import { VirtualFileSystem } from './vfs';
import { takeStringLen } from './wasm-utils';

// WASI constants
// https://github.com/WebAssembly/WASI/blob/main/phases/snapshot/docs.md
const WASI_ESUCCESS = 0;
const WASI_EBADF = 8;
const WASI_EINVAL = 28;
const WASI_ENOENT = 44;
const WASI_ENOSYS = 52;
const WASI_ENOTSUP = 58;

const WASI_PREOPENTYPE_DIR = 0;

export default class GamaWASI {
  instance: WebAssembly.Instance | null = null;
  vfs: VirtualFileSystem;
  mem: () => ArrayBuffer = () => { throw new Error("WASI instance not set") };
  view: () => DataView = () => { throw new Error("WASI instance not set") };

  constructor(instanceId: string) {
    this.vfs = new VirtualFileSystem(instanceId);
  }

  setInstance(inst: WebAssembly.Instance) {
    this.instance = inst;
    this.mem = () => (this.instance!.exports.memory as WebAssembly.Memory).buffer;
    this.view = () => new DataView(this.mem());
  }

  get importObject() {
    return {
      // --- Process & Environment ---
      proc_exit: (code: number) => {
        console.log(`Process exited with code: ${code}`);
        // In a real scenario, you might want to terminate the worker or notify the main thread.
      },
      sched_yield: () => WASI_ESUCCESS,
      environ_sizes_get: (count_ptr: number, buf_size_ptr: number) => {
        this.view().setUint32(count_ptr, 0, true);
        this.view().setUint32(buf_size_ptr, 0, true);
        return WASI_ESUCCESS;
      },
      environ_get: (environ_ptr: number, environ_buf_ptr: number) => WASI_ESUCCESS,
      args_sizes_get: (argc_ptr: number, argv_buf_size_ptr: number) => {
        this.view().setUint32(argc_ptr, 0, true);
        this.view().setUint32(argv_buf_size_ptr, 0, true);
        return WASI_ESUCCESS;
      },
      args_get: (argv_ptr: number, argv_buf_ptr: number) => WASI_ESUCCESS,

      // --- Clock ---
      clock_time_get: (id: number, precision: bigint, time_ptr: number) => {
        const now = BigInt(Date.now()) * 1000000n;
        this.view().setBigUint64(time_ptr, now, true);
        return WASI_ESUCCESS;
      },

      // --- Random ---
      random_get: (buf: number, len: number) => {
        crypto.getRandomValues(new Uint8Array(this.mem(), buf, len));
        return WASI_ESUCCESS;
      },

      // --- File Descriptors ---
      fd_write: (fd: number, iovs_ptr: number, iovs_len: number, nwritten_ptr: number) => {
        let nwritten = 0;
        const iovs = this.readIOVs(iovs_ptr, iovs_len);

        if (fd === 1) { // stdout
            const text = iovs.map(iov => new TextDecoder().decode(iov.buffer)).join('');
            console.log(text);
            nwritten = text.length;
        } else if (fd === 2) { // stderr
            const text = iovs.map(iov => new TextDecoder().decode(iov.buffer)).join('');
            console.error(text);
            nwritten = text.length;
        } else {
            const { nwritten: written, errno } = this.vfs.write(fd, iovs);
            if (errno !== WASI_ESUCCESS) return errno;
            nwritten = written;
        }

        this.view().setUint32(nwritten_ptr, nwritten, true);
        return WASI_ESUCCESS;
      },

      fd_read: (fd: number, iovs_ptr: number, iovs_len: number, nread_ptr: number) => {
          if (fd === 0) { // stdin
              const input = prompt("Enter input for stdin:");
              if (input === null) {
                  this.view().setUint32(nread_ptr, 0, true);
                  return WASI_ESUCCESS;
              }
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
          }

          const iovs = this.readIOVs(iovs_ptr, iovs_len);
          const { nread, errno } = this.vfs.read(fd, iovs);
          if (errno !== WASI_ESUCCESS) return errno;
          
          this.view().setUint32(nread_ptr, nread, true);
          return WASI_ESUCCESS;
      },

      fd_pwrite: () => WASI_ENOSYS,
      fd_pread: () => WASI_ENOSYS,

      fd_close: (fd: number) => this.vfs.close(fd),

      fd_seek: (fd: number, offset: bigint, whence: number, new_offset_ptr: number) => {
          const { new_offset, errno } = this.vfs.seek(fd, offset, whence);
          if (errno !== WASI_ESUCCESS) return errno;
          this.view().setBigUint64(new_offset_ptr, BigInt(new_offset), true);
          return WASI_ESUCCESS;
      },

      fd_tell: (fd: number, offset_ptr: number) => {
          const { offset, errno } = this.vfs.tell(fd);
          if (errno !== WASI_ESUCCESS) return errno;
          this.view().setBigUint64(offset_ptr, BigInt(offset), true);
          return WASI_ESUCCESS;
      },

      fd_sync: () => WASI_ESUCCESS,
      fd_datasync: () => WASI_ESUCCESS,
      fd_advise: () => WASI_ENOSYS,
      fd_allocate: () => WASI_ENOSYS,

      fd_fdstat_get: (fd: number, buf_ptr: number) => {
        const { stat, errno } = this.vfs.getFdStat(fd);
        if (errno !== WASI_ESUCCESS) return errno;
        this.view().setUint8(buf_ptr, stat.fs_filetype);
        this.view().setUint16(buf_ptr + 2, stat.fs_flags, true);
        this.view().setBigUint64(buf_ptr + 8, stat.fs_rights_base, true);
        this.view().setBigUint64(buf_ptr + 16, stat.fs_rights_inheriting, true);
        return WASI_ESUCCESS;
      },

      fd_fdstat_set_flags: () => WASI_ENOSYS,

      fd_filestat_get: (fd: number, buf_ptr: number) => {
          const { stat, errno } = this.vfs.fstat(fd);
          if (errno !== WASI_ESUCCESS) return errno;
          this.writeStat(buf_ptr, stat);
          return WASI_ESUCCESS;
      },
      fd_filestat_set_size: () => WASI_ENOSYS,
      fd_filestat_set_times: () => WASI_ENOSYS,

      fd_prestat_get: (fd: number, buf_ptr: number) => {
        // We don't have preopened directories, so we return EBADF for any fd.
        if(fd === 3) {
            this.view().setUint8(buf_ptr, WASI_PREOPENTYPE_DIR);
            this.view().setUint32(buf_ptr + 4, 1, true); // length of "."
            return WASI_ESUCCESS;
        }
        return WASI_EBADF;
      },
      fd_prestat_dir_name: (fd: number, path_ptr: number, path_len: number) => {
        if (fd === 3) {
            if (path_len > 0) {
                new Uint8Array(this.mem(), path_ptr, 1)[0] = ".".charCodeAt(0);
                return WASI_ESUCCESS;
            }
        }
        return WASI_EINVAL;
      },
      fd_readdir: () => WASI_ENOSYS,
      fd_renumber: () => WASI_ENOSYS,

      // --- Path Operations ---
      path_open: (dirfd: number, dirflags: number, path_ptr: number, path_len: number, oflags: number, fs_rights_base: bigint, fs_rights_inheriting: bigint, fdflags: number, fd_ptr: number) => {
        const path = takeStringLen(this.instance!.exports.memory as WebAssembly.Memory, path_ptr, path_len);
        const newFd = this.vfs.open(path, oflags, fs_rights_base, fdflags);
        if (newFd < 0) {
            return -newFd;
        }
        this.view().setUint32(fd_ptr, newFd, true);
        return WASI_ESUCCESS;
      },
      path_create_directory: () => WASI_ENOSYS,
      path_filestat_get: (dirfd: number, flags: number, path_ptr: number, path_len: number, buf_ptr: number) => {
          const path = takeStringLen(this.instance!.exports.memory as WebAssembly.Memory, path_ptr, path_len);
          const { stat, errno } = this.vfs.stat(path);
          if (errno !== WASI_ESUCCESS) return errno;
          this.writeStat(buf_ptr, stat);
          return WASI_ESUCCESS;
      },
      path_filestat_set_times: () => WASI_ENOSYS,
      path_link: () => WASI_ENOSYS,
      path_readlink: () => WASI_ENOSYS,
      path_remove_directory: () => WASI_ENOSYS,
      path_rename: () => WASI_ENOSYS,
      path_symlink: () => WASI_ENOSYS,
      path_unlink_file: (dirfd: number, path_ptr: number, path_len: number) => {
          const path = takeStringLen(this.instance!.exports.memory as WebAssembly.Memory, path_ptr, path_len);
          return this.vfs.unlink(path);
      },

      // --- Networking ---
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

  private writeStat(buf_ptr: number, stat: any) {
      this.view().setBigUint64(buf_ptr, BigInt(stat.dev), true);
      this.view().setBigUint64(buf_ptr + 8, BigInt(stat.ino), true);
      this.view().setUint8(buf_ptr + 16, stat.filetype);
      this.view().setUint8(buf_ptr + 24, stat.nlink);
      this.view().setBigUint64(buf_ptr + 32, BigInt(stat.size), true);
      this.view().setBigUint64(buf_ptr + 40, stat.atim, true);
      this.view().setBigUint64(buf_ptr + 48, stat.mtim, true);
      this.view().setBigUint64(buf_ptr + 56, stat.ctim, true);
  }
}