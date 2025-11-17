const std = @import("std");
const gama = @import("root.zig");
const input = @import("input.zig");
const print = std.debug.print;
const stdin = std.io.getStdIn().reader();
const stdout = std.io.getStdOut().writer();

const CLI_HELP =
    \\Gama cli
    \\usage:
    \\ gama dev
    \\ gama create
;

fn show_help() void {
    print(CLI_HELP, .{});
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    var args = std.process.args();
    _ = args.next();

    const command = args.next();
    if (command == null or std.mem.eql(u8, command.?, "help")) {
        show_help();
    } else if (std.mem.eql(u8, command.?, "init")) {
        const template = args.next() orelse "skeleton";
        gama.create.initProject(allocator, .{ .template = template }) catch |err| {
            print("Error creating project: {}\n", .{err});
        };
    } else if (std.mem.eql(u8, command.?, "build")) {
        gama.build.buildProject(allocator);
    } else {
        print("Invalid command: '{s}' use gama help to show help\n", .{command.?});
    }
}
