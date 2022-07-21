package notify

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"net/url"
	"strings"
)

type SendOpts struct {
	Token   string
	Message string
	Prefix  string
}

func Send(param SendOpts) {
	const URL = "https://notify-api.line.me/api/notify"
	data := url.Values{}
	if param.Prefix != "" {
		data.Set("message", fmt.Sprintf("[%s] %s", param.Prefix, param.Message))
	} else {
		data.Set("message", param.Message)
	}

	fmt.Printf("Send: %v\n", data)

	client := &http.Client{}
	req, _ := http.NewRequest(http.MethodPost, URL, strings.NewReader(data.Encode()))
	req.Header.Set("Authorization", fmt.Sprintf("Bearer %s", param.Token))
	req.Header.Add("Content-Type", "application/x-www-form-urlencoded")

	resp, err := client.Do(req)

	if err != nil {
		log.Fatal(fmt.Sprintf("Err %s", err))
	}

	defer resp.Body.Close()
	var res map[string]interface{}
	json.NewDecoder(resp.Body).Decode(&res)

	if res["message"] == "Missing Bearer" {
		res["message"] = fmt.Sprintf(
			"%s (Please set LINE_NOTIFY_TOKEN)",
			res["message"])
	}

	fmt.Printf("Resp: %v\n", res)
}
