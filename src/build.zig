const std = @import("std");
const print = std.debug.print;
const installation = @import("installation.zig");
pub fn buildProject(allocator: std.mem.Allocator) !void {
    const install = try installation.getInstallation();
    print("Building c code", .{});
    const cwd = std.fs.cwd();
    cwd.makeDir("build") catch {};
    const buildPath = try cwd.realpathAlloc(allocator, "build");
    defer allocator.free(buildPath);
    try install.copyBuildTemplate(allocator, buildPath);
}
