package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

// fontCmd represents the font command
var (
	fontsUseDirectory string
	fontCmd           = &cobra.Command{
		Use:   "font <command>",
		Short: "Manage project fonts",
		Long: `Manage project fonts in ./assets/fonts/. from gama builtin toolchain fonts.
add fonts using 
	gama font add <name>
list available fonts using
	gama font list
`,
		Run: func(cmd *cobra.Command, args []string) {
			fmt.Println("Specify a command please. Use `gama font --help` for usage")
		},
	}
)

func init() {
	rootCmd.AddCommand(fontCmd)
	fontCmd.PersistentFlags().StringVarP(&fontsUseDirectory, "dir", "d", "", "Add  and list fonts from specified directory instead.")

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// fontCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// fontCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
