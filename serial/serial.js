var SerialPort = require('serialport').SerialPort;
var serial = new SerialPort('/dev/ttyUSB0', { baudrate: 38400 });
var log = console.log;

serial.on('open', serialEvent);

function serialEvent() {
  log('open');

  serial.on('data', get);
}

function get(data) {
  log(data); // Buffer
  /*var buffer = new Buffer(data);*/
}

function write(data) {
  serial.write(data, function (err, results) {
    if (err)
      log('write err: ', err);
    log('Written bytes: ', results);
  });
}

exports = write;
exports = get;


