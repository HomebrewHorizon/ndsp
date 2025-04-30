package main

import (
    "fmt"
    "os"
    "net/http"
    "io"
)

// Package deployment function
func deployPackage(packageName string) error {
    packageURL := "https://gomii.example.com/packages/" + packageName + ".zip"
    destination := "/opt/gomii/" + packageName + ".zip"

    fmt.Println("Downloading:", packageURL)

    resp, err := http.Get(packageURL)
    if err != nil {
        return fmt.Errorf("Failed to download package: %v", err)
    }
    defer resp.Body.Close()

    outFile, err := os.Create(destination)
    if err != nil {
        return fmt.Errorf("Failed to save package: %v", err)
    }
    defer outFile.Close()

    _, err = io.Copy(outFile, resp.Body)
    if err != nil {
        return fmt.Errorf("Error writing package data: %v", err)
    }

    fmt.Println("Package downloaded successfully:", destination)
    return nil
}

// Main function
func main() {
    if len(os.Args) < 2 {
        fmt.Println("Usage: gomii-deploy <package-name>")
        os.Exit(1)
    }

    packageName := os.Args[1]
    err := deployPackage(packageName)
    if err != nil {
        fmt.Println("Deployment failed:", err)
        os.Exit(1)
    }

    fmt.Println("Deployment complete!")
}
