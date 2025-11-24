module vgama

import os

pub fn (p Project) add_editor_config(inst Installation) ! {
	os.write_file(os.join_path(p.path, '.clangd'), '
CompileFlags:
  Add:
    - -I${inst.lib}
---
If:
  PathMatch: .*\\.h\$
CompileFlags:
  Add:
    - -x
    - c
')!
}

pub fn Project.generate(inst Installation, conf ProjectConf, template GamaTemplate) !Project {
	project_dir := conf.name
	os.mkdir(project_dir)!

	os.mkdir(os.join_path(project_dir, 'assets'))!

	template.copy_to(project_dir)!

	project := Project{
		path: os.abs_path(project_dir)
	}
	project.set_conf(conf)!
	project.add_editor_config(inst)!
	return project
}
