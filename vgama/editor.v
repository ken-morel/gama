module vgama

import os

pub fn (p Project) add_editor_config(conf ProjectConf, inst Installation) ! {
	os.write_file(os.join_path(p.path, '.clangd'), '
CompileFlags:
  Add:
    - -Iinclude/
---
If:
  PathMatch: .*\\.h\$
CompileFlags:
  Add:
    - -x
    - c
') or {}
	os.write_file(os.join_path(p.path, '${conf.name}.cbp'), '
	<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<CodeBlocks_project_file>
	<FileVersion major="1" minor="6" />
	<Project>
		<Option title="${conf.name}" />
		<Option pch_mode="2" />
		<Build>
			<Target title="Debug">
				<Option output="bin/Debug/${conf.name}" prefix_auto="1" extension_auto="1" />
				<Option object_output="obj/Debug/" />
				<Option type="0" />
			</Target>
			<Target title="Release">
				<Option output="bin/Release/${conf.name}" prefix_auto="1" extension_auto="1" />
				<Option object_output="obj/Release/" />
				<Option type="0" />
			</Target>
		</Build>
		<Compiler>
			<Add directory="./include/" />
		</Compiler>
		<Linker>
			<Add option="-lm" />
			<Add library="vgama" />
			<Add directory="./build/native" />
		</Linker>
		<Unit filename="src/main.c">
			<Option compilerVar="CC" />
		</Unit>
		<Extensions>
			<code_completion>
				<search_path add="./include" />
			</code_completion>
		</Extensions>
	</Project>
</CodeBlocks_project_file>
') or {}

	os.write_file(os.join_path(p.path, '${conf.name}.sublime-project'), '
{
	"folders": [{"path": "."}],
	"build_systems": [
		{"name": "gama dev", "cmd": ["gama","dev"], "working_dir": "\$project_path","file_regex": "^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)\$",},
		{"name": "Build and run", "cmd": ["gama","build","-r"], "working_dir": "\$project_path","file_regex": "^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)\$",},
	]
}
') or {}
}
