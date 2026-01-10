// WASI constants used by the VFS
// See: https://github.com/WebAssembly/WASI/blob/main/phases/snapshot/docs.md
const WASI_ESUCCESS = 0;
const WASI_EBADF = 8;
const WASI_EINVAL = 28;
const WASI_ENOENT = 44;
const WASI_EACCES = 2;

const WASI_FILETYPE_REGULAR_FILE = 4;
const WASI_FILETYPE_DIRECTORY = 3;
const WASI_FILETYPE_CHARACTER_DEVICE = 2;

const WASI_O_CREAT = 1;
const WASI_O_TRUNC = 4;

const WASI_RIGHTS_FD_READ = 2n;
const WASI_RIGHTS_FD_WRITE = 64n;
const WASI_RIGHTS_FD_FILESTAT_GET = 32n;
const WASI_RIGHTS_FD_SEEK = 16n;
const WASI_RIGHTS_FD_TELL = 8n;


interface FileDescriptor {
    path: string;
    rights: {
        base: bigint;
        inheriting: bigint;
    };
    position: number;
    flags: number;
    isPreopen: boolean;
}

/**
 * A simplified virtual file system that persists to localStorage.
 * Each instance is sandboxed based on the instanceId.
 */
export class VirtualFileSystem {
    private files: Map<string, Uint8Array>;
    private fds: Map<number, FileDescriptor>;
    private nextFd: number;
    private storageKey: string;

    constructor(instanceId: string) {
        this.storageKey = `gama-vfs-${instanceId}`;
        this.files = new Map();
        this.fds = new Map();
        // 0, 1, 2 are reserved for stdio
        this.nextFd = 3;
        this.loadFromStorage();
    }

    private loadFromStorage() {
        const storedFiles = localStorage.getItem(this.storageKey);
        if (storedFiles) {
            try {
                const parsed = JSON.parse(storedFiles);
                for (const [path, data] of Object.entries(parsed)) {
                    // JSON stores Uint8Array as an object with numeric keys, so we convert it back
                    const buffer = Uint8Array.from(Object.values(data as any));
                    this.files.set(path, buffer);
                }
                console.log(`VFS for ${this.storageKey} loaded ${this.files.size} files from localStorage.`);
            } catch (e) {
                console.error("Failed to load VFS from localStorage", e);
            }
        }
    }

    private saveToStorage() {
        try {
            const fileObject: { [key: string]: any } = {};
            for (const [path, data] of this.files.entries()) {
                // Convert Uint8Array to a plain array for JSON serialization
                fileObject[path] = Array.from(data);
            }
            localStorage.setItem(this.storageKey, JSON.stringify(fileObject));
        } catch (e) {
            console.error("Failed to save VFS to localStorage", e);
        }
    }

    open(path: string, oflags: number, rights_base: bigint, fdflags: number): number {
        let fileContent = this.files.get(path);

        if (fileContent === undefined) {
            if ((oflags & WASI_O_CREAT) === WASI_O_CREAT) {
                fileContent = new Uint8Array(0);
                this.files.set(path, fileContent);
                this.saveToStorage();
            } else {
                // The user requested a network fetch here. However, WASI file operations
                // are synchronous, and network requests (fetch) are asynchronous.
                // A synchronous XMLHttpRequest is deprecated and not ideal.
                // For now, we return ENOENT, as a robust async-backed file system
                // requires a much more complex implementation (e.g., using Asyncify).
                return -WASI_ENOENT;
            }
        }

        if ((oflags & WASI_O_TRUNC) === WASI_O_TRUNC) {
            if (!((rights_base & WASI_RIGHTS_FD_WRITE) === WASI_RIGHTS_FD_WRITE)) {
                return -WASI_EACCES;
            }
            fileContent = new Uint8Array(0);
            this.files.set(path, fileContent);
        }

        const fd = this.nextFd++;
        this.fds.set(fd, {
            path,
            position: 0,
            rights: { base: rights_base, inheriting: rights_base },
            flags: fdflags,
            isPreopen: false
        });

        return fd;
    }

    close(fd: number): number {
        if (!this.fds.has(fd)) return -WASI_EBADF;
        this.fds.delete(fd);
        return WASI_ESUCCESS;
    }

    write(fd: number, iovs: { buffer: Uint8Array, offset: number }[]): { nwritten: number, errno: number } {
        const descriptor = this.fds.get(fd);
        if (!descriptor) return { nwritten: 0, errno: WASI_EBADF };

        if (!(descriptor.rights.base & WASI_RIGHTS_FD_WRITE)) return { nwritten: 0, errno: WASI_EACCES };

        let fileContent = this.files.get(descriptor.path) || new Uint8Array(0);
        let nwritten = 0;

        for (const iov of iovs) {
            const newSize = descriptor.position + iov.buffer.length;
            if (newSize > fileContent.length) {
                const newContent = new Uint8Array(newSize);
                newContent.set(fileContent);
                fileContent = newContent;
            }
            fileContent.set(iov.buffer, descriptor.position);
            descriptor.position += iov.buffer.length;
            nwritten += iov.buffer.length;
        }

        this.files.set(descriptor.path, fileContent);
        this.saveToStorage();
        return { nwritten, errno: WASI_ESUCCESS };
    }

