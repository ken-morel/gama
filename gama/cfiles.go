package gama

import (
	"os"
	"path"
)

func getDirCFiles(folder string) ([]string, error) {
	var cfiles []string
	content, err := os.ReadDir(folder)
	if err != nil {
		return nil, err
	}
	for _, entry := range content {
		if entry.IsDir() {
			children, err := getDirCFiles(path.Join(folder, entry.Name()))
			if err != nil {
				return nil, err
			}
			cfiles = append(cfiles, children...)
		} else {
			if path.Ext(entry.Name()) == ".c" {
				cfiles = append(cfiles, path.Join(folder, entry.Name()))
			}
		}
	}
	return cfiles, nil
}
