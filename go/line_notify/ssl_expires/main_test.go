package main

import (
	"line_notify/ssl_expires/ssl"
	"testing"
)

func TestCreateMessage(t *testing.T) {

	fqdn := "facebook.com"
	result := ssl.Result{
		Fqdn:          fqdn,
		DaysToExpires: 2,
	}

	if msg := createMessage(result); msg != "facebook.com expires in 2 day(s)" {
		t.Fatalf("Wrong message: %q", msg)
	}
}
