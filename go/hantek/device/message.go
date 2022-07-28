package device

import (
	"errors"
)

const (
	DEBUG_MESSAGE_MARKER  = 0x43 // 67
	NORMAL_MESSAGE_MARKER = 0x53 // 83

	SAMPLE_RESPONSE_CMD = 0x82 // 130
	SAMPLE_LEN_SUBCMD   = 0x00
	SAMPLE_DATA_SUBCMD  = 0x01
	SAMPLE_SUM_SUBCMD   = 0x02
	SAMPLE_STOP_SUBCMD  = 0x03 // Errors or STOP mode
)

var NormalSubCommands = [...]int{
	0x02, // Read sample data
	SAMPLE_RESPONSE_CMD,
	0x10, // 16 Read file
	0x90, // 144
	0x12, // 18 Lock/unlock control panel, start/stop acquisition
	0x92, // 146
	0xA0, // 160 screenshot response
}

type Message struct {
	Mark       int
	Length     int
	Command    int
	SubCommand int
	Data       []byte
	Checksum   bool
	Response   bool
}

func BuildMessage(pkt []byte) (Message, error) {
	if pkt[0] == 0 {
		return Message{}, errors.New("no data to build a message")
	}

	msg := Message{
		Mark:     int(pkt[0]),
		Length:   int(pkt[1]) + int(pkt[2])<<8,
		Command:  int(pkt[3]),
		Checksum: checksum(pkt),
		Response: (pkt[3] & 0x80) > 0,
	}

	if msg.Length < 3 {
		// no data
		return msg, nil
	}

	dataIndex := 4

	for _, cmd := range NormalSubCommands {
		if msg.Command == cmd {
			msg.SubCommand = int(pkt[dataIndex])
			dataIndex++
			break
		}
	}

	msg.Data = pkt[dataIndex : msg.Length+2]

	return msg, nil
}

func CreatePacket(msg Message) []byte {
	pkt := []byte{
		byte(msg.Mark),
		0, 0,
		byte(msg.Command),
	}
	length := 2

	for _, cmd := range NormalSubCommands {
		if msg.Command == cmd {
			pkt = append(pkt, byte(msg.SubCommand))
			length++
			break
		}
	}

	if len(msg.Data) > 0 {
		pkt = append(pkt, msg.Data...)
		length += len(msg.Data)
	}

	pkt[1] = byte(length & 0x00ff)
	pkt[2] = byte(length >> 8)
	pkt = append(pkt, makeSum(pkt))

	return pkt
}

func makeSum(pkt []byte) byte {
	var sum byte
	for _, dat := range pkt {
		sum += dat
	}
	return sum
}

func checksum(pkt []byte) bool {
	pktLength := int(pkt[1]) + int(pkt[2])<<8
	chkIndex := int(pktLength + 2)
	if chkIndex > len(pkt) {
		return false
	}

	var sum byte = 0
	for _, dat := range pkt[:chkIndex] {
		sum += dat
	}
	//fmt.Println("sum", sum, pkt[chkIndex])

	return sum == pkt[chkIndex]
}
