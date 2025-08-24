package gama

import (
	"fmt"
	"os"
	"os/exec"
	"path"
	"runtime"
)

func runBuildWindows(name string) error {
	command := exec.Command(path.Join("build", name+".exe"))
	return runBuildCommand(command)
}

func runBuildLinux(name string) error {
	command := exec.Command(path.Join("build", name))
	return runBuildCommand(command)
}

func runBuildWine(name string) error {
	command := exec.Command("wine", path.Join("build", name+".exe"))
	return runBuildCommand(command)
}

func runBuildCommand(command *exec.Cmd) error {
	command.Stderr = os.Stderr
	command.Stdin = os.Stdin
	command.Stdout = os.Stdout
	return command.Run()
}

func RunBuild(wine bool) error {
	if config.Config == nil {
		return fmt.Errorf("configuration not found")
	}
	name := config.Config.Project.Name
	if name == "" {
		return fmt.Errorf("empty project name")
	}
	switch runtime.GOOS {
	case "windows":
		return runBuildWindows(config.Config.Project.Name)
	case "linux":
		if wine {
			runBuildWine(name)
		} else {
			runBuildLinux(name)
		}
	default:
		return fmt.Errorf("unsupported system for building")
	}
	return nil
}
