
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
var serial = new SerialPort(config.blutooth_port, { baudrate: 38400 });
serial.on('open', function() {
  log('Open serial port', serial.path);
  /*serial.on('data', getFtcMessage);*/
  serial.on('data', getAsciiCr);
});

var recvBuffer = new Buffer(32 + 1); // 32 byte the maximum length allowed in FTC
var recvBufferIndex = 0;

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
      if (dat !== 0x7D) {
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
      if (dat === 0x7E) {
        ftcRcvState = FTC_STATE.kMessageVerify;
        continue;
      }
      recvBuffer[recvBufferIndex] = dat;
      ++recvBufferIndex;
      if (recvBufferIndex >= ftcMessageSize)
        ftcRcvState = FTC_STATE.kEnd;
    } else if (ftcRcvState.value === FTC_STATE.kMessageVerify.value) {
      if ((dat !== 0x5D) && (dat !== 0x5E) && (dat !== 0x5F)) {
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
      if (dat !== 0x7F) {
        log("ftc message error: ", ftcRcvState.name, dat);
        recvBufferIndex = 0;
        return;
      }
      //TODO: crc check

      var buf = new Buffer(recvBufferIndex);
      buf[0] = recvBuffer[0] >> 5; // can use >>>
      buf[1] = (recvBuffer[0] & 0x1C) >> 2;
      buf[2] = recvBuffer[0] & 0x03;
      for (i=1; i<recvBufferIndex-2; i++) {
        buf[i+2] = recvBuffer[i];
      }
      var bufString = hexString(buf[0]);
      for (i=1; i<ftcMessageSize; i++) {
        bufString += ' ' + hexString(buf[i]);
      }
      if ((buf.length > 7) && (buf[3] === FTC_COMMANDS.CMD_ADDRESS))
        dbAddCommand(
            hexString(buf[4])
            + hexString(buf[5])
            + hexString(buf[6])
            + hexString(buf[7]), function(err, msg) {
              log('sql:', msg);
            });

      ftc.emit('message', bufString);
      log('emit', new Date(), ': ', bufString);
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
  sum &= 0xFF;
  sum ^= 0xFF;
  callback(data, sum);
}

var dataString = "";
var dataStringSnap = "";
function getAsciiCr(data) {
  var index=0;

  log(dateToString(), "recv:", data.length);
  for (var i=0, len = data.length; i < len; i++) {
    if (data[i] == 0x0D) {
      dataStringSnap = dataString;
      log(dataStringSnap);
      dataStringSnap.replace(/\s/g, '');
      pr.emit('message', dataStringSnap);
      dataString = "";
      continue;
    }
    dataString += String.fromCharCode(data[i]);
  }
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

function sendFtc(data, sum) {
  var buf = new Buffer(data.length + 4); // start + sum + stop

  buf[0] = 0x7D; //start
  buf[1] = data.length + 1; // data length + itself
  data.copy(buf, 2);
  buf[buf.length-2] = sum;
  buf[buf.length-1] = 0x7F; //stop
  write(buf);
}

function dbAddCommand(sn, callback) {
  var q = "SELECT id FROM command WHERE sn='" + sn + "';";

  sql.query(q, function(err, rows, fields) {
    if (err) throw err;
    if (rows.length > 0) {
      if (arguments.length == 3) {
        callback(err, q);
      }
      return;
    }

    var date = dateToString();
    q = "INSERT INTO command (sn, create_date) VALUES ('" + sn + "', '"
        + date + "');";
        /*log(q);*/
    sql.query(q, function(err, rows, fields) {
      if (err) throw err;
      if (arguments.length == 3) {
        callback(err, q);
      }
    });
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
    /*res.sendfile(path.join(__dirname, 'public', 'socket.html'));*/
    res.sendfile(path.join(__dirname, 'public', 'prx10.html'));
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
      var buf = new Buffer([0x48, FTC_COMMANDS.CMD_ADDRESS , 0xAA, 1]);

      calculateCheckSum(buf, sendFtc);
    }
  });
});
var pr = io.of('/pr');
pr.on('connection', function(socket) {
  log('on connect');
  socket.on('enquiry', function(data) {
    if (data.cmd === 'address') {
      var buf = new Buffer([0x30, 0x30, 0x0D]);
      write(buf);
    } else if (data.cmd === 'raw') {
      var buf = new Buffer(data.raw + '\r');
      write(buf);
    }
  });
});

server.listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});

