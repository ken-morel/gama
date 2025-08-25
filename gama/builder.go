package gama

import (
	"fmt"
	"os"
	"os/exec"
	"path"
	"runtime"
)

// winegcc src/main.c -o test.exe -lopengl32 -lgdi32
func buildProjectWine(name string, cfiles []string) error {
	outDir := path.Join("build", "windows")
	os.RemoveAll(outDir)
	os.MkdirAll(outDir, 0755)
	cmd := exec.Command(
		"winegcc",
		append(
			cfiles,
			"-o",
			path.Join(outDir, name+".exe"),
			"-lopengl32",
			"-lgdi32",
			"-D BACKEND_WIN32=\"\"",
		)...,
	)
	fmt.Println("Running:", cmd.String())
	return runBuildCommand(cmd)
}

// gcc ./test/src/main.c -o ./test/test -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread && ./test/test
func buildProjectLinux(name string, cfiles []string) error {
	outDir := path.Join("build", "linux")
	os.RemoveAll(outDir)
	os.MkdirAll(outDir, 0755)
	cmd := exec.Command(
		"gcc",
		append(
			cfiles,
			"-o",
			path.Join(outDir, name),
			"-g",
			"-lglfw",
			"-lGL",
			"-lm",
			"-lXrandr",
			"-lXi",
			"-lX11",
			"-lXxf86vm",
			"-lpthread",
			"-D BACKEND_GLFW=\"\"",
		)...,
	)
	fmt.Println("Running:", cmd.String())
	return runBuildCommand(cmd)
}

// gcc src/main.c -o test.exe -lopengl32 -lgdi32
func buildProjectWindows(name string, cfiles []string) error {
	gccPath := config.Config.Build.GCC
	if gccPath == nil {
		return fmt.Errorf("GCCpath required when building on windows. Set it to the path to your gcc executable")
	}
	outDir := path.Join("build", "windows")
	os.RemoveAll(outDir)
	os.MkdirAll(outDir, 0755)

	cmd := exec.Command(
		*gccPath,
		append(
			cfiles,
			"-o",
			path.Join(outDir, name+".exe"),
			"-lopengl32",
			"-lgdi32",
			"-D BACKEND_WIN32=\"\"",
		)...,
	)
	fmt.Println("Running:", cmd.String())
	return runBuildCommand(cmd)
}

func getProjectCFiles() ([]string, error) {
	sourceFiles, err := getDirCFiles("src")
	if err != nil {
		return nil, err
	}
	gamaFiles, err := getDirCFiles("gama")
	if err != nil {
		fmt.Printf("Warning: could not list files in gama/: %s\n", err.Error())
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
	if len(cfiles) == 0 {
		return fmt.Errorf("no c source file found file found")
	}
	fmt.Println("Building files", cfiles)

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
