module gama

import os

pub fn (p Project) clean() ! {
	conf := p.get_conf()!
	os.rmdir_all(os.join_path(p.path, 'build', 'gen')) or {}
	os.rm(os.join_path(p.path, 'build', 'native', conf.name)) or {}
	os.rm(os.join_path(p.path, 'build', 'native', conf.name + '.exe')) or {}
	os.rm(os.join_path(p.path, 'build', 'web', conf.name + '.wasm')) or {}
}
