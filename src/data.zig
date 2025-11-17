const Locations = struct {
    gama: []const u8,
    templates: []const u8,
};

pub fn locate() !Locations {
    const devDir = "/home/engon/gama/";
    return .{ .gama = devDir ++ "lib", .templates = devDir ++ "templates" };
}
