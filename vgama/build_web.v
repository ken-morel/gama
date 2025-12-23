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

pub fn (p Project) build_web(inst Installation) ! {
	conf := p.get_conf()!

	build_dir := p.build_path('web')
	os.mkdir_all(build_dir) or { return error('failed to create build directory: ${err}') }
	p.copy_build_web_artifacts(inst)!
	source_files := p.get_src_c_files()!
	if source_files.len == 0 {
		return error('No c source files in src directory')
	}
	inst.copy_gama(os.join_path(p.path, 'include'), false) or {
		println(term.warn_message('Failed to copy gama'))
	}

	include_path := os.join_path(p.path, 'include')

	output := os.join_path(build_dir, '${conf.name}.wasm')

	res := os.execute('zig cc -target wasm32-freestanding -g ${source_files.join(' ')} -I${include_path} -Wl,--export-all-symbols -Wl,--no-entry -o ${output} -D__ZIG_CC__')

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

pub fn (p Project) copy_build_web_artifacts(inst Installation) ! {}
