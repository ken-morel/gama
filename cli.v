module main

import cli
import os
import vgama

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
					println('Welcome to gama project creation assistant')
					mut name := ''
					nameloop: for {
						for r in os.input('Enter the project name, the project name should only contain lowercase letters, numbers, and underscores\n> ') {
							if (r >= 97 && r <= 122) || (r >= 48 && r <= 57) || r == 95 {
								name += r.str()
							}
						}
						if name.len < 3 || name.len > 15 {
							println('The name should have at least 3 and at most 15 letters')
							continue nameloop
						}
						if os.input('The project will be named: "${name}" is that okay (yep / nop)?') in [
							'n',
							'N',
							'no',
							'nope',
							'nop',
							'nah',
						] {
							break nameloop
						} else {
							name = ''
						}
					}
					desc := os.input('Oh sounds cool, what is ${name} about?\n> ')
					repo := os.input_opt("Will you host your project on a github repo? Type it's name as user/reponame, if you don't know what it is, just hit enter\n> ")
					conf := vgama.ProjectConf{
						name:        name
						description: desc
						version:     'v0.1.0'
						repo:        repo or { '' }
						gama:        {
							version: installation.gamaVersion
						}
					}

					vgama.Project.generate(conf)!

					return
				}
			},
		]
	}
	app.setup()
	app.parse(os.args)
}
