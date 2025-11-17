const std = @import("std");

pub fn readLine(
    allocator: std.mem.Allocator,
    writer: anytype,
    reader: anytype,
    prompt: []const u8,
) ![]const u8 {
    while (true) {
        try writer.print("{s}: ", .{prompt});
        const input = try reader.readUntilDelimiterAlloc(allocator, '\n', 1024); // Max 1KB line
        if (input.len == 0) {
            allocator.free(input);
            try writer.print("Input cannot be empty. Please try again.\n", .{});
            continue;
        }
        return input;
    }
}

pub fn getStringInput(
    allocator: std.mem.Allocator,
    writer: anytype,
    reader: anytype,
    prompt: []const u8,
) ![]const u8 {
    return readLine(allocator, writer, reader, prompt);
}

pub fn getIntInput(
    allocator: std.mem.Allocator,
    writer: anytype,
    reader: anytype,
    prompt: []const u8,
    comptime T: type,
) !T {
    while (true) {
        const input_str = try readLine(allocator, writer, reader, prompt);
        defer allocator.free(input_str);

        const parsed_int = std.fmt.parseInt(T, input_str, 10); // Base 10

        switch (parsed_int) {
            .ok => |value| return value,
            .err => |err| {
                try writer.print("Invalid integer input: {s}. Please try again.\n", .{err});
            },
        }
    }
}

fn getBoolInput(
    allocator: std.mem.Allocator,
    writer: anytype,
    reader: anytype,
    prompt: []const u8,
) !bool {
    while (true) {
        const input_str = try readLine(allocator, writer, reader, prompt);
        defer allocator.free(input_str);

        var lower_input = try allocator.alloc(u8, input_str.len);
        defer allocator.free(lower_input);
        for (input_str, 0..) |char, i| {
            lower_input[i] = std.ascii.toLower(char);
        }

        if (std.mem.eql(u8, lower_input, "y") or std.mem.eql(u8, lower_input, "yes")) {
            return true;
        } else if (std.mem.eql(u8, lower_input, "n") or std.mem.eql(u8, lower_input, "no")) {
            return false;
        } else {
            try writer.print("Please enter 'y'/'yes' or 'n'/'no'. Try again.\n", .{});
        }
    }
}
