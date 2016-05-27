#!/usr/bin/env nodejs

var SerialPort = require('serialport').SerialPort;
var port = new SerialPort('/dev/ttyUSB0', {
  baudrate: 9600,
});

/*port.on('open', function () {*/
/*port.write('main screen turn on', function(err, bytesWritten) {*/
/*if (err) {*/
/*return console.log('Error: ', err.message);*/
/*}*/
/*console.log(bytesWritten, 'bytes written');*/
/*});*/
/*});*/

port.on('data', function (data) {
  console.log('Data length: ' + data.length);
  if (data.length == 1) {
    if (data == '\r')
      data = 'CR';
    else if (data == '\n')
      data = 'NL\n\n';
    console.log('Data 1: ' + data);
  } else {
    console.log('Multi Data: ' + data);
  }
});

console.log('Hi');

