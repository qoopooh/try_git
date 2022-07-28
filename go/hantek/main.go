package main

import (
	"fmt"

	"th.berm/hantek/device"
)

// Make beep sound
func beep(seconds float32) {
	scope, err := device.Init()
	if err != nil {
		_ = fmt.Errorf("scope error: %v", err)
	}
	defer scope.Close()

	scope.Beep(seconds)
}

func readDsoSettings() []byte {
	scope, err := device.Init()
	if err != nil {
		_ = fmt.Errorf("DSO error: %v", err)
	}
	defer scope.Close()

	//scope.LockPanel(true)
	scope.SettingsRequest()
	//scope.LockPanel(false)
	settings := scope.ReadSettings()

	return settings
	// return make([]byte, 0)
}

func main() {
	//beep(0.1)
	out := readDsoSettings()
	fmt.Printf("set %v\n", out)
}
