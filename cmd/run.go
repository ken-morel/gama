package cmd

import (
	"github.com/ken-morel/gama/gama"
	"github.com/spf13/cobra"
)

// runCmd represents the run command
var runCmd = &cobra.Command{
	Use:   "run",
	Short: "Run the built binaries",
	Long:  `Run built binaries in build/.`,
	Run: func(cmd *cobra.Command, args []string) {
		gama.RunBuild(args, useWine)
	},
}

func init() {
	rootCmd.AddCommand(runCmd)
	runCmd.PersistentFlags().BoolVarP(&useWine, "wine", "w", false, "Run windows build with wine")

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// runCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// runCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
