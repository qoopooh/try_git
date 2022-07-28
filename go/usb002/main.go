package main

import (
	"fmt"
	"strings"

	"github.com/karalabe/usb"
)

const (
	VID = 0x049f
	PID = 0x505a
)

func main() {
	fmt.Println("https://github.com/karalabe/usb")

	// hids, err := usb.EnumerateHid(VID, PID)
	hids, err := usb.EnumerateHid(0, 0)
	if err != nil {
		panic(err)
	}
	for i, hid := range hids {
		fmt.Println(strings.Repeat("-", 128))
		fmt.Printf("HID #%d\n", i)
		fmt.Printf("  OS Path:      %s\n", hid.Path)
		fmt.Printf("  Vendor ID:    %#04x\n", hid.VendorID)
		fmt.Printf("  Product ID:   %#04x\n", hid.ProductID)
		fmt.Printf("  Release:      %d\n", hid.Release)
		fmt.Printf("  Serial:       %s\n", hid.Serial)
		fmt.Printf("  Manufacturer: %s\n", hid.Manufacturer)
		fmt.Printf("  Product:      %s\n", hid.Product)
		fmt.Printf("  Usage Page:   %d\n", hid.UsagePage)
		fmt.Printf("  Usage:        %d\n", hid.Usage)
		fmt.Printf("  Interface:    %d\n", hid.Interface)
	}
	fmt.Println(strings.Repeat("=", 128))

	// Enumerate all the non-HID devices in alphabetical path order
	raws, err := usb.EnumerateRaw(0, 0)
	if err != nil {
		panic(err)
	}
	for i := 0; i < len(raws); i++ {
		for j := i + 1; j < len(raws); j++ {
			if raws[i].Path > raws[j].Path {
				raws[i], raws[j] = raws[j], raws[i]
			}
		}
	}
	for i, raw := range raws {
		fmt.Printf("RAW #%d\n", i)
		fmt.Printf("  OS Path:    %s\n", raw.Path)
		fmt.Printf("  Vendor ID:  %#04x\n", raw.VendorID)
		fmt.Printf("  Product ID: %#04x\n", raw.ProductID)
		fmt.Printf("  Interface:  %d\n", raw.Interface)
		fmt.Println(strings.Repeat("-", 128))
	}
}
