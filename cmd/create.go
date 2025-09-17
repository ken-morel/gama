package cmd

import (
	"fmt"

	"github.com/ken-morel/gama/gama"
	"github.com/spf13/cobra"
)

var createCmd = &cobra.Command{
	Use:   "create",
	Short: "Create a new gama project interactively",
	Long: `Create is a command to create or start a new utility.

	Use:
	gama create

	It creates a new project interactively.
	`,
	Run: func(cmd *cobra.Command, args []string) {
		err := gama.CreateProjectInteractive()
		if err != nil {
			fmt.Println("Error creating project: ", err.Error())
		}
	},
	Aliases: []string{"init", "initialize"},
	Example: "gama create helloWorld",
}

func init() {
	rootCmd.AddCommand(createCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// createCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// createCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
