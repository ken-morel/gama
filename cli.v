module main

import cli
import os
import vgama
import term
import time

struct Compiler {
	name string
	path string
}

const compiler_descriptions = {
	'clang': 'A relatively fast compiler for modern systems'
	'gcc':   'Robust compiler(comes with code blocks)'
	'tcc':   'Tiny and extremely fast compiler(but the resulting app is not as fast)'
	'msvc':  'A compiler for windows systems, support not guaranteed'
}

fn find_compilers() []Compiler {
	mut found := []Compiler{}
	mut locations := os.getenv('PATH').split(os.path_separator)
	compilers := ['clang', 'gcc', 'tcc', 'msvc']

	$if windows {
		locations << 'C:/CodeBlocks/MinGW/bin'
		locations << 'C:/Program Files/CodeBlocks/MinGW/bin'
		locations << 'C:/Program Files(x86)/CodeBlocks/MinGW/bin'
		locations << 'C:/MinGW/bin'
	} $else {
		locations << '/usr/bin'
		locations << '/usr/local/bin'
	}

	for loc in locations {
		for compiler in compilers {
			mut exe_path := os.join_path(loc, compiler)
			$if windows {
				exe_path += '.exe'
			}
			if os.exists(exe_path) {
				found << Compiler{compiler, exe_path}
			}
		}
	}

	return found
}

struct Watcher {
mut:
	last_mtime i64    @[required]
	pattern    string @[required]
}

fn Watcher.new(pattern string) !Watcher {
	mut w := Watcher{0, pattern}
	w.last_mtime, _ = w.get_last_mtime()
	return w
}

fn (w Watcher) get_last_mtime() (i64, string) {
	mut max_mtime := i64(0)
	mut max_f := ''
	files := os.glob(w.pattern) or { return 0, '' }
	for file in files {
		mtime := os.inode(file).mtime
		if mtime > max_mtime {
			max_mtime = mtime
			max_f = file
		}
	}
	return max_mtime, max_f
}

fn (mut w Watcher) poll() string {
	ctime, mf := w.get_last_mtime()
	if ctime > w.last_mtime {
		w.last_mtime = ctime
		return mf
	}
	return ''
}

fn get_project() !vgama.Project {
	return vgama.Project.find_at(os.getwd()) or {
		println(term.fail_message('Not in a gama project. (Could not find gama.toml)'))
		return error('Not a gama project')
	}
}

fn get_installation() !vgama.Installation {
	return vgama.Installation.folder(os.dir(os.executable()))
}

fn main() {
	mut app := cli.Command{
		name:        'gama'
		description: 'Gama project manager'
		execute:     fn (cmd cli.Command) ! {
			println("Invalid command, run 'gama help' for help")
			return
		}
		commands:    [
			cli.Command{
				name:        'create'
				usage:       'create'
				description: 'Create a new gama project with the assistant'
				execute:     generator_assistant
			},
			cli.Command{
				name:        'build'
				usage:       'build [-r] [-tcc]'
				description: 'Builds the current gama project'
				flags:       [
					cli.Flag{
						name:        'run'
						abbrev:      'r'
						description: 'Run the project after building'
						required:    false
					},
					cli.Flag{
						name:        'tcc'
						abbrev:      'tcc'
						description: 'Build the project using builtin tcc compiler'
						required:    false
					},
				]
				execute:     fn (cmd cli.Command) ! {
					run_after_build := cmd.flags.get_bool('run') or { false }
					force_tcc := cmd.flags.get_bool('run') or { false }
					project := get_project()!

					println(term.ok_message('Building project at: ${project.path}'))
					installation := get_installation()!

					project.build_native(installation, force_tcc) or {
						println(term.fail_message('Build failed: ${err}'))
						return
					}
					println(term.ok_message('Build successful!'))
					if run_after_build {
						project.run_native_build(true) or {
							println(term.fail_message('Error running build: ${err}'))
						}
					}
					return
				}
			},
			cli.Command{
				name:        'run'
				usage:       'run'
				description: 'Run the built project'
				execute:     fn (_ cli.Command) ! {
					project := get_project()!
					println(term.ok_message('Running project at: ${project.path}'))

					project.run_native_build(true) or {
						println(term.fail_message('Error running build: ${err}'))
					}
				}
			},
			cli.Command{
				name:        'dev'
				usage:       'dev [-tcc]'
				description: 'Build and re-run the project on code changes'
				flags:       [
					cli.Flag{
						name:        'tcc'
						abbrev:      'tcc'
						description: 'Build the project using builtin tcc compiler'
						required:    false
					},
				]
				execute:     fn (cmd cli.Command) ! {
					force_tcc := cmd.flags.get_bool('tcc') or { false }
					inst := get_installation()!
					project := get_project()!
					println(term.ok_message('Running project at: ${project.path} in dev mode'))

					// Ensure we only watch source files, not binaries
					watch_path := os.join_path(project.path, 'src', '**')
					println('Watching: ${os.glob(watch_path)!}')

					mut w := Watcher.new(watch_path) or {
						println(term.fail_message('Failed watching directory'))
						return err
					}

					loop_dev: for {
						exe := project.build_native(inst, force_tcc) or {
							println(term.fail_message('Error building: ${err}'))
							time.sleep(time.second * 2)
							continue
						}

						mut process := os.new_process(exe)
						process.run()

						process_exit_ch := chan bool{}

						spawn fn (mut p os.Process, ch chan bool) {
							p.wait()
							ch <- true or { return }
						}(mut process, process_exit_ch)

						loop_wait: for {
							select {
								_ := <-process_exit_ch {
									println(term.warn_message('Process exited.'))
									break loop_dev
								}
								1 * time.second {
									changed_file := w.poll()
									if changed_file != '' {
										println(term.warn_message('Code changed in ${changed_file}, restarting...'))

										if process.is_alive() {
											process.signal_kill()
											process.wait()
										}
										break loop_wait
									}
								}
							}
						}
					}
				}
			},
			cli.Command{
				name:        'lib'
				usage:       'lib <cmd>'
				description: "manage the project's gama library"
				args:        [
					'cmd',
				]
				execute:     fn (_ cli.Command) ! {
					println(term.warn_message('No command specified'))
				}
				commands:    [
					cli.Command{
						name:        'reset'
						usage:       'reset'
						description: "reset the project's gama library to the cli tool's verion"
						execute:     fn (_ cli.Command) ! {
							installation := get_installation()!
							project := get_project()!
							project.reset_gama(installation) or {
								println(term.fail_message('Error reseting gama: ${err}'))
								return err
							}
							println(term.ok_message('Reset gama successfuly'))

							return
						}
					},
				]
			},
			cli.Command{
				name:        'package'
				usage:       'package'
				description: 'Package the current gama project into a setup'
				execute:     package_project
			},
		]
	}

	app.setup()
	app.parse(os.args)
}

