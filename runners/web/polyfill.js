const _wasi_snapshot_preview1 = {
  proc_exit: (code) => {
    console.log(`Process exited with code: ${code}`);
  },
  fd_write: (fd, iov, iovcnt, pnum) => {
    // WebAssembly memory access
    const memory = p.instance.exports.memory;
    const buffer = new Uint8Array(memory.buffer);

    let totalWritten = 0;

    for (let i = 0; i < iovcnt; i++) {
      const iovPtr = iov + i * 8; // Each iovec is 8 bytes (pointer + length)
      const ptr = new Uint32Array(buffer.buffer, iovPtr, 1)[0];
      const len = new Uint32Array(buffer.buffer, iovPtr + 4, 1)[0];

      const str = new TextDecoder().decode(buffer.subarray(ptr, ptr + len));
      console.log(str);
      totalWritten += len;
    }

    // Write number of bytes written
    new Uint32Array(p.instance.exports.memory.buffer, pnum, 1)[0] = totalWritten;
    return 0; // Success
  },
  fd_read: (fd, iov, iovcnt, pnum) => {
    // Not implemented - return 0 for no data
    return 0;
  },
  fd_close: (fd) => {
    return 0; // Success
  },
  fd_fdstat_get: (fd, buf) => {
    // Simplified implementation - return basic stats for stdout/stderr
    const mem = new Uint8Array(p.instance.exports.memory.buffer);
    mem[buf] = 1; // filetype = wasi.FILETYPE_CHARACTER_DEVICE
    mem[buf + 1] = 0; // rights
    mem[buf + 2] = 0;
    mem[buf + 3] = 0;
    mem[buf + 4] = 0;
    mem[buf + 5] = 0;
    mem[buf + 6] = 0;
    mem[buf + 7] = 0;
    return 0; // Success
  },
  fd_seek: (fd, offset, whence, newOffset) => {
    return 28; // ENOTSUP: Not supported
  },
  random_get: (buf, bufLen) => {
    const memory = p.instance.exports.memory;
    const buffer = new Uint8Array(memory.buffer, buf, bufLen);
    crypto.getRandomValues(buffer);
    return 0; // Success
  },
  clock_time_get: (id, precision, timePtr) => {
    const now = BigInt(Date.now()) * 1000000n; // Convert to nanoseconds
    const memory = p.instance.exports.memory;
    const view = new DataView(memory.buffer);
    view.setBigUint64(timePtr, now, true); // littleEndian = true
    return 0; // Success
  },
  environ_get: (envc, envp) => {
    return 0;
  },
  environ_sizes_get: (envc, envs) => {
    new Uint32Array(p.instance.exports.memory.buffer, envc, 1)[0] = 0;
    new Uint32Array(p.instance.exports.memory.buffer, envs, 1)[0] = 0;
    return 0;
  },
  args_get: (argv, argvBuf) => {
    return 0;
  },
  args_sizes_get: (argc, argvBufSize) => {
    new Uint32Array(p.instance.exports.memory.buffer, argc, 1)[0] = 0;
    new Uint32Array(p.instance.exports.memory.buffer, argvBufSize, 1)[0] = 0;
    return 0;
  },
  path_open: (fd, dirflags, path, path_len, oflags, fs_rights_base,
    fs_rights_inheriting, fd_flags, opened_fd) => {
    return 1; // Error code for unsupported
  },
  path_readlink: (fd, path, path_len, buf, bufLen, result_len) => {
    return 1; // Error code for unsupported
  },
  path_remove_directory: (fd, path, path_len) => {
    return 1; // Error code for unsupported
  },
  path_rename: (fd, old_path, old_path_len, new_fd, new_path, new_path_len) => {
    return 1; // Error code for unsupported
  },
  path_symlink: (old_path, old_path_len, fd, new_path, new_path_len) => {
    return 1; // Error code for unsupported
  },
  path_unlink_file: (fd, path, path_len) => {
    return 1; // Error code for unsupported
  },
  fd_sync: (fd) => {
    return 0; // Success
  },
  fd_datasync: (fd) => {
    return 0; // Success
  },
  fd_allocate: (fd, offset, len) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_filestat_get: (fd, buf) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_filestat_set_size: (fd, size) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_filestat_set_times: (fd, atim, mtim, fst_flags) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_prestat_get: (fd, buf) => {
    return 1; // Error code for unsupported
  },
  fd_prestat_dir_name: (fd, path, path_len) => {
    return 1; // Error code for unsupported
  },
  poll_oneoff: (in_ptr, out_ptr, nsubscriptions, nevents_ptr) => {
    return 28; // ENOTSUP: Not supported
  },
  sched_yield: () => {
    return 0; // Success
  },
  sock_recv: (fd, ri_data, ri_flags, ro_databuf, ro_flags) => {
    return 28; // ENOTSUP: Not supported
  },
  sock_send: (fd, si_data, si_flags, nwritten) => {
    return 28; // ENOTSUP: Not supported
  },
  sock_shutdown: (fd, how) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_pwrite: (fd, iov, iovcnt, offset, pnum) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_prestat_get: (fd, buf) => {
    return 1; // Error code for unsupported
  },
  fd_prestat_dir_name: (fd, path, path_len) => {
    return 1; // Error code for unsupported
  },
  fd_fdstat_set_flags: (fd, flags) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_filestat_set_times: (fd, atim, mtim, fst_flags) => {
    return 28; // ENOTSUP: Not supported
  },
  path_create_directory: (fd, path, path_len) => {
    return 28; // ENOTSUP: Not supported
  },
  path_filestat_get: (fd, flags, path, path_len, buf) => {
    return 28; // ENOTSUP: Not supported
  },
  path_filestat_set_times: (fd, flags, path, path_len, atim, mtim, fst_flags) => {
    return 28; // ENOTSUP: Not supported
  },
  path_link: (old_fd, old_flags, old_path, old_path_len, new_fd, new_path, new_path_len) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_advise: (fd, offset, len, advice) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_allocate: (fd, offset, len) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_close: (fd) => {
    return 0; // Success
  },
  fd_datasync: (fd) => {
    return 0; // Success
  },
  fd_fdstat_get: (fd, buf) => {
    // Simplified implementation - return basic stats for stdout/stderr
    const mem = new Uint8Array(p.instance.exports.memory.buffer);
    mem[buf] = 1; // filetype = wasi.FILETYPE_CHARACTER_DEVICE
    mem[buf + 1] = 0; // rights
    mem[buf + 2] = 0;
    mem[buf + 3] = 0;
    mem[buf + 4] = 0;
    mem[buf + 5] = 0;
    mem[buf + 6] = 0;
    mem[buf + 7] = 0;
    return 0; // Success
  },
  fd_fdstat_set_flags: (fd, flags) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_filestat_get: (fd, buf) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_filestat_set_size: (fd, size) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_filestat_set_times: (fd, atim, mtim, fst_flags) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_pread: (fd, iov, iovcnt, offset, nread) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_prestat_get: (fd, buf) => {
    return 1; // Error code for unsupported
  },
  fd_prestat_dir_name: (fd, path, path_len) => {
    return 1; // Error code for unsupported
  },
  fd_readdir: (fd, buf, buf_len, cookie, buf_used) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_renumber: (fd, to) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_seek: (fd, offset, whence, newOffset) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_sync: (fd) => {
    return 0; // Success
  },
  fd_tell: (fd, offset) => {
    return 28; // ENOTSUP: Not supported
  },
  path_exists: (fd, path, path_len) => {
    return 28; // ENOTSUP: Not supported
  },
  path_filestat_get: (fd, flags, path, path_len, buf) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_writev: (fd, iov, iovcnt, pnum) => {
    return 28; // ENOTSUP: Not supported
  },
  fd_readv: (fd, iov, iovcnt, pnum) => {
    return 28; // ENOTSUP: Not supported
  },
  path_symlink: (old_path, old_path_len, fd, new_path, new_path_len) => {
    return 28; // ENOTSUP: Not supported
  },
  sock_recv: (fd, ri_data, ri_flags, ro_databuf, ro_flags) => {
    return 28; // ENOTSUP: Not supported
  },
  sock_send: (fd, si_data, si_flags, nwritten) => {
    return 28; // ENOTSUP: Not supported
  },
  random_get: (buf, bufLen) => {
    const memory = p.instance.exports.memory;
    const buffer = new Uint8Array(memory.buffer, buf, bufLen);
    crypto.getRandomValues(buffer);
    return 0; // Success
  }
};

// Additional fallback for any WASI functions that may not be explicitly defined
// This dynamically adds functions that are requested but not defined
const proxyHandler = {
  get: function(target, prop) {
    if (prop in target) {
      return target[prop];
    }
    // Return a generic function that returns "not supported" error
    return function() {
      console.warn(`Called unimplemented WASI function: ${prop}`);
      return 28; // ENOTSUP: Not supported
    };
  }
};


export const wasi_snapshot_preview1 = new Proxy(_wasi_snapshot_preview1, proxyHandler);

