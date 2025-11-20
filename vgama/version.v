module vgama

pub struct Version {
pub:
	major u16
	minor u16 = 1
	patch u16
}

pub fn (v Version) str() string {
	return '${v.major}.${v.minor}.${v.patch}'
}

pub fn Version.parse(txt string) !Version {
	start := if txt[0] == `v` { 1 } else { 0 }
	parts := txt[start..].split('.').map(|s| u16(s.int()))
	return Version{parts[0], parts[1], parts[2]}
}
