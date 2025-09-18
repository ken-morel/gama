package gama

import (
	"archive/zip"
	"fmt"
	"io"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"unicode"
)

func zipSource(source, target string) error {
	zipfile, err := os.Create(target)
	if err != nil {
		return fmt.Errorf("could not create zip file %s: %w", target, err)
	}
	defer zipfile.Close()

	archive := zip.NewWriter(zipfile)
	defer archive.Close()
	return filepath.Walk(source, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}
		header, err := zip.FileInfoHeader(info)
		if err != nil {
			return err
		}
		header.Name, err = filepath.Rel(source, path)
		if err != nil {
			return err
		}
		if info.IsDir() {
			header.Name += "/"
		} else {
			header.Method = zip.Deflate
		}

		writer, err := archive.CreateHeader(header)
		if err != nil {
			return err
		}

		if !info.IsDir() {
			file, err := os.Open(path)
			if err != nil {
				return err
			}
			defer file.Close()
			_, err = io.Copy(writer, file)
			return err
		}
		return err
	})
}

func isValidProjectName(s string) bool {
	if len(s) == 0 {
		return false
	}
	for _, r := range s {
		if !unicode.IsLetter(r) && !unicode.IsDigit(r) && !unicode.IsSpace(r) {
			return false
		}
	}
	return true
}

func openURL(url string) error {
	var cmd *exec.Cmd

	switch runtime.GOOS {
	case "windows":
		cmd = exec.Command("cmd", "/c", "start", url)
	case "darwin": // macOS
		cmd = exec.Command("open", url)
	default: // Linux, FreeBSD, OpenBSD, NetBSD, etc.
		// Check if running under WSL (Windows Subsystem for Linux)
		if isWSL() {
			cmd = exec.Command("cmd.exe", "/c", "start", url)
		} else {
			cmd = exec.Command("xdg-open", url)
		}
	}

	return cmd.Start()
}

// isWSL checks if the current environment is Windows Subsystem for Linux.
func isWSL() bool {
	// A simple way to check for WSL is to look for the /proc/version file
	// and see if it contains "Microsoft".
	// This might not be exhaustive for all WSL versions or future changes.
	_, err := exec.Command("grep", "-q", "Microsoft", "/proc/version").CombinedOutput()
	return err == nil
}
