module vgama

import os
import term

pub fn Project.generate(inst Installation, conf ProjectConf, template GamaTemplate) !Project {
	project_dir := conf.name
	os.mkdir(project_dir) or {
		println(term.warn_message('Could not create project directory: ${err}'))
	}
	template.copy_to(project_dir) or {
		println(term.warn_message('Could not copy template to directory: ${err}'))
	}
	os.mkdir_all(os.join_path(project_dir, 'assets', 'images')) or {}
	os.cp(os.join_path(inst.assets, 'images', 'gama.png'), os.join_path(project_dir, 'assets',
		'images', 'icon.png')) or {
	}
	os.cp(os.join_path(inst.assets, 'images', 'gama.ico'), os.join_path(project_dir, 'assets',
		'images', 'icon.ico')) or {}

	project := Project{
		path: os.abs_path(project_dir)
	}
	project.reset_gama(inst) or {
		println(term.warn_message('Could not copy gama to directory: ${err}'))
	}
	project.set_conf(conf) or {
		println(term.warn_message('Could not create gama configuration: ${err}'))
	}
	project.add_editor_config(conf, inst) or {
		println(term.warn_message('Could not create editor configuration: ${err}'))
	}
	project.copy_build_native_artifacts(inst, true) or {
		println(term.warn_message('Error copying ubild artifacts: ${err}'))
	}
	return project
}
