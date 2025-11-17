const std = @import("std");
const futils = @import("futils.zig");
pub const Installation = struct {
    locs: struct {
        gama: []const u8,
        templates: []const u8,
        zig: []const u8,
        buildtemplate: []const u8,
    },
    pub fn copyTemplate(self: Installation, allocator: std.mem.Allocator, template: []const u8, to: []const u8) !void {
        const templatePath = try std.fs.path.join(allocator, &[_][]const u8{ .templates, self.locs.template });
        defer allocator.free(templatePath);
        _ = std.fs.cwd().openDir(templatePath, .{}) catch |err| switch (err) {
            error.FileNotFound => {
                std.debug.print("Error: template '{s}' not found", .{template});
                return err;
            },
            else => {
                std.debug.print("Error opening template folder {}", .{err});
                return err;
            },
        };
        try futils.copyDir(allocator, templatePath, to);
    }
    pub fn copyGama(self: Installation, allocator: std.mem.Allocator, to: []const u8) !void {
        try futils.copyDir(allocator, self.locs.gama, to);
    }
    pub fn copyBuildTemplate(self: *Installation, allocator: std.mem.Allocator, to: []const u8) !void {
        try futils.copyDir(allocator, self.locs.buildtemplate, to);
    }
};

pub fn getInstallation() !Installation {
    const devDir = "/home/engon/gama/";
    return .{
        .locs = .{
            .gama = devDir ++ "lib",
            .templates = devDir ++ "templates",
            .zig = "/home/engon/zig/zig",
            .buildtemplate = devDir ++ "buildtemplate",
        },
    };
}
