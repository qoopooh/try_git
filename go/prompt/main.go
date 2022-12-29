/**
 * ChatGPT helps me to write this code ;)
 */
package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"strings"
)

type Response struct {
	Choices []struct {
		Text string `json:"text"`
	} `json:"choices"`
}

//
// go run main.go -prompt="What are 5 largest cities in Thailand?" -temperature=0.9 -verbose
//
func main() {
	// Define the API endpoint
	//apiEndpoint := "https://api.openai.com/v1/engines/davinci-codex/completions"
	apiEndpoint := "https://api.openai.com/v1/engines/text-davinci-003/completions"

	// Get the API key from the environment variable
	apiKey := os.Getenv("OPENAI_API_KEY")
	if apiKey == "" {
		fmt.Println("Please set the OPENAI_API_KEY environment variable")
		return
	}

	// Define the command line flag for the prompt
	prompt := flag.String("prompt", "", "The prompt for the OpenAI API")
	temperature := flag.Float64("temperature", 0.6, "The temperature for the OpenAI API")
	verbose := flag.Bool("verbose", false, "Print more data")
	flag.Parse()

	// Check if the prompt flag was set
	if *prompt == "" {
		fmt.Println("Please provide a prompt using the -prompt flag")
		return
	}

	// Define the request body
	requestBody := strings.NewReader(fmt.Sprintf(`{
		"prompt": %q,
		"temperature": %f,
		"max_tokens": 1000
	}`, *prompt, *temperature))

	// Create a new HTTP client
	client := &http.Client{}

	// Create a new HTTP request
	req, _ := http.NewRequest("POST", apiEndpoint, requestBody)

	// Add the API key to the request headers
	req.Header.Add("Authorization", fmt.Sprintf("Bearer %s", apiKey))
	req.Header.Add("Content-Type", "application/json")

	// Send the request
	resp, _ := client.Do(req)

	// Read the response body
	body, _ := ioutil.ReadAll(resp.Body)

	if *verbose {
		// Print the response body
		fmt.Println(string(body))
	}

	// Unmarshal the response body into a struct
	var response Response
	json.Unmarshal(body, &response)

	// Iterate over the choices and print the text
	for _, choice := range response.Choices {
		fmt.Println(choice.Text)
	}
}
