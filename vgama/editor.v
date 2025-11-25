module vgama

import os

pub fn (p Project) add_editor_config(conf ProjectConf, inst Installation) ! {
	os.write_file(os.join_path(p.path, '.clangd'), '
CompileFlags:
  Add:
    - -I${inst.lib}
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
				<Compiler>
					<Add option="-g" />
				</Compiler>
			</Target>
			<Target title="Release">
				<Option output="bin/Release/${conf.name}" prefix_auto="1" extension_auto="1" />
				<Option object_output="obj/Release/" />
				<Option type="0" />
				<Compiler>
					<Add option="-O2" />
				</Compiler>
				<Linker>
					<Add option="-s" />
				</Linker>
			</Target>
		</Build>
		<Compiler>
			<Add option="-Weverything" />
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
}
