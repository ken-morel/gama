package gama

import (
	"fmt"
	"os"
	"os/exec"
	"path"
	"runtime"

	"github.com/plus3it/gorecurcopy"
)

func PackageZip(useWine bool) error {
	if config.Config == nil {
		return fmt.Errorf("configuration file not found")
	}
	name := config.Config.Project.Name
	version := config.Config.Project.Version
	tempDir, err := os.MkdirTemp("", "gama-zip-package-*")
	if err != nil {
		return fmt.Errorf("error creating temporary folder to package zip %s", err.Error())
	}
	defer os.RemoveAll(tempDir)
	exePath, err := getBuildExecutablePath(name, useWine)
	if err != nil {
		return err
	}
	buildFolder := path.Dir(exePath)
	err = gorecurcopy.CopyDirectory(buildFolder, tempDir)
	if err != nil {
		return fmt.Errorf("build executable not found. Make sure you built the project first: %s", err.Error())
	}
	err = gorecurcopy.CopyDirectory("assets", path.Join(tempDir, "assets"))
	if err != nil {
		return fmt.Errorf("error copying assets: %s", err.Error())
	}
	return zipSource(tempDir, path.Join(buildFolder, name+"-"+version+".zip"))
}

func PackageNSIS() error {
	if config.Config == nil {
		return fmt.Errorf("no project configuration found")
	}
	if err := gorecurcopy.Copy(path.Join(config.InstallPath, "conf-templates", "installer.nsi"), "installer.nsi"); err != nil {
		return fmt.Errorf("error copying nsi template: %s", err.Error())
	}
	content, err := os.ReadFile("installer.nsi")
	if err != nil {
		return err
	}
	if err := os.WriteFile(".temp-builder.nsi", SubstituteTemplate(content), 0o755); err != nil {
		return err
	}
	switch runtime.GOOS {
	case "windows":
		return buildNSIS()
	case "linux", "darwin":
		return buildMakeNSIS()
	default:
		fmt.Println("Unsupported build platform. Trying makensis command")
		return buildMakeNSIS()
	}
}

func buildNSIS() error {
	return _buildNSIS("nsis")
}

func buildMakeNSIS() error {
	return _buildNSIS("nsis")
}

func _buildNSIS(exe string) error {
	cmd := exec.Command(exe, ".temp-builder.nsi")
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Stdout = os.Stdout
	return cmd.Run()
}
