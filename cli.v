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
						println('Error getting gama version: ${err}')
						return err
					}
					println('Gama version ${gama_version}')
					templates := installation.get_templates() or {
						println('Error loading gama templates: ${err}')
						return err
					}
					mut name := ''
					nameloop: for {
						for c in os.input('Enter the project name, the project name should only contain lowercase letters, numbers, and underscores\n> ').runes() {
							n := if c >= `A` && c <= `Z` { c - (`A` - `a`) } else { c } // convert to lower
							if (n >= `a` && n <= `z`) || (n >= `0` && n <= `9`) || n == `_` {
								name += n.str()
							}
						}
						if name.len < 3 || name.len > 15 {
							println('The name should have at least 3 and at most 15 letters')
							continue nameloop
						}
						if os.input('The project will be named: "${name}" is that okay (Yep / nop)?') in [
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
					desc := os.input('Oh sounds cool, what is ${name} about?\n> ')
					println('And what do you want to use as template for your application?')
					mut template := &vgama.GamaTemplate(unsafe { nil })
					templateloop: for template == unsafe { nil } {
						for index, tmpl in templates {
							print(term.cyan(' ${index}) '))
							print(term.green(tmpl.name))
							println(term.gray('  ${tmpl.description}'))
						}
						index := os.input('> ').int()
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
					repo := os.input_opt("Will you host your project on a github repo? Type it's name as user/reponame, if you don't know what it is, just hit enter\n> ")
					conf := vgama.ProjectConf{
						name:        name
						description: desc
						repo:        repo or { '' }
						gama:        vgama.ProjectGamaConf{
							version: installation.get_gama_version() or {
								println(err)
								vgama.Version{}
							}
						}
					}

					_ = vgama.Project.generate(installation, conf, template)!

					return
				}
			},
		]
	}
	app.setup()
	app.parse(os.args)
}
