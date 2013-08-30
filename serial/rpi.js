
/**
 * Module dependencies.
 */
var express = require('express')
  , http = require('http')
  , mysql = require('mysql')
  , path = require('path');

var fs = require('fs');
var config = JSON.parse(fs.readFileSync('config.json'));
var app = express();
var server = http.createServer(app);
var io = require('socket.io').listen(server);
var log = console.log;
var sql_options = { host: config.sql_ip, port: config.sql_port,
    user: config.sql_user, password: config.sql_pass, database: config.sql_database }; // aaebio
var sql = mysql.createConnection(sql_options);

// all environments
app.set('port', process.env.PORT || config.port);
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser());
app.use(express.methodOverride());
  app.use(express.cookieParser('your secret here'));
  app.use(express.session());
app.use(app.router);
app.use(express.static(path.join(__dirname, 'public')));

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

var SerialPort = require('serialport').SerialPort;
var serial = new SerialPort('/dev/tty.usbserial', { baudrate: 115200 });
serial.on('open', function() {
  log('Open serial port', serial.path);
  serial.on('data', getAscii);
});

var recvBuffer = new Buffer(32 + 1); // 32 byte the maximum length allowed in FTC
var recvBufferIndex = 0;

function hexString(uint8) {
  var text = uint8.toString(16).toUpperCase();
  if (text.length === 1)
    text = '0' + text;
  return text;
}

function calculateCheckSum(data, callback) {
  var sum = data.length + 1; // data length + itself;

  for (var i=0; i<data.length; i++) {
    sum += data[i];
  }
  sum &= 0xFF;
  sum ^= 0xFF;
  callback(data, sum);
}

var dataString = "";
var dataStringSnap = "";
var recvTimeoutId = 0;
function getAscii(data) {
  var index=0;

  if (recvTimeoutId)
    clearTimeout(recvTimeoutId);
  for (var i=0, len = data.length; i < len; i++) {
    dataString += String.fromCharCode(data[i]);
  }
  recvTimeoutId = setTimeout(function() {
    dataStringSnap = dataString;
    log(dataStringSnap);
    dataStringSnap.replace(/\s/g, '');
    rpi.emit('message', dataStringSnap);
    dataString = "";
  }, 3);
}

function write(data) {
  serial.write(data, function (err, results) {
    if (err)
      log('err: ', err);
    log('Serial wrote: ', results);
    var str = hexString(data[0]);
    for (i=1; i<data.length; i++) {
      str += ' ' + hexString(data[i]);
    }

    log('str: ', str);
  });
}

function dateToString(d) {
  var dt = (d) ? d : new Date();

  var year   = dt.getFullYear();
  var month  = zeroPad(dt.getMonth() + 1);
  var day    = zeroPad(dt.getDate());
  var hour   = zeroPad(dt.getHours());
  var minute = zeroPad(dt.getMinutes());
  var second = zeroPad(dt.getSeconds());

  return year + '-' + month + '-' + day + ' ' + hour + ':' + minute + ':' + second;
}

function zeroPad(number) {
  return (number < 10) ? '0' + number : number;
}


// application
app.get('/', function(req,res) {
    res.sendfile(path.join(__dirname, 'public', 'rpi.html'));
});
app.get('/js/:file', function(req,res) {
    res.sendfile(path.join(__dirname, 'public', 'js', req.params.file));
});
app.get('/css/:file', function(req,res) {
    res.sendfile(path.join(__dirname, 'public', 'css', req.params.file));
});

var rpi = io.of('/rpi');
rpi.on('connection', function(socket) {
  log('on connect');
  socket.on('enquiry', function(data) {
    if (data.cmd === 'address') {
    } else if (data.cmd === 'raw') {
      var buf = new Buffer(data.raw + '\r');
      write(buf);
    }
  });
});

server.listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});

