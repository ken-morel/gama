package gama

import (
	"bytes"
	"os"
	"path"
)

type ProjectTemplate struct {
	Name        string
	Path        string
	Description string
}

func GetTemplates() (templates []ProjectTemplate, err error) {
	templatesFolder := path.Join(config.InstallPath, "templates")
	content, err := os.ReadDir(templatesFolder)
	for _, entry := range content {
		templatePath := path.Join(templatesFolder, entry.Name())
		if entry.IsDir() {
			description := "Could not load description"
			content, err := os.ReadFile(path.Join(templatePath, "README.md"))
			if err == nil {
				parts := bytes.Split(content, []byte("\n"))

				if len(parts) >= 3 {
					description = string(parts[2])
				}
			} else {
				description = "Error: " + err.Error()
			}
			templates = append(templates, ProjectTemplate{
				Name:        entry.Name(),
				Path:        templatePath,
				Description: description,
			})
		}
	}
	return templates, err
}
