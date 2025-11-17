//! By convention, root.zig is the root source file when making a library.
const std = @import("std");
const print = std.debug.print;
const config = @import("config.zig");
const data = @import("data.zig");
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
        \\    - -std=11
        ,
        .sub_path = ".clangd",
        .flags = .{},
    });
}

pub fn initProject(allocator: std.mem.Allocator, c: InitProjectConfig) !void {
    const cwd = std.fs.cwd();
    const cwdPath = try cwd.realpathAlloc(allocator, ".");
    defer allocator.free(cwdPath);
    const appData = try data.locate();
    try config.create();
    try createEditorConfig(cwd);
    try cwd.makeDir("src");
    try cwd.makeDir("gama");

    try cwd.makeDir("assets");
    const templatePath = try std.fs.path.join(allocator, &[_][]const u8{ appData.templates, c.template });
    defer allocator.free(templatePath);
    _ = std.fs.cwd().openDir(templatePath, .{}) catch |err| switch (err) {
        error.FileNotFound => {
            print("Error: template '{s}' not found", .{c.template});
            return err;
        },
        else => {
            print("Error opening template folder {}", .{err});
            return err;
        },
    };
    print("Copying {s} to {s}", .{ templatePath, cwdPath });
    const dest = try cwd.realpathAlloc(allocator, "gama");
    defer allocator.free(dest);

    try futils.copyDir(allocator, templatePath, cwdPath);
    try futils.copyDir(allocator, appData.gama, dest);
}
