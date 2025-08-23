package gama

import (
	"fmt"
	"os"
	"os/exec"
	"path"
	"runtime"
)

type RunBuildArgs struct {
	Args []string
}

func RunBuild(args *RunBuildArgs, log chan<- *Status) {
	var command *exec.Cmd
	if config.Config == nil {
		fmt.Println("Config not found")
		log <- &Status{
			Message: "Error starting",
			Error:   fmt.Errorf("configuration not found"),
		}
		return
	}
	name := config.Config.Project.Name
	if name == "" {
		log <- &Status{
			Message: "Configuration error",
			Error:   fmt.Errorf("empty project name"),
		}
	}
	switch runtime.GOOS {
	case "windows":
		command = exec.Command(path.Join("build", name+".exe"))
	case "linux":
		command = exec.Command(path.Join("build", name))
	default:
		log <- &Status{
			Message: "Error finding build command",
			Error:   fmt.Errorf("unsupported system for building"),
		}
	}
	log <- &Status{
		Message: "Running build",
		Error:   nil,
	}
	command.Stderr = os.Stderr
	command.Stdin = os.Stdin
	command.Stdout = os.Stdout
	err := command.Run()
	if err != nil {
		log <- &Status{
			Message: "Error running program",
			Error:   err,
		}
	} else {
		log <- &Status{
			Message: "Ran program succesfully",
			Error:   nil,
		}
	}
	close(log)
}
