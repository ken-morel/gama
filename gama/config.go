package gama

const templateConfig = `
project:
  name: %s
  version: 0.1.0
gama:
  version: 0.1.0
build:
  gcc: null
`

type ProjectProjectConfig struct {
	Name    string `yaml:"name"`
	Version string `yaml:"version"`
}
type ProjectGamaConfig struct {
	Version string `yaml:"version"`
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
