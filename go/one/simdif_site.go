package main

import (
	"bufio"
	"context"
	"fmt"
	"net"
	"time"
	"os"
	"strings"
	"sync"
)

func checkDomain(domain string, targetIPs []string, wg *sync.WaitGroup, mu *sync.Mutex) {
	defer wg.Done()

	// Remove http:// or https:// prefix if present
	domain = strings.TrimPrefix(domain, "http://")
	domain = strings.TrimPrefix(domain, "https://")

	// Remove any trailing slashes or paths
	if idx := strings.Index(domain, "/"); idx != -1 {
		domain = domain[:idx]
	}

	// Perform DNS lookup for IP addresses (A/AAAA records)
	timeout := 5 * time.Second
	ctx, cancel := context.WithTimeout(context.Background(), timeout)
	defer cancel()

	ips, err := net.DefaultResolver.LookupIP(ctx, "ip", domain)
	if err != nil {
		mu.Lock()
		fmt.Printf("%s NO (Error: %v)\n", domain, err)
		mu.Unlock()
		return
	}

	// Check if any of the resolved IPs match the target IPs
	matched := false
	for _, ip := range ips {
		ipStr := ip.String()
		for _, targetIP := range targetIPs {
			if ipStr == targetIP {
				matched = true
				break
			}
		}
		if matched {
			break
		}
	}

	// Print result (lock to prevent race conditions in output)
	mu.Lock()
	if matched {
		fmt.Printf("%s YES\n", domain)
	} else {
		fmt.Printf("%s NO\n", domain)
	}
	mu.Unlock()
}

func main() {
	// Read target IPs from environment variable TARGET_IPS
	targetIPsStr := os.Getenv("TARGET_IPS")
	if targetIPsStr == "" {
		fmt.Println("Error: TARGET_IPS environment variable is not set")
		os.Exit(1)
	}

	// Split the comma-separated string into a slice of IPs and trim whitespace
	targetIPs := strings.Split(targetIPsStr, ",")
	for i := range targetIPs {
		targetIPs[i] = strings.TrimSpace(targetIPs[i])
	}

	// Validate that at least one IP is provided
	if len(targetIPs) == 0 || (len(targetIPs) == 1 && targetIPs[0] == "") {
		fmt.Println("Error: No valid IPs found in TARGET_IPS")
		os.Exit(1)
	}

	// Input file name (replace with your file path if needed)
	inputFile := "domains.txt"
	if len(os.Args) > 1 {
		inputFile = os.Args[1]
	}

	// Open the input file
	file, err := os.Open(inputFile)
	if err != nil {
		fmt.Printf("Error opening input file %s: %v\n", inputFile, err)
		os.Exit(1)
	}
	defer file.Close()

	// Create a WaitGroup to wait for all goroutines to finish
	var wg sync.WaitGroup

	// Create a Mutex to synchronize output to avoid interleaved printing
	var mu sync.Mutex

	// Create a scanner to read the file line by line
	scanner := bufio.NewScanner(file)

	// Process each line (domain) in the file
	for scanner.Scan() {
		// Get the domain from the line and trim whitespace
		domain := strings.TrimSpace(scanner.Text())

		// Skip empty lines
		if domain == "" {
			continue
		}

		// Increment WaitGroup counter and launch a goroutine for each domain
		wg.Add(1)
		go checkDomain(domain, targetIPs, &wg, &mu)
	}

	// Check for errors from scanning the file
	if err := scanner.Err(); err != nil {
		fmt.Printf("Error reading input file: %v\n", err)
	}

	// Wait for all goroutines to finish
	wg.Wait()
}
