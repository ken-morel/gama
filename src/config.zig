const std = @import("std");
const toml = std.toml;

pub fn create() !void {
    try std.fs.cwd().writeFile(.{ .data =
        \\
    , .sub_path = "gama.toml" });
}