    read(fd: number, iovs: { buffer: Uint8Array, offset: number }[]): { nread: number, errno: number } {
        const descriptor = this.fds.get(fd);
        if (!descriptor) return { nread: 0, errno: WASI_EBADF };
        if (!(descriptor.rights.base & WASI_RIGHTS_FD_READ)) return { nread: 0, errno: WASI_EACCES };

        const fileContent = this.files.get(descriptor.path);
        if (!fileContent) return { nread: 0, errno: WASI_ENOENT };

        let nread = 0;
        for (const iov of iovs) {
            const readLength = Math.min(iov.buffer.length, fileContent.length - descriptor.position);
            if (readLength <= 0) break;

            const slice = fileContent.subarray(descriptor.position, descriptor.position + readLength);
            iov.buffer.set(slice);

            descriptor.position += readLength;
            nread += readLength;
        }

        return { nread, errno: WASI_ESUCCESS };
    }

    seek(fd: number, offset: bigint, whence: number): { new_offset: number, errno: number } {
        const descriptor = this.fds.get(fd);
        if (!descriptor) return { new_offset: 0, errno: WASI_EBADF };
        if (!(descriptor.rights.base & WASI_RIGHTS_FD_SEEK)) return { new_offset: 0, errno: WASI_EACCES };

        const fileContent = this.files.get(descriptor.path);
        if (!fileContent) return { new_offset: 0, errno: WASI_ENOENT };

        let newPosition = -1;
        switch (whence) {
            case 0: // SET
                newPosition = Number(offset);
                break;
            case 1: // CUR
                newPosition = descriptor.position + Number(offset);
                break;
            case 2: // END
                newPosition = fileContent.length + Number(offset);
                break;
        }

        if (newPosition < 0) return { new_offset: 0, errno: WASI_EINVAL };

        descriptor.position = newPosition;
        return { new_offset: descriptor.position, errno: WASI_ESUCCESS };
    }

    tell(fd: number): { offset: number, errno: number } {
        const descriptor = this.fds.get(fd);
        if (!descriptor) return { offset: 0, errno: WASI_EBADF };
        if (!(descriptor.rights.base & WASI_RIGHTS_FD_TELL)) return { offset: 0, errno: WASI_EACCES };
        return { offset: descriptor.position, errno: WASI_ESUCCESS };
    }

    stat(path: string): { stat: any, errno: number } {
        const fileContent = this.files.get(path);
        if (fileContent === undefined) return { stat: null, errno: WASI_ENOENT };

        return {
            stat: {
                dev: 1,
                ino: 0, // Inode number - not really applicable
                filetype: WASI_FILETYPE_REGULAR_FILE,
                nlink: 1,
                size: fileContent.length,
                atim: BigInt(0),
                mtim: BigInt(0),
                ctim: BigInt(0),
            },
            errno: WASI_ESUCCESS
        };
    }

    fstat(fd: number): { stat: any, errno: number } {
        const descriptor = this.fds.get(fd);
        if (!descriptor) return { stat: null, errno: WASI_EBADF };
        if (!(descriptor.rights.base & WASI_RIGHTS_FD_FILESTAT_GET)) return { stat: null, errno: WASI_EACCES };

        return this.stat(descriptor.path);
    }
    
    unlink(path: string): number {
        if (this.files.has(path)) {
            this.files.delete(path);
            this.saveToStorage();
            return WASI_ESUCCESS;
        }
        return WASI_ENOENT;
    }

    getFdStat(fd: number): { stat: any, errno: number } {
        if (fd < 3) { // Stdio
            return {
                stat: {
                    fs_filetype: WASI_FILETYPE_CHARACTER_DEVICE,
                    fs_flags: 0,
                    fs_rights_base: WASI_RIGHTS_FD_READ | WASI_RIGHTS_FD_WRITE,
                    fs_rights_inheriting: 0n,
                },
                errno: WASI_ESUCCESS,
            };
        }

        const descriptor = this.fds.get(fd);
        if (!descriptor) return { stat: null, errno: WASI_EBADF };

        return {
            stat: {
                fs_filetype: WASI_FILETYPE_REGULAR_FILE,
                fs_flags: descriptor.flags,
                fs_rights_base: descriptor.rights.base,
                fs_rights_inheriting: descriptor.rights.inheriting,
            },
            errno: WASI_ESUCCESS,
        };
    }
}
