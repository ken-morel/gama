module vgama

import os

pub fn (p Project) package_native(inst Installation) ! {
	conf := p.get_conf()!

	// 1. Define paths
	template_path := os.join_path(inst.runners, 'native', 'installer.nsi')
	build_dir := p.build_path('windows')
	os.mkdir_all(build_dir) or { return error('failed to create windows build directory: ${err}') }

	nsi_script_path := os.join_path(build_dir, 'installer.nsi')

	// 2. Read template
	mut template := os.read_file(template_path) or {
		return error('Failed to read NSIS template at ${template_path}: ${err}')
	}

	// 3. Perform string replacements
	template = template.replace('{{project.name}}', conf.name)
	template = template.replace('{{project.version}}', conf.version.str())
	template = template.replace('{{author.name}}', conf.author)

	// 4. Write final NSIS script
	os.write_file(nsi_script_path, template)!

	// 5. Execute makensis
	nsis := $if windows { 'nsis' } $else { 'makensis' }
	cmd := '${nsis} ${nsi_script_path}'
	println('Packaging with: ${cmd}')
	res := os.execute(cmd)

	return if res.exit_code != 0 {
		error('Failed to package app: ${res.output}')
	} else {
		println(res.output)
		return
	}
}
