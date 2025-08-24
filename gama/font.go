package gama

import (
	"fmt"
	"os"
	"path"

	"github.com/plus3it/gorecurcopy"
)

func AddFont(name string, fontsDir string) error {
	if fontsDir == "" {
		fontsDir = path.Join(config.InstallPath, "fonts")
	}

	fileName := path.Join(fontsDir, name)
	_, err := os.Stat(fileName)
	if err != nil {
		return fmt.Errorf("font not found: %s", err.Error())
	}
	os.MkdirAll(path.Join("assets", "fonts"), 0755)
	err = gorecurcopy.Copy(fileName, path.Join("assets", "fonts", name))
	if err != nil {
		return fmt.Errorf("error copying font: %s", err.Error())
	} else {
		return nil
	}
}

func ListFonts(fontsDir string) error {
	if fontsDir == "" {
		fontsDir = path.Join(config.InstallPath, "fonts")
	}
	entries, err := os.ReadDir(fontsDir)
	if err != nil {
		return fmt.Errorf("error searching fonts in %s: %s", fontsDir, err.Error())
	}
	fmt.Println("Listing fonts at:", fontsDir)
	for _, entry := range entries {
		fmt.Printf(" - %s\n", entry.Name())
	}
	return nil
}