fn package_project(cmd cli.Command) ! {
	project := get_project()!
	installation := get_installation()!

	println(term.ok_message('Packaging project at: ${project.path}'))

	project.package_native(installation) or {
		println(term.fail_message('Packaging failed: ${err}'))
		return
	}
	println(term.ok_message('Packaging successful!'))
	return
}

@[unsafe]
fn generator_assistant(cmd cli.Command) ! {
	println('Welcome to gama project creation assistant, let me load a few things')
	installation := get_installation()!
	gama_version := installation.get_gama_version() or {
		println(term.fail_message('Error getting gama version: ${err}'))
		return err
	}
	templates := installation.get_templates() or {
		println(term.fail_message('Error loading gama templates: ${err}'))
		return err
	}
	println(term.ok_message('Loaded installation with gama version ${gama_version} and ${templates.len} templates'))
	mut name := ''
	nameloop: for {
		println('Enter the project name, the project name should only contain lowercase letters, numbers, and underscores')
		mut rm := false
		for c in os.input(term.blue('> ')).runes() {
			n := if c >= `A` && c <= `Z` { c - (`A` - `a`) } else { c } // convert to lower
			if (n >= `a` && n <= `z`) || (n >= `0` && n <= `9`) || n == `_` {
				name += n.str()
			} else {
				rm = true
			}
		}
		if rm {
			println(term.warn_message('Some characters had to be removed'))
		}
		if name.len < 3 || name.len > 15 {
			println(term.warn_message('The name should have at least 3 and at most 15 letters'))
			name = ''
			continue nameloop
		}
		print('The project will be named: ')
		print(term.bold(name))
		print(' is that okay? (Yep / nop)')
		if os.input(term.blue('? ')) in [
			'n',
			'N',
			'no',
			'nope',
			'nop',
			'nah',
		] {
			name = ''
			continue nameloop
		} else {
			break
		}
	}
	print('Oh sounds cool, what is ')
	print(term.bold(name))
	println(' about?')
	desc := os.input(term.blue('> '))
	println('And what do you want to use as template for your application?')
	mut template := &vgama.GamaTemplate(nil)
	templateloop: for template == nil {
		for index, tmpl in templates {
			print(term.cyan(' ${index}) '))
			print(term.green(tmpl.name))
			println(term.gray('  ${tmpl.description}'))
		}
		index := os.input(term.blue('> ')).int()
		if index < 0 || index >= templates.len {
			println(term.fail_message('Invalid index'))
			continue templateloop
		} else {
			if os.input('so we use template ${templates[index].name}? (Yep/nop)') in [
				'n',
				'N',
				'no',
				'nop',
				'nope',
			] {
				continue
			} else {
				template = &templates[index]
				break templateloop
			}
		}
	}
	println('Looking for compilers...')
	compilers := find_compilers()

	println('Please choose a compiler, you can still change it latter')

	mut compiler := &Compiler(nil)
	compilerloop: for compiler == nil {
		print(term.cyan(' 0) '))
		println(term.green('use builtin tcc compiler'))
		for index, comp in compilers {
			print(term.cyan(' ${index + 1}) '))
			print(term.green(comp.name))
			print(' at ')
			print(comp.path)
			print('  ')
			println(term.gray(compiler_descriptions[comp.name] or { 'no description' }))
		}
		mut index := os.input(term.blue('> ')).int()
		if index == 0 {
			compiler = nil
			break compilerloop
		}
		index -= 1
		if index < 0 || index > templates.len {
			println(term.fail_message('Invalid index'))
			continue compilerloop
		} else {
			if os.input('so we use compiler ${compilers[index]}? (Yep/nop)') in [
				'n',
				'N',
				'no',
				'nop',
				'nope',
			] {
				continue
			} else {
				compiler = &compilers[index]
				break compilerloop
			}
		}
	}
	conf := vgama.ProjectConf{
		name:        name
		description: desc
		gama:        vgama.ProjectGamaConf{
			version:  installation.get_gama_version() or {
				println(term.fail_message(err.str()))
				vgama.Version{}
			}
			compiler: if compiler != nil { compiler.path } else { '' }
		}
	}

	vgama.Project.generate(installation, conf, template) or {
		println(term.fail_message('Error generating the project: ${err}'))
		return
	}
	println(term.ok_message('${name} generated successfuly!'))
	return
}
