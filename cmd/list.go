package cmd

import (
	"fmt"

	"github.com/ken-morel/gama/gama"
	"github.com/spf13/cobra"
)

// listCmd represents the list command
var listCmd = &cobra.Command{
	Use:   "list",
	Short: "List available fonts in toolchain",
	Long:  `List available fonts.`,
	Run: func(cmd *cobra.Command, args []string) {
		err := gama.ListFonts(fontsUseDirectory)
		if err != nil {
			fmt.Println(err.Error())
		}
	},
}

func init() {
	fontCmd.AddCommand(listCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// listCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// listCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
