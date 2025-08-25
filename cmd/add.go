package cmd

import (
	"fmt"

	"github.com/ken-morel/gama/gama"
	"github.com/spf13/cobra"
)

// addCmd represents the add command
var addCmd = &cobra.Command{
	Use:   "add <name>",
	Short: "Add specified font to project",
	Long:  `Add specified font to project.`,
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		name := args[0]
		err := gama.AddFont(name, fontsUseDirectory)
		if err != nil {
			fmt.Println(err.Error())
		}
	},
}

func init() {
	fontCmd.AddCommand(addCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// addCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// addCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
