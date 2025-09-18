package gama

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"path"
	"runtime"
	"time"
)

func runBuildWindows(name string, args []string) error {
	command := exec.Command(path.Join("build", "windows", name+".exe"), args...)
	return runBuildCommand(command)
}

func runBuildLinux(name string, args []string) error {
	command := exec.Command(path.Join("build", "linux", name), args...)
	return runBuildCommand(command)
}

func runBuildEmscripten(name string) error {
	fileServer := http.FileServer(http.Dir(path.Join("build", "emscripten")))
	http.Handle("/", fileServer)
	port := 5173
	go func() {
		time.Sleep(time.Second)
		openURL(fmt.Sprintf("http://localhost:%d/%s.html", port, name))
	}()
	for range 10 {
		log.Println("Starting file server on :", port)
		err := http.ListenAndServe(fmt.Sprintf(":%d", port), nil)
		if err != nil {
			fmt.Println("Failed, restarting server")
			port++
		} else {
			return nil
		}
	}
	return fmt.Errorf("failed to start server")
}

func runBuildWine(name string, args []string) error {
	bargs := []string{path.Join("build", "windows", name+".exe")}
	bargs = append(bargs, args...)
	command := exec.Command("wine", bargs...)
	return runBuildCommand(command)
}

func runBuildCommand(command *exec.Cmd) error {
	command.Stderr = os.Stderr
	command.Stdin = os.Stdin
	command.Stdout = os.Stdout
	return command.Run()
}

func RunBuild(args []string, wine bool, emscripten bool) error {
	if config.Config == nil {
		return fmt.Errorf("configuration not found")
	}
	name := config.Config.Project.Name
	if name == "" {
		return fmt.Errorf("empty project name")
	}
	if emscripten {
		return runBuildEmscripten(name)
	}
	switch runtime.GOOS {
	case "windows":
		return runBuildWindows(config.Config.Project.Name, args)
	case "linux":
		if wine {
			runBuildWine(name, args)
		} else {
			runBuildLinux(name, args)
		}
	default:
		return fmt.Errorf("unsupported system for building")
	}
	return nil
}
