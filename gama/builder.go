package gama

import (
	"fmt"
	"os/exec"
	"path"
	"runtime"
)

// winegcc src/main.c -o test.exe -lopengl32 -lgdi32
func buildProjectWine(name string, cfiles []string) error {
	cmd := exec.Command(
		"winegcc",
		append(
			cfiles,
			path.Join("build", name),
			"-g",
			"-lglfw",
			"-lGL",
			"-lm",
			"-lXrandr",
			"-lXi",
			"-lX11",
			"-lXxf86vm",
			"-lpthread",
		)...,
	)
	return runBuildCommand(cmd)
}

// gcc ./test/src/main.c -o ./test/test -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread && ./test/test
func buildProjectLinux(name string, cfiles []string) error {
	cmd := exec.Command(
		"gcc",
		append(
			cfiles,
			path.Join("build", name),
			"-g",
			"-lglfw",
			"-lGL",
			"-lm",
			"-lXrandr",
			"-lXi",
			"-lX11",
			"-lXxf86vm",
			"-lpthread",
		)...,
	)
	return runBuildCommand(cmd)
}

// gcc src/main.c -o test.exe -lopengl32 -lgdi32
func buildProjectWindows(name string, cfiles []string) error {
	gccPath := config.Config.Build.GCC
	if gccPath == nil {
		return fmt.Errorf("GCCpath required when building on windows. Set it to the path to your gcc executable")
	}
	cmd := exec.Command(
		*gccPath,
		append(
			cfiles, "-o",
			fmt.Sprintf("build/%s.exe", name),
			"-lopengl32",
			"-lgdi32",
		)...,
	)
	return runBuildCommand(cmd)
}

func getProjectCFiles() ([]string, error) {
	sourceFiles, err := getDirCFiles("src")
	if err != nil {
		return nil, err
	}
	gamaFiles, err := getDirCFiles("gama")
	if err != nil {
		return nil, err
	}
	return append(sourceFiles, gamaFiles...), nil
}

func BuildProject(wine bool) error {
	if config.Config == nil {
		return fmt.Errorf("config not found")
	}
	name := config.Config.Project.Name
	if name == "" {
		return fmt.Errorf("invalid project name")
	}
	cfiles, err := getProjectCFiles()
	if err != nil {
		return fmt.Errorf("error listing c source files in project: %s", err.Error())
	}
	switch runtime.GOOS {
	case "windows":
		return buildProjectWindows(name, cfiles)
	case "linux":
		if wine {
			return buildProjectWine(name, cfiles)
		} else {
			return buildProjectLinux(name, cfiles)
		}
	}
	return nil
}
