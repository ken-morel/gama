/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"
	"os"

	"github.com/ken-morel/gama/gama"
	"github.com/spf13/cobra"
)

var (
	projectName  string
	templateName string
	createCmd    = &cobra.Command{
		Use:   "create <name>",
		Short: "Create a new gama project",
		Long: `Create is a command to create or start a new utility.

	Use:
	gama create <name>

	It creates a new directory called <name> and initializes the
	project in it.
	`,
		Run: func(cmd *cobra.Command, args []string) {
			if len(projectName) == 0 && len(args) > 0 {
				projectName = args[0]
			}
			if projectName == "" {
				fmt.Println("No name specified")
				os.Exit(1)
			}
			fmt.Printf("Creating project at ./%s/\n", projectName)
			logs := make(chan *gama.Status)
			go gama.CreateProject(&gama.CreateProjectArgs{
				Name:     projectName,
				Template: templateName,
			}, logs)
			for log := range logs {
				fmt.Println(log.Message)
				if log.Error != nil {
					fmt.Println(log.Error.Error())
				}
			}
		},
		Aliases: []string{"init", "initialize"},
		Example: "gama create helloWorld",
	}
)

func init() {
	rootCmd.AddCommand(createCmd)

	createCmd.Flags().StringVarP(&projectName, "name", "n", "", "The name of the project you want to create")
	createCmd.Flags().StringVarP(&templateName, "template", "t", "skeleton", "The starter template")

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// createCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// createCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
