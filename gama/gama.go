// Package gama: holds gamma actual functions
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
	os.Mkdir("gama", 0o755)
	err := gorecurcopy.CopyDirectory(gamaLib, projectGama)
	if err != nil {
		return fmt.Errorf("error copying gama library: %s", err.Error())
	} else {
		return nil
	}
}
