package main

import (
	"bytes"
	"encoding/json"
	"flag"
	"fmt"
	"net/http"
	"os"
)

func main() {
	urlFlag := flag.String("u", os.Getenv("SLACK_WEBHOOK_URL"), "URL to send the request to")
	messageFlag := flag.String("m", "", "Message to send")

	flag.Parse()

	webhookURL := *urlFlag
	if webhookURL == "" {
		fmt.Println("Error: SLACK_WEBHOOK_URL is not set")
		os.Exit(1)
	}

	if *messageFlag == "" {
		fmt.Println("Error: Message cannot be empty")
		os.Exit(1)
	}

	payload := map[string]string{
		"text": *messageFlag,
	}
	jsonPayload, err := json.Marshal(payload)
	if err != nil {
		fmt.Println("Error: Failed to marshal JSON payload")
		os.Exit(1)
	}

	response, err := http.Post(webhookURL, "application/json", bytes.NewBuffer(jsonPayload))
	if err != nil {
		fmt.Println("Error: Failed to send request")
		os.Exit(1)
	}

	defer response.Body.Close()
	if response.StatusCode != http.StatusOK {
		fmt.Printf("Error: Received non-200 response: %s\n", response.Status)
		os.Exit(1)
	}

	fmt.Println("Message sent successfully!")
}
