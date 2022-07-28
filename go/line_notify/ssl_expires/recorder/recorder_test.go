package recorder

import (
	"line_notify/ssl_expires/ssl"
	"testing"
)

func TestFirstMaxDays(t *testing.T) {
	r := Recorder{}
	r.Load()
	r.Clear()

	fqdn := "example.com"
	dat := ssl.Result{
		Fqdn:          fqdn,
		DaysToExpires: 1,
	}

	if !r.ShouldNotify(dat) {
		t.Fatalf("Should notify %v for the first records %q", dat, r.records)
	}

	if r.Count != 1 {
		t.Fatalf("Wrong count %q", r)
	}

	dat.DaysToExpires = 7
	if !r.ShouldNotify(dat) {
		t.Fatalf("Should notify %v for new expires %q", dat, r.records)
	}

	if r.records[fqdn] != "7" {
		t.Fatalf("record of %v should be new set %q", fqdn, r.records)
	}

	dat.DaysToExpires = 3
	if !r.ShouldNotify(dat) || r.records[fqdn] != "7,3" {
		t.Fatalf("Wrong %v records: %q, should be '7,3'",
			fqdn,
			r.records[fqdn])
	}

	dat.DaysToExpires = 4
	if !r.ShouldNotify(dat) || r.records[fqdn] != "7,4,3" {
		t.Fatalf("Wrong %v records: %q", fqdn, r.records[fqdn])
	}
}
