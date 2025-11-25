module vgama

import os

pub struct Project {
pub:
	path string @[required]
}

pub fn (p Project) build_path(type string) string {
	return os.join_path(p.path, 'build', type)
}

pub fn Project.find_at(start string) ?Project {
	mut project_root := start
	for {
		if os.exists(os.join_path(project_root, 'gama.toml')) {
			break
		}
		parent := os.dir(project_root)
		if parent == project_root {
			return none
		}
		project_root = parent
	}
	return Project{project_root}
}

pub fn (p Project) get_conf() !ProjectConf {
	return ProjectConf.load(os.join_path(p.path, 'gama.toml'))
}

pub fn (p Project) set_conf(conf ProjectConf) ! {
	conf.save(os.join_path(p.path, 'gama.toml'))!
}
