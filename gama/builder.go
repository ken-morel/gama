package gama

import (
	"fmt"
	"os"
	"os/exec"
	"path"
	"runtime"
	"strings"
)

func getBuildExecutablePath(name string, useWine bool) (string, error) {
	lin := path.Join("build", "linux", name)
	win := path.Join("build", "windows", name+".exe")
	if useWine || runtime.GOOS == "windows" {
		return win, nil
	} else if runtime.GOOS == "linux" {
		return lin, nil
	}
	return "", fmt.Errorf("system not supported")
}

// winegcc src/main.c -o test.exe -lopengl32 -lgdi32
func buildProjectWine(name string, cfiles []string, gama string) error {
	outDir := path.Join("build", "windows")
	os.RemoveAll(outDir)
	os.MkdirAll(outDir, 0o755)
	cmd := exec.Command(
		"winegcc",
		append(
			cfiles,
			"-o",
			path.Join(outDir, name+".exe"),
			"-lopengl32",
			"-lgdi32",
			"-D BACKEND_WIN32=\"\"",
			fmt.Sprintf("-I%s", gama),
		)...,
	)
	fmt.Println("Running:", cmd.String())
	return runBuildCommand(cmd)
}

// gcc ./test/src/main.c -o ./test/test -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread && ./test/test
func buildProjectLinux(name string, cfiles []string, gama string) error {
	outDir := path.Join("build", "linux")
	os.RemoveAll(outDir)
	os.MkdirAll(outDir, 0o755)
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
			fmt.Sprintf("-I%s", gama),
		)...,
	)
	fmt.Println("Running:", cmd.String())
	return runBuildCommand(cmd)
}

// gcc src/main.c -o test.exe -lopengl32 -lgdi32
func buildProjectWindows(name string, cfiles []string, gama string) error {
	gccPath := config.Config.Build.GCC
	if gccPath == nil {
		return fmt.Errorf("GCCpath required when building on windows. Set it to the path to your gcc executable")
	}
	outDir := path.Join("build", "windows")
	os.RemoveAll(outDir)
	os.MkdirAll(outDir, 0o755)

	cmd := exec.Command(
		*gccPath,
		append(
			cfiles,
			"-o",
			path.Join(outDir, name+".exe"),
			"-lopengl32",
			"-lgdi32",
			"-D BACKEND_WIN32=\"\"",
			fmt.Sprintf("-I%s", gama),
		)...,
	)
	fmt.Println("Running:", cmd.String())
	return runBuildCommand(cmd)
}

func getGamaLocation() (string, error) {
	if config == nil {
		return "", fmt.Errorf("no project configuration found")
	}
	cwd, err := os.Getwd()
	if err != nil {
		return "", err
	}
	return strings.Replace(config.Config.Gama.Location, "$project", cwd, 1), nil
}

func getProjectCFiles() ([]string, error) {
	sourceFiles, err := getDirCFiles("src")
	var gamaFiles []string
	if err != nil {
		return nil, err
	}
	gama, err := getGamaLocation()

	if err == nil {
		gamaFiles, err = getDirCFiles(gama)
	}
	if err != nil {
		fmt.Printf("Warning: could not list files in gama/: %s\n", err.Error())
	}
	return append(sourceFiles, gamaFiles...), nil
}

func buildProjectEmscripten(name string, cfiles []string, gama string) error {
	outDir := path.Join("build", "emscripten")
	os.RemoveAll(outDir)
	os.MkdirAll(outDir, 0o755)
	cmd := exec.Command(
		"emcc",
		append(
			cfiles,
			"-o",
			path.Join(outDir, name+".html"),
			"-s",
			"USE_GLFW=3",
			"-s",
			"USE_WEBGL2=1",
			"-D",
			"BACKEND_EMSCRIPTEN",
			fmt.Sprintf("-I%s", gama),
		)...,
	)
	fmt.Println("Running:", cmd.String())
	return runBuildCommand(cmd)
}

func BuildProject(wine bool, emscripten bool) error {
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
	gama, _ := getGamaLocation()

	if emscripten {
		return buildProjectEmscripten(name, cfiles, gama)
	}

	switch runtime.GOOS {
	case "windows":
		return buildProjectWindows(name, cfiles, gama)
	case "linux":
		if wine {
			return buildProjectWine(name, cfiles, gama)
		} else {
			return buildProjectLinux(name, cfiles, gama)
		}
	}
	return nil
}
