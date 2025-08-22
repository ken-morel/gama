// Package gama: holds gamma actual functions
package gama

type Status struct {
	Message string
	Error   error
}

type GamaConfig struct {
	InstallPath string
}

var config *GamaConfig

func Init(conf *GamaConfig) {
	config = conf
}
