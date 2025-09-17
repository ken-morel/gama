package gama

import "bytes"

func SubstituteTemplate(content []byte) []byte {
	if config == nil || config.Config == nil {
		return content
	}
	subst := map[string]string{
		"{{project.version}}": config.Config.Project.Version,
		"{{project.name}}":    config.Config.Project.Name,
		"{{author.name}}":     config.Config.Project.Author.Name,
		"{{author.email}}":    config.Config.Project.Author.Email,
		"{{gama.location}}":   config.Config.Gama.Location,
		"{{gama.install}}":    config.InstallPath,
	}
	for key, value := range subst {
		content = bytes.ReplaceAll(content, []byte(key), []byte(value))
	}
	return content
}
