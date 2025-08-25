/*
Copyright © 2025 Engon Ken Morel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
package main

import (
	"fmt"
	"log"
	"os"
	"path"

	"github.com/ken-morel/gama/cmd"
	"github.com/ken-morel/gama/gama"
	"gopkg.in/yaml.v3"
)

func getInstallDir() (string, error) {
	installDir := os.Getenv("GAMA_DIR")
	if installDir == "" {
		_, err := os.Stat("/usr/share/gama")
		if err == nil {
			return "/usr/share/gama", nil
		}
		homeDir, err := os.UserHomeDir()
		if err == nil {
			installDir = path.Join(homeDir, ".gama")
		} else {
			fmt.Println("Error getting system home directory  gama")
		}
	}
	_, err := os.Stat(installDir)
	if err != nil {
		return "", fmt.Errorf("error: gama install folder(%s) not found, please reinstall gama: %s", installDir, err.Error())
	}
	return installDir, nil
}

func getConfig() (*gama.ProjectConfig, error) {
	var config gama.ProjectConfig
	file, err := os.Open("gama.yml")
	if err != nil {
		return nil, err
	}
	defer file.Close()
	err = yaml.NewDecoder(file).Decode(&config)
	return &config, err
}

func main() {
	installDir, err := getInstallDir()
	if err != nil {
		log.Fatal(err)
	}
	conf, err := getConfig()
	if err != nil {
		log.Println(err.Error())
	}
	gama.Init(&gama.GamaConfig{
		InstallPath: installDir,
		Config:      conf,
	})
	cmd.Execute()
}
