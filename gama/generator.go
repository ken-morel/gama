package gama

import (
	"fmt"
	"os"
	"path"
	"strings"

	"github.com/AlecAivazis/survey/v2"
	"github.com/plus3it/gorecurcopy"
	"gopkg.in/yaml.v3"
)

func CreateProjectInteractive() error {
	if config == nil {
		return fmt.Errorf("gama configuration not found")
	}
	fmt.Println("Let's make this real!")
	hostName, _ := os.Hostname()
	templates, err := GetTemplates()
	if err != nil {
		return fmt.Errorf("could not load templates: %s", err.Error())
	}
	templateNames := make([]string, len(templates))
	for i, t := range templates {
		templateNames[i] = t.Name
	}

	responses := struct {
		ProjectName     string   `survey:"projectName"`
		ProjectTemplate string   `survey:"projectTemplate"`
		AuthorName      string   `survey:"authorName"`
		AuthorEmail     string   `survey:"authorEmail"`
		Editors         []string `survey:"editors"`
	}{}
	questions := []*survey.Question{
		{
			Name:   "projectName",
			Prompt: &survey.Input{Message: "What name do you want to give to your project? Use just letters, numbers and spaces:"},
			Validate: func(val any) error {
				if !isValidProjectName(val.(string)) {
					return fmt.Errorf("invalid project name:  should contain only letters, numbers or spaces")
				}
				return nil
			},
		},
		{
			Name: "projectTemplate",
			Prompt: &survey.Select{
				Message: "What project template do you use?",
				Options: templateNames,
				Default: "skeleton",
				Help:    "The template is the base code from which you are going to start your project.",
				Description: func(value string, index int) string {
					if index < len(templates) {
						return templates[index].Description
					}
					return "No description found"
				},
			},
			Validate: survey.Required,
		},
		{
			Name: "authorName",
			Prompt: &survey.Input{
				Message: "How should be named the author of the project?",
				Default: hostName,
				Help:    "Use just letters, numbers and spaces. Try to make it shord and simple.",
			},
			Validate: func(val any) error {
				if !isValidProjectName(val.(string)) {
					return fmt.Errorf("invalid project name:  should contain only letters, numbers or spaces")
				}
				return nil
			},
		},
		{
			Name: "authorEmail",
			Prompt: &survey.Input{
				Message: "Enter the project author's email. like example@gmail.com or test@example.something:",
				Suggest: func(entry string) []string {
					if strings.HasSuffix(entry, "@") {
						return []string{"gmail.com", "engon.cm"}
					}
					return nil
				},
				Help: "Enter your google mail address or any similar email so users can contact you.",
			},
			Transform: func(val any) any {
				return strings.ReplaceAll(survey.ToLower(val).(string), " ", "")
			},
			Validate: func(ans any) error {
				val := ans.(string)
				if strings.Contains(val, " ") {
					return fmt.Errorf("email address should not contain spaces")
				}
				if strings.Contains(val, "@") {
					tail := val[strings.Index(val, "@"):]
					if !strings.Contains(tail, ".") {
						return fmt.Errorf("email address domain should have a dot(like gmail.com)")
					}
					return nil
				} else {
					return fmt.Errorf("email address contain @")
				}
			},
		},
		{
			Name: "editors",
			Prompt: &survey.MultiSelect{
				Message: "What editor configurations do you want to add?",
				Options: []string{"Code::Blocks", "Code", "Sublime Text", "Neovim"},
				Default: nil,
				Help:    "Choose what you are going to use to code",
				Description: func(value string, index int) string {
					switch value {
					case "Code::Blocks":
						return "Code blocks, a popular ide for GCE A/L students, recommended for beginers in C."
					case "Code":
						return "VS Code actually. A popular ide due to it's autocompletion, design and ease of use."
					case "Sublime Text":
						return "Sublime's just Sublime, I think the most beautiful editor amongst these with snippets, python plugins, and more."
					case "Neovim":
						return "Neovim(used to write gama), the modal text editor, it may not be easy to use, but is fast and nice."
					default:
						return "No description"
					}
				},
			},
		},
	}
	err = survey.Ask(questions, &responses)
	if err != nil {
		return err
	}
	os.Mkdir(responses.ProjectName, 0o755)
	os.Chdir(responses.ProjectName)
	for _, template := range templates {
		if template.Name == responses.ProjectTemplate {
			err := template.Copy()
			if err != nil {
				return err
			} else {
				break
			}
		}
	}
	os.MkdirAll(path.Join("assets", "images"), 0o755)
	os.MkdirAll(path.Join("assets", "sprites"), 0o755)
	os.MkdirAll(path.Join("assets", "fonts"), 0o755)
	if err := gorecurcopy.Copy(path.Join(config.InstallPath, "images", "project-icon.png"), "assets/images/logo.png"); err != nil {
		fmt.Println("Error copying icon: ", err.Error())
	}
	if err := gorecurcopy.Copy(path.Join(config.InstallPath, "conf-templates", "license"), "LICENSE"); err != nil {
		fmt.Println("Error copying license:", err.Error())
	} else {
		content, err := os.ReadFile("LICENSE")
		if err == nil {
			content := SubstituteTemplate(content)
			os.WriteFile("LICENSE", content, 0o755)
		}
	}
	if err := CopyGamaLibrary(); err != nil {
		return err
	}
	configFile, err := os.Create("gama.yml")
	if err != nil {
		return fmt.Errorf("error creating gama configuration: %s", err.Error())
	}
	defer configFile.Close()
	yaml.NewEncoder(configFile).Encode(ProjectConfig{
		Project: ProjectProjectConfig{
			Name:    responses.ProjectName,
			Version: "0.1.0",
			Author: AuthorConfig{
				Name:  responses.AuthorName,
				Email: responses.AuthorEmail,
			},
		},
		Gama: ProjectGamaConfig{
			Location: "$project/gama",
		},
		Build: ProjectBuildConfig{
			GCC: nil,
		},
	})
	return nil
}
