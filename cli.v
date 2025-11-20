module main

import cli
import os
import vgama
import term

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
				execute:     fn (cmd cli.Command) ! {
					println('Welcome to gama project creation assistant, let me load a few things')
					installation := vgama.Installation.dev('/home/engon/gama')
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
					mut template := &vgama.GamaTemplate(unsafe { nil })
					templateloop: for template == unsafe { nil } {
						for index, tmpl in templates {
							print(term.cyan(' ${index}) '))
							print(term.green(tmpl.name))
							println(term.gray('  ${tmpl.description}'))
						}
						index := os.input(term.blue('> ')).int()
						if index < 0 || index > templates.len {
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
					println("Will you host your project on a github repo? Type it's name as user/reponame, if you don't know what it is, just hit enter")
					repo := os.input_opt(term.blue('> '))
					conf := vgama.ProjectConf{
						name:        name
						description: desc
						repo:        repo or { '' }
						gama:        vgama.ProjectGamaConf{
							version: installation.get_gama_version() or {
								println(term.fail_message(err.str()))
								vgama.Version{}
							}
						}
					}

					vgama.Project.generate(installation, conf, template) or {
						println(term.fail_message('Error generating the project: ${err}'))
						return err
					}
					println(term.ok_message('${name} generated successfuly!'))
					return
				}
			},
			cli.Command{
				name:        'build'
				usage:       'build [-r]'
				description: 'Builds the current gama project'
				flags:       [
					cli.Flag{
						name:        'run'
						abbrev:      'r'
						description: 'Run the project after building'
						required:    false
					},
				]
				execute:     fn (cmd cli.Command) ! {
					run_after_build := cmd.flags.get_bool('run') or { false }
					project := vgama.Project.find_at(os.getwd()) or {
						println(term.fail_message('Not in a gama project. (Could not find gama.toml)'))
						return error('Not a gama project')
					}

					println(term.ok_message('Building project at: ${project.path}'))
					installation := vgama.Installation.dev('/home/engon/gama')

					project.build_native(installation) or {
						println(term.fail_message('Build failed: ${err}'))
						return err
					}
					println(term.ok_message('Build successful!'))
					return
				}
			},
		]
	}
	app.setup()
	app.parse(os.args)
}
