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
package cmd

import (
	"github.com/ken-morel/gama/gama"
	"github.com/spf13/cobra"
)

// buildCmd represents the build command
var (
	shouldRun bool
	useWine   bool
	buildCmd  = &cobra.Command{
		Use:   "build",
		Short: "Build the applicaiton",
		Long:  `Build the application into an executable in build.`,
		Run: func(cmd *cobra.Command, args []string) {
			err := gama.BuildProject(useWine)
			if err == nil && shouldRun {
				err = gama.RunBuild(useWine)
			}
		},
	}
)

func init() {
	rootCmd.AddCommand(buildCmd)
	rootCmd.PersistentFlags().BoolVarP(&shouldRun, "run", "r", false, "Run the application after building it")
	rootCmd.PersistentFlags().BoolVarP(&shouldRun, "wine", "w", false, "Build the applicaiton using wine(linux only)")

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// buildCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// buildCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
