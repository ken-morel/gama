const std = @import("std");
const print = std.debug.print;
const installation = @import("installation.zig");
pub fn buildProject(allocator: std.mem.Allocator) !void {
    const install = try installation.getInstallation();
    print("Building c code");
    const cwd = std.fs.cwd();
    _ = cwd.makeDir("build");
    const buildPath = cwd.realpathAlloc(allocator, "build");
    defer allocator.free(buildPath);
    try install.copyBuildTemplate(allocator, buildPath);
}
