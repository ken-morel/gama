package gama

type AuthorConfig struct {
	Name  string `yaml:"name"`
	Email string `yaml:"email"`
}
type ProjectProjectConfig struct {
	Name    string       `yaml:"name"`
	Version string       `yaml:"version"`
	Author  AuthorConfig `yaml:"author"`
}
type ProjectGamaConfig struct {
	Location string `yaml:"location"`
}
type ProjectBuildConfig struct {
	GCC *string `yaml:"gcc"`
}

type ProjectConfig struct {
	Project ProjectProjectConfig `yaml:"project"`
	Gama    ProjectGamaConfig    `yaml:"gama"`
	Build   ProjectBuildConfig   `yaml:"build"`
}

type GamaConfig struct {
	InstallPath string
	Config      *ProjectConfig
}

var config *GamaConfig

func Init(conf *GamaConfig) {
	config = conf
}
