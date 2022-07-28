package device

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestChecksum(t *testing.T) {
	pkt := []byte{0x53, 0x02, 0x00, 0x01, 0x55}
	msg, _ := BuildMessage(pkt)
	assert.False(t, msg.Checksum)

	pkt[len(pkt)-1] = 0x56
	msg, _ = BuildMessage(pkt)
	assert.True(t, msg.Checksum)
}

func TestScreenshotCommand(t *testing.T) {
	pkt := []byte{0x53, 0x02, 0x00, 0x20, 0x75}
	msg, _ := BuildMessage(pkt)

	assert.Equal(t, msg.Mark, NORMAL_MESSAGE_MARKER)
	assert.Equal(t, msg.Length, 2)
	assert.Equal(t, msg.Command, 0x20)
	assert.Equal(t, msg.Data, []byte(nil))
	assert.True(t, msg.Checksum)
}

func TestLockControlPanelCommand(t *testing.T) {
	pkt := []byte{0x53, 0x04, 0x00, 0x12, 0x01, 0x01, 0x6b}
	msg, _ := BuildMessage(pkt)

	assert.Equal(t, msg.Mark, NORMAL_MESSAGE_MARKER)
	assert.Equal(t, msg.Length, 4)
	assert.Equal(t, msg.Command, 0x12)
	assert.Equal(t, msg.SubCommand, 0x01)
	assert.Equal(t, msg.Data, []byte{1})
	assert.True(t, msg.Checksum)
}

func TestBuzzerPacket(t *testing.T) {
	msg := Message{
		Mark:    DEBUG_MESSAGE_MARKER,
		Command: 0x44,
		Data:    []byte{1},
	}

	pkt := CreatePacket(msg)
	assert.Equal(t, []byte{0x43, 0x03, 0x00, 0x44, 0x01, 139}, pkt)
}

func TestScreenshotPacket(t *testing.T) {
	msg := Message{
		Mark:    NORMAL_MESSAGE_MARKER,
		Length:  2,
		Command: 0x20,
	}

	assert.Equal(t, []byte{0x53, 0x02, 0x00, 0x20, 0x75}, CreatePacket(msg))
}
