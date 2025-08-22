package gama

import (
	"embed"
	"fmt"
	"path"
)

//go:embed all:templates
var templates embed.FS

func GetTemplate(name string) ([]byte, error) {
	fmt.Println(templates.ReadDir("templates"))
	return templates.ReadFile(path.Join("templates", name+".c"))
}
