package main

import (
	"testing"
)

func TestProcessLine(t *testing.T) {
	tests := []struct {
		input       string
		lineNumber  int
		want        string
		wantChanges []string
	}{
		{
			input:      "We should debug it.",
			lineNumber: 1,
			want:       "We shouldดีบักit.",
			wantChanges: []string{
				"debug -> ดีบัก ที่บรรทัดที่ 1",
			},
		},
		{
			input:      "This is a Program.",
			lineNumber: 1,
			want:       "This is aโปรแกรม.",
			wantChanges: []string{
				"Program -> โปรแกรม ที่บรรทัดที่ 1",
			},
		},
	}

	for _, tt := range tests {
		var changes []string
		got := processLine(tt.input, tt.lineNumber, &changes)
		if got != tt.want {
			t.Errorf("processLine() = %q, want %q", got, tt.want)
		}
		if !equal(changes, tt.wantChanges) {
			t.Errorf("changes = %v, want %v", changes, tt.wantChanges)
		}
	}
}

func equal(a, b []string) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}
