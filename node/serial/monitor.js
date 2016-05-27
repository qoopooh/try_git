#!/usr/bin/env nodejs

var usb = '/dev/ttyUSB0'; // Linux style
/*var usb = '/dev/tty.wchusbserial24120'; // mac style */

process.argv.forEach(function (val, index, array) {
   if (index > 1)
      usb = val;
});

var SerialPort = require('serialport').SerialPort;
var port = new SerialPort(usb, {
  baudrate: 9600,
});

port.on('open', function () {
  port.write('AT+NOTI1\r\n', function(err, bytesWritten) {
    // NOTI1 to show -> OK+LOST\r\n and OK+CONN\r\n
    if (err) {
      return console.log('Error: ', err.message);
    }
    console.log(bytesWritten, 'bytes written');
  });
});

port.on('data', function (data) {
  console.log('Data length: ' + data.length);
  data = ("" + data).replace('\r', '\\r');
  data = data.replace('\n', '\\n\n\n');
  if (data.length == 1) {
    console.log('Data 1: ' + data);
  } else {
    console.log('Multi Data: ' + data);
  }
});

console.log('Open ' + usb + '\n');

