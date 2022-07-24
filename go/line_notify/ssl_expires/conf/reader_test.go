package conf

import "testing"

func TestReading(t *testing.T) {
	cfg := Read()

	if len(cfg.Domains) < 1 {
		t.Fatalf("Missing domains: %q", cfg)
	}
}
