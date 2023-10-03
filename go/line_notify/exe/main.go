package main

import (
	"os"
  "strings"

	"github.com/qoopooh/linenotify/notify"
)

func main() {
	msg := ""

	if len(os.Args) > 1 {
		msg = strings.TrimSpace(os.Args[1])
	}

       if len(msg) == 0 {
         return
       }

	param := notify.SendOpts{
		Token:   os.Getenv("LINE_NOTIFY_TOKEN"),
		Prefix:  os.Getenv("LINE_NOTIFY_PREFIX"),
		Message: msg,
		Verbose: true,
	}

	notify.Send(param)
}
