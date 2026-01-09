var __typeError = (msg) => {
  throw TypeError(msg);
};
var __accessCheck = (obj, member, msg) => member.has(obj) || __typeError("Cannot " + msg);
var __privateAdd = (obj, member, value) => member.has(obj) ? __typeError("Cannot add the same private member more than once") : member instanceof WeakSet ? member.add(obj) : member.set(obj, value);
var __privateMethod = (obj, member, method) => (__accessCheck(obj, member, "access private method"), method);

// src/color.ts
function gmcDecode(col) {
  return {
    r: (col & 4278190080) >> 24,
    g: (col & 16711680) >> 16,
    b: (col & 65280) >> 8,
    a: (col & 255) >> 0
  };
}
function gmcToCss(c) {
  const { r, g, b, a } = gmcDecode(c);
  return `rgba(${r}, ${g}, ${b}, ${a})`;
}

// src/index.ts
var WORKER_URL = URL.createObjectURL(new Blob(['"use strict";\n(() => {\n  // src/wasi.js\n  var GamaWASI = class {\n    constructor() {\n      this.instance = null;\n    }\n    setInstance(inst) {\n      this.instance = inst;\n    }\n    get importObject() {\n      const s = this;\n      const mem = () => this.instance.exports.memory.buffer;\n      const view = () => new DataView(mem());\n      return {\n        // --- Process & Environment ---\n        proc_exit: (code) => console.log(`Process exited: ${code}`),\n        sched_yield: () => 0,\n        environ_sizes_get: (conf, bufsize) => {\n          view().setUint32(conf, 0, true);\n          view().setUint32(bufsize, 0, true);\n          return 0;\n        },\n        environ_get: (environ, environ_buf) => 0,\n        args_sizes_get: (argc, argv_buf_size) => {\n          view().setUint32(argc, 0, true);\n          view().setUint32(argv_buf_size, 0, true);\n          return 0;\n        },\n        args_get: (argv, argv_buf) => 0,\n        // --- Clock ---\n        clock_time_get: (id, precision, ptr) => {\n          const now = BigInt(Date.now()) * 1000000n;\n          view().setBigUint64(ptr, now, true);\n          return 0;\n        },\n        // --- Random ---\n        random_get: (buf, len) => {\n          crypto.getRandomValues(new Uint8Array(mem(), buf, len));\n          return 0;\n        },\n        // --- File Descriptors (The meat of the logic) ---\n        fd_write: (fd, iovs, iovs_len, nwritten) => {\n          let total = 0;\n          for (let i = 0; i < iovs_len; i++) {\n            const ptr = view().getUint32(iovs + i * 8, true);\n            const len = view().getUint32(iovs + i * 8 + 4, true);\n            const txt = new TextDecoder().decode(new Uint8Array(mem(), ptr, len));\n            fd === 1 ? console.log(txt) : console.warn(txt);\n            total += len;\n          }\n          view().setUint32(nwritten, total, true);\n          return 0;\n        },\n        fd_pwrite: (fd, iovs, iovs_len, offset, nwritten) => {\n          return this.importObject.fd_write(fd, iovs, iovs_len, nwritten);\n        },\n        fd_read: () => 0,\n        fd_pread: () => 0,\n        fd_close: () => 0,\n        fd_seek: () => 28,\n        // ENOTSUP\n        fd_tell: () => 28,\n        fd_sync: () => 0,\n        fd_datasync: () => 0,\n        fd_advise: () => 0,\n        fd_allocate: () => 28,\n        fd_fdstat_get: (fd, buf) => {\n          const v = view();\n          v.setUint8(buf, 1);\n          v.setUint16(buf + 2, 0, true);\n          v.setBigUint64(buf + 8, 0n, true);\n          v.setBigUint64(buf + 16, 0n, true);\n          return 0;\n        },\n        fd_fdstat_set_flags: () => 0,\n        fd_filestat_get: () => 28,\n        fd_filestat_set_size: () => 28,\n        fd_filestat_set_times: () => 28,\n        fd_prestat_get: () => 8,\n        // EBADF (No preopened dirs)\n        fd_prestat_dir_name: () => 8,\n        fd_readdir: () => 28,\n        fd_renumber: () => 28,\n        // --- Path Operations ---\n        path_open: () => 44,\n        // ENOENT\n        path_create_directory: () => 28,\n        path_filestat_get: () => 44,\n        path_filestat_set_times: () => 28,\n        path_link: () => 28,\n        path_readlink: () => 44,\n        path_remove_directory: () => 28,\n        path_rename: () => 28,\n        path_symlink: () => 28,\n        path_unlink_file: () => 28,\n        // --- Networking ---\n        poll_oneoff: () => 28,\n        sock_recv: () => 28,\n        sock_send: () => 28,\n        sock_shutdown: () => 28\n      };\n    }\n  };\n\n  // src/wasm-utils.ts\n  var utf8Decoder = new TextDecoder("utf-8");\n\n  // src/worker.ts\n  var d = { mod: null, inst: null, buff: null, cmds: [], size: [500, 500], mem: null, running: true, mouse: { x: 0, y: 0, down: false }, last_t: 0, keyboard: { down: [] }, image_counter: 1, buff32: null };\n  var state = 1;\n  function runGama() {\n    console.info("Running gama.main");\n    const ret = d.inst.exports.gama_run();\n    if (ret == 0)\n      console.info("main function returned 0");\n    else\n      console.error(`main function returned ${ret}`);\n  }\n  self.onmessage = (msg) => {\n    if (state == 1) {\n      state = 0;\n      let data = msg.data;\n      const wasi = new GamaWASI();\n      const wasmImports = { wasi_snapshot_preview1: wasi.importObject };\n      WebAssembly.instantiate(data.wasmData, wasmImports).then(function({ module: mod, instance: inst }) {\n        d.mod = mod;\n        d.inst = inst;\n        d.mem = d.inst.exports.memory;\n        state = 2;\n        self.postMessage({\n          ok: true,\n          error: null\n        });\n      });\n    } else if (state == 2) {\n      state = 3;\n      let data = msg.data;\n      d.buff = data.buffer;\n      d.buff32 = new Int32Array(d.buff);\n      setTimeout(runGama, 0);\n    }\n  };\n})();\n'], {
  type: "application/javascript"
}));
var _Gama_instances, handleWorkerMessage_fn, workerError_fn;
var _Gama = class _Gama {
  constructor(w) {
    __privateAdd(this, _Gama_instances);
    this.worker = w;
    this.canvas = {
      front: new OffscreenCanvas(500, 500),
      back: new OffscreenCanvas(500, 500)
    };
    this.ctx = {
      front: this.canvas.front.getContext("2d"),
      back: this.canvas.back.getContext("2d")
    };
    this.window = {
      side: 500,
      offset: { x: 0, y: 0 }
    };
    this.buffer = new SharedArrayBuffer(1024);
    this.buffer32 = new Int32Array(this.buffer);
    console.log(this.worker);
    this.worker.onerror = __privateMethod(this, _Gama_instances, workerError_fn);
    this.worker.onmessage = null;
    this.output = null;
  }
  attach(canv) {
    this.output = canv.getContext("2d");
  }
  static create(wasmPath) {
    return new Promise(async function(resolve, reject) {
      const fetchResponse = await fetch(wasmPath);
      const wasmDataBuffer = await fetchResponse.arrayBuffer();
      const worker = new Worker(WORKER_URL, { type: "module" });
      console.log(worker);
      worker.onerror = (e) => {
        reject(e);
      };
      worker.postMessage({
        wasmData: wasmDataBuffer
      }, [wasmDataBuffer]);
      worker.onmessage = (msg) => {
        const gama = new _Gama(worker);
        if (msg.data.ok)
          resolve(gama);
        else
          reject(msg.data.error);
      };
    });
  }
  async start() {
    this.worker.onmessage = __privateMethod(this, _Gama_instances, handleWorkerMessage_fn);
    this.worker.postMessage({
      buffer: this.buffer
    });
  }
  yield() {
    this.ctx.back.clearRect(0, 0, this.canvas.back.width, this.canvas.back.height);
    this.ctx.back.drawImage(this.canvas.front, 0, 0);
    this.ctx.front.clearRect(0, 0, this.canvas.front.width, this.canvas.front.height);
    return new Promise((resolve) => {
      requestAnimationFrame(() => {
        this.output?.drawImage(this.canvas.back, 0, 0);
        resolve();
      });
    });
  }
  drawCmd(cmd) {
  }
  setFullscreen(fs) {
    if (fs) {
      if (this.output)
        this.output.canvas.requestFullscreen();
    } else {
      if (document.fullscreenElement)
        document.exitFullscreen();
    }
  }
  setBackground(col) {
    if (this.output)
      this.output.canvas.style.backgroundColor = gmcToCss(col);
  }
  setTitle(msg) {
    try {
      document.querySelector("title").innerHTML = msg;
    } catch (e) {
      console.error(e);
    }
  }
  resize(width, height) {
    this.canvas.front.width = width;
    this.canvas.front.height = height;
    this.canvas.back.width = width;
    this.canvas.back.height = height;
    if (this.output) {
      this.output.canvas.width = width;
      this.output.canvas.height = height;
    }
    this.window.side = Math.min(width, height);
    this.window.offset.x = (width - this.window.side) / 2;
    this.window.offset.y = (height - this.window.side) / 2;
  }
};
_Gama_instances = new WeakSet();
handleWorkerMessage_fn = async function(msg) {
  switch (msg.data.type) {
    case "resize":
      this.resize(msg.data.size[0], msg.data.size[1]);
      break;
    case "set-title":
      this.setTitle(msg.data.title);
      break;
    case "set-background":
      this.setBackground(msg.data.color);
      break;
    case "fullscreen":
      this.setFullscreen(msg.data.fullscreen);
      break;
    case "draw":
      for (const cmd of msg.data.cmds) {
        this.drawCmd(cmd);
      }
      break;
    case "yield":
      const promise = this.yield();
      Atomics.store(this.buffer32, 0, 1);
      Atomics.notify(this.buffer32, 0);
      await promise;
      break;
  }
};
workerError_fn = function(e) {
  console.error("Error running gama web worker: ", e);
};
var Gama = _Gama;
export {
  Gama as default
};
