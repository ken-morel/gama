package gama

import (
	"fmt"
	"os"
	"os/exec"
	"path"
	"runtime"
)

// winegcc src/main.c -o test.exe -lopengl32 -lgdi32

func buildWindows() error {
	if gccPath == nil {
		return fmt.Errorf("no gcc path specified")
	}
	name := config.Config.Project.Name
	if name == "" {
		return fmt.Errorf("invalid project name")
	}
	cmd.Stderr = os.Stderr
	cmd.Stdout = os.Stdout
	return cmd.Run()
}

// gcc ./test/src/main.c -o ./test/test -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread && ./test/test

func buildLinux() error {
	name := config.Config.Project.Name
	out := path.Join("build", name)
	if name == "" {
		return fmt.Errorf("invalid project name")
	}
	cmd := exec.Command("gcc", "src/main.c", "-o", out, "-g", "-lglfw", "-lGL", "-lm", "-lXrandr", "-lXi", "-lX11", "-lXxf86vm", "-lpthread")
	cmd.Stderr = os.Stderr
	cmd.Stdout = os.Stdout
	return cmd.Run()
}

func BuildProject(args *BuilldProjectArgs, log chan<- *Status) {
	var err error
	if config.Config == nil {
		log <- &Status{
			Message: "Configuration error",
			Error:   fmt.Errorf("project configuration not found"),
		}
		return
	}
	os.Mkdir("build", 0755)
	switch runtime.GOOS {
	case "windows":
		err = buildWindows()
	case "linux":
		err = buildLinux()
	default:
		log <- &Status{
			Message: " Error",
			Error:   fmt.Errorf("unsupported build platform"),
		}
		close(log)
		return
	}
	if err != nil {
		log <- &Status{
			Message: "Error while running build command",
			Error:   err,
		}
	} else {
		log <- &Status{
			Message: "Built executable",
			Error:   nil,
		}
	}
	if args.Run {
		RunBuild(&RunBuildArgs{Args: []string{}}, log)
	} else {
		close(log)
	}
}

func buildProjectWindows(name string) error {
	gccPath := config.Config.Build.GCC
	if gccPath == nil {
		return fmt.Errorf("GCCpath required when building on windows. Set it to the path to your gcc executable")
	}
	cmd := exec.Command(
		*gccPath,
		"src/main.c",
		"-o",
		fmt.Sprintf("build/%s.exe", name),
		"-lopengl32",
		"-lgdi32",
	)
	return runBuildCommand(cmd)
}

func BuildProject(wine bool) error {
	if config.Config == nil {
		return fmt.Errorf("Config not found")
	}
	name := config.Config.Project.Name
	if name == "" {
		return fmt.Errorf("Invalid project name")
	}
	switch runtime.GOOS {
	case "windows":
		return buildProjectWindows(name)
	case "linux":
		if wine {
			return buildProjectWine(name)
		} else {
			return buildProjectLinux(name)
		}
	}
}
