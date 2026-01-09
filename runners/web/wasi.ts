export default class GamaWASI {
  instance: WebAssembly.Instance | null = null;
  constructor() { }

  setInstance(inst: WebAssembly.Instance) {
    this.instance = inst;
  }

  get importObject() {
    const s = this;
    const mem = () => (this.instance!.exports.memory as WebAssembly.Memory).buffer;
    const view = () => new DataView(mem());

    return {
      // --- Process & Environment ---
      proc_exit: (code: number) => console.log(`Process exited: ${code}`),
      sched_yield: () => 0,
      environ_sizes_get: (conf, bufsize) => {
        view().setUint32(conf, 0, true);
        view().setUint32(bufsize, 0, true);
        return 0;
      },
      environ_get: (environ, environ_buf) => 0,
      args_sizes_get: (argc, argv_buf_size) => {
        view().setUint32(argc, 0, true);
        view().setUint32(argv_buf_size, 0, true);
        return 0;
      },
      args_get: (argv, argv_buf) => 0,

      // --- Clock ---
      clock_time_get: (id, precision, ptr) => {
        const now = BigInt(Date.now()) * 1000000n;
        view().setBigUint64(ptr, now, true);
        return 0;
      },

      // --- Random ---
      random_get: (buf, len) => {
        crypto.getRandomValues(new Uint8Array(mem(), buf, len));
        return 0;
      },

      // --- File Descriptors (The meat of the logic) ---
      fd_write: (fd, iovs, iovs_len, nwritten) => {
        let total = 0;
        for (let i = 0; i < iovs_len; i++) {
          const ptr = view().getUint32(iovs + i * 8, true);
          const len = view().getUint32(iovs + i * 8 + 4, true);
          const txt = new TextDecoder().decode(new Uint8Array(mem(), ptr, len));
          fd === 1 ? console.log(txt) : console.warn(txt);
          total += len;
        }
        view().setUint32(nwritten, total, true);
        return 0;
      },

      fd_pwrite: (fd, iovs, iovs_len, offset, nwritten) => {
        // Offset is ignored here as we are writing to console
        return this.importObject.fd_write(fd, iovs, iovs_len, nwritten);
      },

      fd_read: () => 0,
      fd_pread: () => 0,
      fd_close: () => 0,
      fd_seek: () => 28, // ENOTSUP
      fd_tell: () => 28,
      fd_sync: () => 0,
      fd_datasync: () => 0,
      fd_advise: () => 0,
      fd_allocate: () => 28,

      fd_fdstat_get: (fd, buf) => {
        const v = view();
        v.setUint8(buf, 1); // Filetype: Character Device
        v.setUint16(buf + 2, 0, true); // Flags
        v.setBigUint64(buf + 8, 0n, true); // Rights base
        v.setBigUint64(buf + 16, 0n, true); // Rights inheriting
        return 0;
      },

      fd_fdstat_set_flags: () => 0,
      fd_filestat_get: () => 28,
      fd_filestat_set_size: () => 28,
      fd_filestat_set_times: () => 28,
      fd_prestat_get: () => 8, // EBADF (No preopened dirs)
      fd_prestat_dir_name: () => 8,
      fd_readdir: () => 28,
      fd_renumber: () => 28,

      // --- Path Operations ---
      path_open: () => 44, // ENOENT
      path_create_directory: () => 28,
      path_filestat_get: () => 44,
      path_filestat_set_times: () => 28,
      path_link: () => 28,
      path_readlink: () => 44,
      path_remove_directory: () => 28,
      path_rename: () => 28,
      path_symlink: () => 28,
      path_unlink_file: () => 28,

      // --- Networking ---
      poll_oneoff: () => 28,
      sock_recv: () => 28,
      sock_send: () => 28,
      sock_shutdown: () => 28,
    };
  }
}
