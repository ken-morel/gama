const std = @import("std");

pub fn copyDir(allocator: std.mem.Allocator, src_path: []const u8, dest_path: []const u8) !void {
    std.fs.makeDirAbsolute(dest_path) catch |err|
        if (err != error.PathAlreadyExists)
            return err;

    var src_dir = try std.fs.openDirAbsolute(src_path, .{ .iterate = true });
    defer src_dir.close();

    var walker = try src_dir.walk(allocator);
    while (try walker.next()) |entry| {
        const current_src_path = try std.fs.path.join(allocator, &[_][]const u8{ src_path, entry.path });
        defer allocator.free(current_src_path);

        const current_dest_path = try std.fs.path.join(allocator, &[_][]const u8{ dest_path, entry.path });
        defer allocator.free(current_dest_path);

        switch (entry.kind) {
            .file => {
                try std.fs.copyFileAbsolute(current_src_path, current_dest_path, .{});
            },
            .directory => {
                try copyDir(allocator, current_src_path, current_dest_path);
            },
            else => {
                std.debug.print("Skipping non-file/directory entry: {s}\n", .{entry.path});
            },
        }
    }
}
