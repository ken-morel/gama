module vgama

import gg
import math

type GmColor = u32

pub fn (c GmColor) to_gg() gg.Color {
	alpha := u8(math.sqrt(f64(c & 0xFF) / f64(255)) * 255)
	return gg.rgba(u8((c & 0xFF000000) >> 24), u8((c & 0x00FF0000) >> 16), u8((c & 0x0000FF00) >> 8),
		alpha)
}
