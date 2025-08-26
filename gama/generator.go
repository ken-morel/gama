package gama

import (
	"fmt"
	"os"
	"path"

	"github.com/plus3it/gorecurcopy"
)

func CopyGamaLibrary() error {
	projectGama := "gama"
	gamaLib := path.Join(config.InstallPath, "gama")
	os.RemoveAll(projectGama)
	os.Mkdir("gama", 0755)
	err := gorecurcopy.CopyDirectory(gamaLib, projectGama)
	if err != nil {
		return fmt.Errorf("error copying gama library: %s", err.Error())
	} else {
		return nil
	}
}

func CreateProject(name string, template string) error {
	if config == nil {
		return fmt.Errorf("error: gama not initialized")
	}
	templatePath := path.Join(config.InstallPath, "templates", template)
	gamaPath := path.Join(config.InstallPath, "gama")
	_, err := os.Stat(templatePath)
	if err != nil {
		return fmt.Errorf("tempate %s not found: %s", template, err.Error())
	}
	err = os.Mkdir(name, 0755)
	if err != nil {
		return fmt.Errorf("error creating project folder at %s: %s", name, err.Error())
	}
	err = gorecurcopy.CopyDirectory(templatePath, name)
	if err != nil {
		return fmt.Errorf("error copying template: %s", err.Error())
	}
	gamaDest := path.Join(name, "gama")
	err = os.Mkdir(gamaDest, 0755)
	if err == nil {
		err = gorecurcopy.CopyDirectory(gamaPath, gamaDest)
	}
	if err != nil {
		return fmt.Errorf("error copying gama: %s", err.Error())
	}

	for _, p := range [][]string{{"assets"}, {"assets", "fonts"}, {"assets", "sprites"}, {"assets", "images"}} {
		os.Mkdir(
			path.Join(
				append(
					[]string{name},
					p...,
				)...,
			), 0755)
	}
	gorecurcopy.Copy(path.Join(config.InstallPath, "images", "gama.ico"), path.Join(name, "assets", "images", "logo.ico"))

	conf := fmt.Sprintf(templateConfig, name)
	err = os.WriteFile(path.Join(name, "gama.yml"), []byte(conf), 0755)
	if err != nil {
		return fmt.Errorf("error writing gama config: %s", err.Error())
	}
	return nil
}
