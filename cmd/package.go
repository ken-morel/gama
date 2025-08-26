package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

var packageCmd = &cobra.Command{
	Use:   "package [mode]",
	Short: "Package the application with specified mode",
	Long:  `Package the application with  specified mode.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("package called")
	},
}

func init() {
	rootCmd.AddCommand(packageCmd)
}
