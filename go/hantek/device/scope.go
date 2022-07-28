package device

import (
	"context"
	"fmt"
	"log"
	"strconv"
	"strings"
	"time"

	"github.com/google/gousb"
)

const (
	VIDPID = "049f:505a" // hantek
	//VIDPID = "0a5c:4500" // bluetooth
	//VIDPID = "0781:5583" // sandisk
	DEBUG = 2
)

type Scope struct {
	Config       int
	Iface        int
	Alternate    int
	InEnp        int
	OutEnp       int
	PayloadSize  int
	ReadBufSize  int
	WriteBufSize int
	ReadNum      int
	Timeout      string
	readBuffer   []byte
	writeBuffer  []byte
	epIn         *gousb.InEndpoint
	epOut        *gousb.OutEndpoint
	dev          *gousb.Device
	done         func()
}

func Init() (Scope, error) {

	scope := Scope{
		Config:       1,
		Iface:        0,
		Alternate:    0,
		InEnp:        1, // 0x81
		OutEnp:       2,
		PayloadSize:  1024,
		ReadBufSize:  8,
		WriteBufSize: 0,
		ReadNum:      0,
		Timeout:      "500ms",
	}

	ctx := gousb.NewContext()
	defer ctx.Close()

	ctx.Debug(DEBUG)

	vid, pid, err := parseVIDPID(VIDPID)
	if err != nil {
		log.Fatalf("Invalid value for --vidpid (%q): %v", VIDPID, err)
	}

	devs, err := ctx.OpenDevices(func(desc *gousb.DeviceDesc) bool {
		//fmt.Printf("%03d.%03d %s:%s %s\n", desc.Bus, desc.Address, desc.Vendor, desc.Product, usbid.Describe(desc))
		//fmt.Printf("  Protocol: %s\n", usbid.Classify(desc))
		if vid == desc.Vendor && pid == desc.Product {
			return true
		}
		return false
	})
	// All Devices returned from OpenDevices must be closed.
	defer func() {
		for index, d := range devs {
			if index > 0 {
				fmt.Printf("devs close %v\n", d)
				d.Close()
			}
		}
	}()

	// OpenDevices can occasionally fail, so be sure to check its return value.
	if err != nil {
		log.Printf("Warning: OpenDevices: %s.", err)
	}
	switch {
	case len(devs) == 0:
		log.Fatal("No matching devices found.")
	case len(devs) > 1:
		log.Printf("Warning: multiple devices found. Using bus %d, addr %d.", devs[0].Desc.Bus, devs[0].Desc.Address)
		for _, d := range devs[1:] {
			d.Close()
		}
		devs = devs[:1]
	}

	scope.dev = devs[0]
	scope.dev.SetAutoDetach(true)

	//cfg, err := scope.dev.Config(scope.Config)
	//if err != nil {
	//log.Fatalf("dev.Config(%d): %v", scope.Config, err)
	//}

	//intf, err := cfg.Interface(scope.Iface, scope.Alternate)
	//if err != nil {
	//log.Fatalf("cfg.Interface(%d, %d): %v", scope.Iface, scope.Alternate, err)
	//}

	intf, done, err := scope.dev.DefaultInterface()
	scope.done = done

	scope.epIn, err = intf.InEndpoint(scope.InEnp)
	if err != nil {
		log.Fatalf("dev.InEndpoint(): %s", err)
	}
	scope.epOut, err = intf.OutEndpoint(scope.OutEnp)
	if err != nil {
		log.Fatalf("dev.OutEndpoint(): %s", err)
	}

	scope.readBuffer = make([]byte, scope.PayloadSize)
	scope.writeBuffer = make([]byte, scope.PayloadSize)
	return scope, nil
}

