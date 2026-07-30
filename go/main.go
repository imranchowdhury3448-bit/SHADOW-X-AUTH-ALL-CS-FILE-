// ========================================================================
// SHADOW X AUTH - GO WORKING SAMPLE PROJECT
// ========================================================================

package main

import (
	"fmt"
	"os"
)

func main() {
	fmt.Println("====================================================")
	fmt.Println("        SHADOW X AUTH - GO SAMPLE PROJECT          ")
	fmt.Println("====================================================\n")

	// Initialize SDK
	auth := NewShadowXAuth("MY_APP", "25D2EFE1BE", "39f8e1a629524947a1767597cd3159967742aed0116a49529e508bba70ad081c", "1.0")

	fmt.Print("Enter License Key: ")
	var key string
	fmt.Scanln(&key)

	res, err := auth.ValidateKey(key)
	if err != nil || !res.Success {
		fmt.Printf("\n[-] Access Denied: %v\n", res.Message)
		os.Exit(1)
	}

	fmt.Printf("\n[+] Access Granted!\n[+] Welcome, %s\n", res.Username)
}
