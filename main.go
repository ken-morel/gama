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
	"os"
	"path"

	"github.com/ken-morel/gama/cmd"
	"github.com/ken-morel/gama/gama"
)

func main() {
	installDir := os.Getenv("GAMA_DIR")
	if installDir == "" {
		homeDir, err := os.UserHomeDir()
		if err == nil {
			installDir = path.Join(homeDir, ".gama")
		} else {
			fmt.Println("Error getting system home directory  gama")
			os.Exit(1)
		}
	}
	_, err := os.Stat(installDir)
	if err != nil {
		fmt.Printf("Error: gama install folder(%s) not found, please reinstall gama: %s\n", installDir, err.Error())
		os.Exit(1)
	}
	gama.Init(&gama.GamaConfig{
		InstallPath: installDir,
	})
	cmd.Execute()
}