func (s *Scope) read(transactions int) error {

	var rdr contextReader = s.epIn
	if s.ReadBufSize > 1 {
		s, err := s.epIn.NewStream(s.PayloadSize, s.ReadBufSize)
		if err != nil {
			log.Fatalf("ep.NewStream(): %v", err)
		}
		defer s.Close()
		rdr = s
	}

	opCtx := context.Background()
	if s.Timeout != "" {
		var done func()
		timeout, _ := time.ParseDuration(s.Timeout)
		opCtx, done = context.WithTimeout(opCtx, timeout)
		defer done()
	}
	for i := 0; transactions == 0 || i < transactions; i++ {
		num, err := rdr.ReadContext(opCtx, s.readBuffer)
		if err != nil {
			log.Fatalf("Reading from device failed: %v", err)
		}
		if DEBUG > 1 {
			fmt.Printf("read %d: %v\n", num, s.readBuffer[:num])
		}
	}

	return nil
}

func (s *Scope) echo() {
	msg := Message{
		Mark:    NORMAL_MESSAGE_MARKER,
		Command: 0x00,
	}
	pkt := CreatePacket(msg)
	s.write(pkt)
}

func (s *Scope) write(pkt []byte) error {

	numBytes, err := s.epOut.Write(pkt)
	if err != nil {
		log.Fatalf("Writing to device failed: %v", err)
	}
	if DEBUG > 1 {
		fmt.Printf("write %d %v\n", numBytes, pkt)
	}

	return err
}

func (s *Scope) Close() {
	s.done()
	s.dev.Close()
	fmt.Println("Close scope")
}

// Make beep sound
func (s *Scope) Beep(seconds float32) {
	msg := Message{
		Mark:    DEBUG_MESSAGE_MARKER,
		Command: 0x44,
		Data:    []byte{byte(seconds * 10)},
	}
	pkt := CreatePacket(msg)
	fmt.Printf("send beep %v %v\n", msg, pkt)
	s.write(pkt)

	//s.echo()

	s.read(1)
	msg, err := BuildMessage(s.readBuffer)
	if err != nil {
		log.Fatalf("BuildMessage` failed: %v", err)
	}
	fmt.Printf("resp: %v\n", msg)
}

func (s *Scope) LockPanel(lock bool) {
	msg := Message{
		Mark:       NORMAL_MESSAGE_MARKER,
		Command:    0x12,
		SubCommand: 0x01,
	}
	if lock {
		msg.Data = []byte{1}
	} else {
		msg.Data = []byte{0}
	}
	pkt := CreatePacket(msg)
	fmt.Printf("send lock %v %v\n", msg, pkt)
	s.write(pkt)
	s.read(1)
	msg, err := BuildMessage(s.readBuffer)
	if err != nil {
		log.Fatalf("BuildMessage` failed: %v", err)
	}
	fmt.Printf("lock resp: %v\n", msg)
}

func (s *Scope) SettingsRequest() {
	msg := Message{
		Mark:    NORMAL_MESSAGE_MARKER,
		Command: 0x01,
	}

	pkt := CreatePacket(msg)
	s.write(pkt)
}

func (s *Scope) ReadSettings() []byte {
	s.read(1)

	msg, err := BuildMessage(s.readBuffer)
	if err != nil {
		log.Fatalf("BuildMessage` failed: %v", err)
	}
	fmt.Printf("settings resp: %v\n", msg)
	return msg.Data
}

type contextWriter interface {
	WriteContext(context.Context, []byte) (int, error)
}

type contextReader interface {
	ReadContext(context.Context, []byte) (int, error)
}

func parseVIDPID(vidPid string) (gousb.ID, gousb.ID, error) {
	s := strings.Split(vidPid, ":")
	if len(s) != 2 {
		return 0, 0, fmt.Errorf("want VID:PID, two 32-bit hex numbers separated by colon, e.g. 1d6b:0002")
	}
	vid, err := strconv.ParseUint(s[0], 16, 32)
	if err != nil {
		return 0, 0, fmt.Errorf("VID must be a hexadecimal 32-bit number, e.g. 1d6b")
	}
	pid, err := strconv.ParseUint(s[1], 16, 32)
	if err != nil {
		return 0, 0, fmt.Errorf("PID must be a hexadecimal 32-bit number, e.g. 1d6b")
	}
	return gousb.ID(vid), gousb.ID(pid), nil
}
