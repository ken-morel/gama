module vgama

import os

pub fn (p Project) package_native(inst Installation) ! {
	conf := p.get_conf()!

	// 1. Define paths
	template_path := os.join_path(inst.runners, 'native', 'installer.nsi')
	build_dir := p.build_path('nsis')
	os.mkdir_all(build_dir) or { return error('failed to create nsis build directory: ${err}') }

	nsi_script_path := os.join_path(build_dir, 'installer.nsi')

	// 2. Read template
	mut template := os.read_file(template_path) or {
		return error('Failed to read NSIS template at ${template_path}: ${err}')
	}
	// 4. Write final NSIS script
	os.write_file(nsi_script_path, conf.substitute(template))!

	// 5. Execute makensis
	nsis := $if windows { 'nsis' } $else { 'makensis' }
	cmd := '${nsis} ${nsi_script_path}'
	println('Packaging with: ${cmd}')
	res := os.execute(cmd)

	if res.exit_code != 0 {
		error('Failed to package app: ${res.output}')
	} else {
		println(res.output)
	}
}
