module main

import cli
import os
import gama
import term
import time
import rand

#flag -std=c99
#flag -DWIN32_FULL
#flag -static
#flag -static-libgcc
#flag -static-libstdc++

struct Compiler {
	name string
	path string
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

fn get_project() !gama.Project {
	return gama.Project.find_at(os.getwd()) or {
		println(term.fail_message('Not in a gama project. (Could not find gama.toml)'))
		return error('Not a gama project')
	}
}

fn get_installation() !gama.Installation {
	mut location := os.dir(os.executable())
	lnx := location.starts_with('/usr/bin')
	if lnx {
		location = '/usr/share/gama'
	}
	return gama.Installation{
		lib:       if lnx { '/usr/lib/gama' } else { os.join_path(location, 'lib') }
		templates: os.join_path(location, 'templates')
		runners:   os.join_path(location, 'runners')
		assets:    os.join_path(location, 'assets')
		tcc:       os.join_path(location, 'compilers', 'tcc')
		zig:       os.join_path(location, 'compilers', 'zig')
	}
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
				name:        'update'
				usage:       'update'
				description: 'Updates the projects gama toolchain'
				execute:     fn (_ cli.Command) ! {
					inst := get_installation()!
					p := get_project()!
					p.update_toolchain(inst)!
					println(term.ok_message('Updated toolchain successfully'))
				}
			},
			cli.Command{
				name:        'clean'
				usage:       'clean'
				description: 'Delete all build artifacts and generated files without removing the required toolchain'
				execute:     fn (_ cli.Command) ! {
					p := get_project()!
					p.clean() or {
						pintln(term.fail_message('Error cleaning the project files: ${err}'))
					}
					println(term.ok_message('Updated toolchain successfully'))
				}
			},
			cli.Command{
				name:        'build'
				usage:       'build [mode] [-r] [-cc name]'
				description: 'Builds the current gama project'
				flags:       [
					cli.Flag{
						name:        'run'
						abbrev:      'r'
						description: 'Run the project after building'
						required:    false
					},
					cli.Flag{
						flag:        .string
						name:        'cc'
						abbrev:      'cc'
						description: 'Use an alternative compiler'
						required:    false
					},
				]
				execute:     fn (cmd cli.Command) ! {
					run_after_build := cmd.flags.get_bool('run') or { false }
					use_cc := cmd.flags.get_string('cc') or { '' }
					project := get_project()!

					println(term.ok_message('Building project at: ${project.path}'))
					installation := get_installation()!

					project.build_native(installation, use_cc) or {
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
				commands:    [
					cli.Command{
						name:        'web'
						usage:       'build web [-r] [-reset]'
						description: 'Builds the project for the web'
						flags:       [
							cli.Flag{
								name:        'run'
								abbrev:      'r'
								description: 'Run the project after building'
								required:    false
							},
							cli.Flag{
								name:        'reset'
								abbrev:      'reset'
								description: 'Run the project after building'
								required:    false
							},
							cli.Flag{
								name:        'port'
								abbrev:      'p'
								description: 'The port to launch the server at'
								required:    false
							},
						]
						execute:     fn (cmd cli.Command) ! {
							run_after_build := cmd.flags.get_bool('run') or { false }
							reset := cmd.flags.get_bool('reset') or { false }
							port := cmd.flags.get_int('port') or { 8095 }
							println('BUilding project for the web')

							project := get_project()!
							println(term.ok_message('Building project at: ${project.path}'))
							installation := get_installation()!

							project.build_web(installation, reset) or {
								println(term.fail_message('${err}'))
							}
							if run_after_build {
								project.run_web_build(installation, port) or {
									println(term.fail_message('${err}'))
								}
							}
						}
					},
				]
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
				commands:    [
					cli.Command{
						name:        'web'
						usage:       'web [-p 8095]'
						description: 'run web build at port'
						execute:     fn (cmd cli.Command) ! {
							port := cmd.flags.get_int('port') or { 8095 }
							project := get_project()!
							inst := get_installation()!
							project.run_web_build(inst, port) or {
								println(term.fail_message('Error running web build at port ${port}: ${err}'))
							}
						}
					},
				]
			},
			cli.Command{
				name:        'dev'
				usage:       'dev [cc]'
				description: 'Build and re-run the project on code changes using compiler [cc]'

				execute: fn (cmd cli.Command) ! {
					cc := cmd.args[0] or { '.tcc' }
					inst := get_installation()!
					project := get_project()!

					// Ensure we only watch source files, not binaries
					watch_path := os.join_path(project.path, 'src', '**')

					mut w := Watcher.new(watch_path) or {
						println(term.fail_message('Failed watching directory'))
						return err
					}

					loop_dev: for {
						exe := project.build_native(inst, cc) or {
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
				name:        'package'
				usage:       'package'
				description: 'Package the current gama project into a setup'
				execute:     fn (_ cli.Command) ! {
					project := get_project()!
					installation := get_installation()!

					println(term.ok_message('Packaging project at: ${project.path}'))

					project.package_native(installation) or {
						println(term.fail_message('Packaging failed: ${err}'))
						return
					}
					println(term.ok_message('Packaging successful!'))
				}
			},
			cli.Command{
				name:        'bake'
				usage:       'bake [-c]'
				description: 'Build project assets files'
				flags:       [
					cli.Flag{
						name:        'clean'
						abbrev:      'c'
						description: 'Delete baked files and rebuild'
						required:    false
					},
				]

				execute: fn (cmd cli.Command) ! {
					clean := cmd.flags.get_bool('clean') or { false }
					installation := get_installation()!
					project := get_project()!
					project.bake(installation, clean) or {
						println(term.fail_message('Error baking project assets: ${err}'))
						return err
					}
					println(term.ok_message('baking complete'))
				}
			},
			cli.Command{
				name:        'zig'
				usage:       'zig ...'
				description: 'call gama zig compiler'
				execute:     fn (cmd cli.Command) ! {
					error('Dummy')
				}
			},
			cli.Command{
				name:        'tcc'
				usage:       'tcc ...'
				description: "call gama's tcc ompiler"
				execute:     fn (cmd cli.Command) ! {
					error('Dummy')
				}
			},
		]
	}
	if os.args.len > 1 {
		match os.args[1] {
			'tcc' {
				inst := get_installation()!
				os.execvp(inst.tcc_exe()!, os.args[2..]) or {
					println(term.fail_message('Error launching the app build executable: ${err}'))
					1
				}
			}
			'zig' {
				inst := get_installation()!
				os.execvp(inst.zig_exe()!, os.args[2..]) or {
					println(term.fail_message('Error launching the app build executable: ${err}'))
				}
			}
			'zcc' {
				inst := get_installation()!
				mut args := ['cc']
				args << os.args[2..]
				os.execvp(inst.zig_exe()!, args) or {
					println(term.fail_message('Error launching the app build executable: ${err}'))
				}
			}
			else {
				app.setup()
				app.parse(os.args)
			}
		}
	}
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
	mut template := &gama.GamaTemplate(nil)
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
	conf := gama.ProjectConf{
		name:        name
		description: desc
		uuid:        rand.uuid_v7()
		gama:        gama.ProjectGamaConf{
			version: installation.get_gama_version() or {
				println(term.fail_message(err.str()))
				gama.Version{}
			}
		}
	}

	gama.Project.generate(installation, conf, template) or {
		println(term.fail_message('Error generating the project: ${err}'))
		return
	}
	println(term.ok_message('${name} generated successfuly!'))
	return
}
