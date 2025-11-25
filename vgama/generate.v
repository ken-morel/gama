module vgama

import os
import term

pub fn Project.generate(inst Installation, conf ProjectConf, template GamaTemplate) !Project {
	project_dir := conf.name
	os.mkdir(project_dir) or {
		println(term.warn_message('Could not create project directory: ${err}'))
	}

	os.mkdir(os.join_path(project_dir, 'assets')) or {}

	template.copy_to(project_dir) or {
		println(term.warn_message('Could not copy template to directory: ${err}'))
	}

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
	project.copy_build_native_artifacts(inst) or {
		println(term.warn_message('Error copying ubild artifacts: ${err}'))
	}
	return project
}
