package main

import (
	"os"

	"github.com/qoopooh/try_git/go/line_notify/notify"
)

func main() {
	msg := "test"

	if len(os.Args) > 1 {
		msg = os.Args[1]
	}

	param := notify.SendOpts{
		Token:   os.Getenv("LINE_NOTIFY_TOKEN"),
		Message: msg,
	}
	notify.Send(param)
}
