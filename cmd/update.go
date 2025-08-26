package cmd

// Copyright © 2025 Engon Ken Morel
import (
	"fmt"

	"github.com/ken-morel/gama/gama"
	"github.com/spf13/cobra"
)

// updateCmd represents the update command
var updateCmd = &cobra.Command{
	Use:   "update",
	Short: "Update the project's gama headers with the toolchain's",
	Long: `This deletes the project's gama folder and recopies that
	which comes with the gama toolchain.`,
	Run: func(cmd *cobra.Command, args []string) {
		err := gama.CopyGamaLibrary()
		if err != nil {
			fmt.Println(err.Error())
		}
	},
}

func init() {
	rootCmd.AddCommand(updateCmd)
}
