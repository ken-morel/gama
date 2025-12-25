module vgama

import veb
import os
import term

pub struct Context {
	veb.Context
}

pub struct RunWebApp {
	veb.Middleware[Context]
	veb.StaticHandler
	build_dir string @[required]
}

pub fn (mut app RunWebApp) index(mut ctx Context) veb.Result {
	return ctx.redirect('/index.html')
}

pub fn (p Project) build_web(inst Installation, reset bool) ! {
	conf := p.get_conf()!

	build_dir := p.build_path('web')
	os.mkdir_all(build_dir) or { return error('failed to create build directory: ${err}') }
	p.copy_build_web_artifacts(inst, reset)!
	source_files := p.get_src_c_files()!
	if source_files.len == 0 {
		return error('No c source files in src directory')
	}
	inst.copy_gama(os.join_path(p.path, 'include'), false) or {
		println(term.warn_message('Failed to copy gama'))
	}

	include_path := os.join_path(p.path, 'include')

	output := os.join_path(build_dir, '${conf.name}.wasm')

	res := os.execute('zig cc -target wasm32-wasi -g ${source_files.join(' ')} -I${include_path} -lc -lm -Wl,--export-all-symbols -Wl,--no-entry -o ${output} -D__ZIG_CC__')

	if res.exit_code != 0 {
		return error('Failed to build app: ${res.output}')
	}
}

pub fn (p Project) run_web_build(inst Installation) ! {
	conf := p.get_conf()!

	build_dir := p.build_path('web')

	mut app := &RunWebApp{
		build_dir: build_dir
	}
	app.handle_static(build_dir, true)!

	app.use(
		handler: fn (mut ctx Context) bool {
			println('adding headers')
			ctx.res.header.add(.cross_origin_embedder_policy, 'require-corp')
			ctx.res.header.add(.cross_origin_opener_policy, 'same-origin')
			return true
		}
	)

	// app.mount_static_folder_at(build_dir, '/')
	veb.run[RunWebApp, Context](mut app, 8095)
}

pub fn (p Project) copy_build_web_artifacts(inst Installation, reset bool) ! {
	conf := p.get_conf()!
	build_dir := p.build_path('web')
	runner_path := os.join_path(inst.runners, 'web')
	os.mkdir_all(build_dir) or {}
	for filename in os.ls(runner_path)! {
		src := os.join_path(runner_path, filename)
		dest := os.join_path(build_dir, filename)
		if !os.exists(dest) || reset {
			if filename.ends_with('.html') || filename.ends_with('.js') {
				code := os.read_file(src) or {
					println(term.warn_message('Error reading template from file ${src}: ${err}'))
					continue
				}
				os.write_file(dest, conf.substitute(code)) or {
					println(term.warn_message('Error writing to file ${dest}: ${err}'))
				}
			} else {
				os.cp(src, dest) or {
					println(term.warn_message('Error copying ${src} to ${dest}: ${err}'))
				}
			}
		}
	}
}
