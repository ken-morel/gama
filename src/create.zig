//! By convention, root.zig is the root source file when making a library.
const std = @import("std");
const print = std.debug.print;
const config = @import("config.zig");
const installation = @import("installation.zig");
const futils = @import("futils.zig");

pub const InitProjectConfig = struct {
    template: []const u8,
};

pub fn createEditorConfig(wd: std.fs.Dir) !void {
    try wd.writeFile(.{
        .data =
        \\CompileFlags:
        \\  Add:
        \\    - -I/home/engon/gama/lib
        \\---
        \\If:
        \\  PathMatch: .*\.h$
        \\CompileFlags:
        \\  Add:
        \\    - -x
        \\    - c
        ,
        .sub_path = ".clangd",
        .flags = .{},
    });
}

pub fn initProject(allocator: std.mem.Allocator, c: InitProjectConfig) !void {
    const install = try installation.getInstallation();
    const cwd = std.fs.cwd();
    const cwdPath = try cwd.realpathAlloc(allocator, ".");
    defer allocator.free(cwdPath);
    try config.create();
    try createEditorConfig(cwd);
    try cwd.makeDir("src");
    try cwd.makeDir("gama");
    try cwd.makeDir("assets");

    try install.copyTemplate(allocator, c.template, cwdPath);
    const gamaDest = try cwd.realpathAlloc(allocator, "gama");
    defer allocator.free(gamaDest);
    try install.copyGama(allocator, gamaDest);
}
