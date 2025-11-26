module vgama

import os

struct ZigCC {
	exepath string
}

struct ZigBuildNativeOptions {
	files           []string @[required]
	include_path    string   @[required]
	executable_path string   @[required]
	lib_path        string   @[required]
}

fn (z ZigCC) build_native(opts ZigBuildNativeOptions) !string {
}
