const std = @import("std");
const print = std.debug.print;
const installation = @import("installation.zig");
pub fn buildProject(allocator: std.mem.Allocator) !void {
    const install = try installation.getInstallation();
    print("Building c code\n", .{});
    const cwd = std.fs.cwd();
    cwd.makeDir("build") catch {};
    const buildPath = try cwd.realpathAlloc(allocator, "build");
    defer allocator.free(buildPath);
    try install.copyBuildTemplate(allocator, buildPath);

    var child = std.process.Child.init(&[_][]const u8{ install.locs.zig, "cc", "src/main.c", "-target", "wasm32-wasi", "-shared-memory", "-max-memory=1073741824", "-I./gama", "-o", "build/game.wasm" }, allocator);
    const term = try child.spawnAndWait();

    switch (term) {
        .Exited => |code| if (code == 0) {
            print("Build succesfull\n", .{});
        } else {
            print("Command exited with code: {d}\n", .{code});
        },
        .Stopped => |code| {
            print("Command stopped with code: {any}\n", .{code});
        },
        .Unknown => |code| {
            print("Command exited unexpectedly with code {any}\n", .{code});
        },
        .Signal => |signal| {
            print("Command exited with signal: {any}\n", .{signal});
        },
    }
    print("Done -------------- \n", .{});
}
