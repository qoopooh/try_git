/**
 * Module dependencies.
 */
var express = require('express')
  , http = require('http')
  , path = require('path');

var fs = require('fs');
var config = JSON.parse(fs.readFileSync('config.json'));
var app = express();
var server = http.createServer(app);
var io = require('socket.io').listen(server);
var log = console.log;

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

// ftc init
var f_ftc = false;
var resAp230 = null;
var SerialPort = require('serialport').SerialPort;
var serial = new SerialPort(config.serial_port, { baudrate: 38400 });
serial.on('open', function() {
  log('open serial port');
  /*serial.on('data', getSer);*/
  serial.on('data', getFtcMessage);
});

var recvBuffer = new Buffer(32 + 1); // 32 byte the maximum length allowed in FTC
var recvBufferIndex = 0;
function getSer(data) { // this is not correct condition
  log("Serial received: ", data.length);
  if (recvBufferIndex > recvBuffer.length) {
    recvBufferIndex = 0;
  }
  data.copy(recvBuffer, recvBufferIndex, 0, data.length);
  recvBufferIndex += data.length;
  if (data[data.length-1] !== 127) {
    return;
  }

  var recvBufferString = hexString(recvBuffer[0]);
  for (i=1; i<recvBufferIndex; i++) {
    recvBufferString += ' ' + hexString(recvBuffer[i]);
  }
  ftc.emit('message', recvBufferString);
  /*log(recvBuffer.slice(0,recvBufferIndex));*/
  log(recvBufferString);
  recvBufferIndex = 0;
}

var FTC_STATE = {
  kInit: {value:0, name:"Initial", code:"I"},
  kMessageSize: {value:1, name:"Message Size", code:"S"},
  kMessageReceive: {value:2, name:"Message Receive", code:"R"},
  kMessageVerify: {value:3, name:"Message Verify", code:"V"},
  kEnd: {value:127, name:"End", code:"E"}
};
var FTC_COMMAD = {
  kAddress: {value:97},
  kUnknown: {value:255}
};

var ftcRcvState = FTC_STATE.kInit;
var ftcMessageSize = 0;

function getFtcMessage(data) {
  var index=0;

  log("Serial received: ", data.length);
  var dataString = hexString(data[0]);
  for (i=1; i<data.length; i++) {
    dataString += ' ' + hexString(data[i]);
  }
  /*log('raw: ', dataString);*/
  
  for (index=0; index<data.length; index++) {
    var dat = data[index];
    if (ftcRcvState.value === FTC_STATE.kInit.value) {
      if (dat !== 125) {
        log("ftc message error: ", ftcRcvState.name, dat);
        return;
      }
      ftcRcvState = FTC_STATE.kMessageSize;
    } else if (ftcRcvState.value === FTC_STATE.kMessageSize.value) {
      if (dat >= 32) {
        log("ftc message error: ", ftcRcvState.name, dat);
        recvBufferIndex = 0;
        return;
      }
      ftcMessageSize = dat;
      ftcRcvState = FTC_STATE.kMessageReceive;
    } else if (ftcRcvState.value === FTC_STATE.kMessageReceive.value) {
      if (dat === 126) {
        ftcRcvState = FTC_STATE.kMessageVerify;
        continue;
      }
      recvBuffer[recvBufferIndex] = dat;
      ++recvBufferIndex;
      if (recvBufferIndex >= ftcMessageSize)
        ftcRcvState = FTC_STATE.kEnd;
    } else if (ftcRcvState.value === FTC_STATE.kMessageVerify.value) {
      if ((dat !== 93) && (dat!==94) && (dat!==95)) {
        log("ftc message error: ", ftcRcvState.name, dat);
        recvBufferIndex = 0;
        return;
      }
      recvBuffer[recvBufferIndex] = dat + 32;
      ++recvBufferIndex;
      if (recvBufferIndex >= ftcMessageSize)
        ftcRcvState = FTC_STATE.kEnd;
      else
        ftcRcvState = FTC_STATE.kMessageReceive;
    } else if (ftcRcvState.value === FTC_STATE.kEnd.value) {
      if (dat !== 127) {
        log("ftc message error: ", ftcRcvState.name, dat);
        recvBufferIndex = 0;
        return;
      }
      //TODO: crc check

      var buf = new Buffer(recvBufferIndex);
      buf[0] = recvBuffer[0] >>> 5;
      buf[1] = (recvBuffer[0] & 28) >>> 2;
      buf[2] = recvBuffer[0] & 3;
      for (i=1; i<recvBufferIndex-2; i++) {
        buf[i+2] = recvBuffer[i];
      }
      var bufString = hexString(buf[0]);
      for (i=1; i<ftcMessageSize; i++) {
        bufString += ' ' + hexString(buf[i]);
      }
      ftc.emit('message', bufString);
      log('emit: ', bufString);
      recvBufferIndex = 0;
      ftcRcvState = FTC_STATE.kInit;
    } else {
      ftcRcvState = FTC_STATE.kInit;
    }
  }
}
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
  sum &= 255;
  sum ^= 255;
  callback(data, sum);
}

function write(data) {
  serial.write(data, function (err, results) {
    if (err)
    log('write err: ', err);
    log('Serial wrote: ', results);
    var dataString = hexString(data[0]);
    for (i=1; i<data.length; i++) {
      dataString += ' ' + hexString(data[i]);
    }

    log('data: ', dataString);
    /*setTimeout(getSer("--"), 10000); // not work on pi*/
  });
}

function sendFtc(data, sum) {
  var buf = new Buffer(data.length + 4); // start + sum + stop

  buf[0] = 125; //start
  buf[1] = data.length + 1; // data length + itself
  data.copy(buf, 2);
  buf[buf.length-2] = sum;
  buf[buf.length-1] = 127; //stop
  write(buf);
}

// application
app.get('/', function(req,res) {
    res.sendfile(path.join(__dirname, 'public', 'socket.html'));
});
app.get('/js/:file', function(req,res) {
    res.sendfile(path.join(__dirname, 'public', 'js', req.params.file));
});
app.get('/css/:file', function(req,res) {
    res.sendfile(path.join(__dirname, 'public', 'css', req.params.file));
});


var ftc = io.of('/ftc');
ftc.on('connection', function(socket) {
  log('on connect');
  socket.on('enquiry', function(data) {
    if (data.cmd === 'address') {
      var buf = new Buffer([72, FTC_COMMAD.kAddress.value , 170, 1]);

      calculateCheckSum(buf, sendFtc);
    }
  });
});

/*io.sockets.on('connection*/

server.listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});

