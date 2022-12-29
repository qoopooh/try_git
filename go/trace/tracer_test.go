package trace

/**
Testing was built into the Go tool chain from the very beginning, making writing automatable tests a first-class citizen.
The test code lives alongside the production code in files suffixed with _test.go.
The Go tools will treat any function that starts with Test (taking a single *testing.T argument) as a unit test, and it will be executed when we run our tests.

``sh
go test -cover
``
*/

import (
	"bytes"
	"testing"
)

func TestNew(t *testing.T) {
	var buf bytes.Buffer
	tracer := New(&buf)
	if tracer == nil {
		t.Error("We haven't written our test yet")
	} else {
		tracer.Trace("Hello trace package.")
		if buf.String() != "Hello trace package.\n" {
			t.Errorf("Trace should not write '%s'.", buf.String())
		}
	}
}

func TestOff(t *testing.T) {
	var silentTracer Tracer = Off()
	silentTracer.Trace("something")
}
