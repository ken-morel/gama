package gama

import (
	"fmt"
	"os"
	"path"

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
