package cmd

import (
	"fmt"

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
			fmt.Println("Running project with args:", args)
			err := gama.BuildProject(useWine)
			if err != nil {
				fmt.Printf("error building project: %s\n", err.Error())
			} else if shouldRun {
				err = gama.RunBuild(args, useWine)
				if err != nil {
					fmt.Printf("Error running project build: %s", err.Error())
				}
			}
		},
	}
)

func init() {
	rootCmd.AddCommand(buildCmd)
	buildCmd.PersistentFlags().BoolVarP(&shouldRun, "run", "r", false, "Run the application after building it")
	buildCmd.PersistentFlags().BoolVarP(&useWine, "wine", "w", false, "Build the applicaiton using wine(linux only)")

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// buildCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// buildCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
