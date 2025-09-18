package cmd

import (
	"fmt"

	"github.com/ken-morel/gama/gama"
	"github.com/spf13/cobra"
)

// nsisCmd represents the nsis command
var nsisCmd = &cobra.Command{
	Use:   "nsis",
	Short: "Package the app in nsis installer",
	Long:  `Package the app using nsis installer, uses nsis on windows and makensis on linux`,
	Run: func(cmd *cobra.Command, args []string) {
		if err := gama.PackageNSIS(); err != nil {
			fmt.Println("Error packaging executable: ", err.Error())
		}
	},
}

func init() {
	packageCmd.AddCommand(nsisCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// nsisCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// nsisCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
