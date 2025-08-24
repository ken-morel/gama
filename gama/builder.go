package gama

import (
	"fmt"
	"os"
	"os/exec"
	"path"
	"runtime"
)

type BuilldProjectArgs struct {
	Run bool
}

// winegcc src/main.c -o test.exe -lopengl32 -lgdi32

func buildWindows() error {
	gccPath := config.Config.Build.GCC
	if gccPath == nil {
		return fmt.Errorf("no gcc path specified")
	}
	name := config.Config.Project.Name
	if name == "" {
		return fmt.Errorf("invalid project name")
	}
	cmd := exec.Command(*gccPath, "src/main.c", "-o", fmt.Sprintf("build/%s.exe", name), "-lopengl32", "-lgdi32")
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
