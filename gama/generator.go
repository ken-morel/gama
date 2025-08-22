package gama

import (
	"fmt"
	"os"
	"path"
)

type CreateProjectArgs struct {
	Name     string
	Template string
}

func CreateProject(args *CreateProjectArgs, log chan<- *Status) {
	if config == nil {
		fmt.Println("Error: gama not initialized")
		os.Exit(2)
	}
	steps := []func(*CreateProjectArgs, chan<- *Status) bool{
		createProjectDir,
		createProjectSource,
		createConfig,
	}
	for _, step := range steps {
		if !step(args, log) {
			break
		}
	}
	close(log)
}

func createProjectDir(args *CreateProjectArgs, log chan<- *Status) bool {
	err := os.Mkdir(args.Name, 0755)
	if err != nil {
		log <- &Status{
			Message: "Error Creating project folder",
			Error:   fmt.Errorf("failed creating folder at %s: %s ", args.Name, err.Error()),
		}
		return false
	}
	log <- &Status{"Created project folder", nil}
	return true
}

func createProjectSource(args *CreateProjectArgs, log chan<- *Status) bool {
	err := os.Mkdir(path.Join(args.Name, "src"), 0755)
	if err != nil {
		log <- &Status{
			Message: "Error Creating project source folder",
			Error:   fmt.Errorf("failed creating folder at %s: %s ", "src", err.Error()),
		}
		return false
	}
	log <- &Status{"Created source folder folder", nil}
	err = createTemplateFiles(path.Join(args.Name, "src"), args.Template)
	if err != nil {
		log <- &Status{
			Message: "Error creating template files",
			Error:   err,
		}
	} else {
		log <- &Status{
			Message: "Created template files",
			Error:   nil,
		}
	}
	return true
}

const templateConfig = `
%%YAML 1.2
---
project:
  name: %s
  template: %s
`

func createConfig(args *CreateProjectArgs, log chan<- *Status) bool {
	conf := fmt.Sprintf(templateConfig, args.Name, args.Template)
	os.WriteFile(path.Join(args.Name, "gama.yml"), []byte(conf), 0775)
	return true
}

func createTemplateFiles(sourceDir string, templ string) error {
	templateDir := path.Join(config.InstallPath, "templates", templ)
	entries, err := os.ReadDir(templateDir)
	if err != nil {
		return fmt.Errorf("template does not exist: %s", err.Error())
	}
	for _, entry := range entries {
		if entry.IsDir() {
			fmt.Println("Found a directory in template, this is unexpected but not error prone.")
		} else {
			file := entry.Name()
			content, err := os.ReadFile(path.Join(templateDir, file))
			if err != nil {
				fmt.Printf("Could not read file %s from template. This should not happen", file)
			} else {
				err := os.WriteFile(path.Join(sourceDir, file), content, 0755)
				if err != nil {
					fmt.Println("Could not put file from template to destination path")
					return err
				}
			}
		}
	}
	return err
}
