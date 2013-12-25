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
var f_sql_failed = true;

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
FTC_STATE = {
  kInit: {value:0, name:"Initial", code:"I"},
  kMessageSize: {value:1, name:"Message Size", code:"S"},
  kMessageReceive: {value:2, name:"Message Receive", code:"R"},
  kMessageVerify: {value:3, name:"Message Verify", code:"V"},
  kEnd: {value:127, name:"End", code:"E"}
};
FTC_COMMANDS = {
  CMD_ANALOG:0x41,
  CMD_BLOCK:0x42,
  CMD_SCENARIO:0x43,
  CMD_DEVICE:0x44,
  CMD_ERASE_INPUT:0x45,
  CMD_INIT_OUTPUT:0x49,
  CMD_KEY_REMOTE:0x4B,
  CMD_TIMER:0x4C,
  CMD_MODE:0x4D,
  CMD_NAME:0x4E,
  CMD_DIRECT:0x4F,
  CMD_RESET:0x52,
  CMD_SET:0x53,
  CMD_TOGGLE:0x54,
  CMD_UPDATE_SCENARIO:0x55,
  CMD_CLIMA:0x59,
  CMD_TOGGLE_MASTER:0x5A,
  CMD_ADDRESS:0x61,
  CMD_BACKUP:0x62,
  CMD_CONFIG:0x63,
  CMD_DELAY:0x64,
  CMD_ERROR:0x65,
  CMD_FACTORY_RESET:0x66,
  CMD_BLINDS_AUTO:0x68,
  CMD_TEMPERATURE:0x6B,
  CMD_LEDS_BUZZ:0x6C,
  CMD_BLINDS_MANUAL:0x6D,
  CMD_BLINDS_OPEN:0x6F,
  CMD_PROG_DATA:0x70,
  CMD_REPEAT:0x72,
  CMD_STATUS:0x73,
  CMD_TIME_DATE:0x74,
  CMD_UNIT_CMD:0x75,
  CMD_TEMPCTRL:0x76,
  CMD_SOFT_VERSION:0x77,
  CMD_OUTPUTSCTRL:0x78,
  CMD_TEST:0x79,
  CMD_RESTORE:0x7A,
}

var SerialPort = require('serialport').SerialPort;
var serial = new SerialPort(config.serial_port, { baudrate: 38400 });
//var serial = new SerialPort("/dev/ttyS0", { baudrate: 38400 });
serial.on('open', function() {
  log('Open serial port', serial.path);
  serial.on('data', getFtcMessage);
});

var recvBuffer = new Buffer(32 + 1); // 32 byte the maximum length allowed in FTC
var recvBufferIndex = 0;
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
          + hexString(buf[7]),
          function(err, msg) {
            if (err)
              log(dateToString(), 'db error:', err);
            else
              log('sql success:', msg);
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

function write(data) {
  serial.write(data, function (err, results) {
    if (err)
    log('err: ', err);
    log('Serial wrote: ', results);
    var dataString = hexString(data[0]);
    for (i=1; i<data.length; i++) {
      dataString += ' ' + hexString(data[i]);
    }

    log('data: ', dataString);
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
  return callback(); // ignore sql
  var q = "SELECT id FROM command WHERE serial='" + sn + "';";

  try {
    /*if (f_sql_failed) {*/
    /*try {*/
        var sql = mysql.createConnection(sql_options);
        /*} catch (err) {*/
        /*return callback(err);*/
        /*}*/
        /*}*/
    sql.query(q, function(err, rows, fields) {
      if (err)
        return callback(err, q);
      if (rows.length > 0) {
        if (arguments.length == 3) {
          return callback(err, q);
        }
        return;
      }

      var date = dateToString();
      q = "INSERT INTO command (serial, create_date) VALUES ('" + sn + "', '"
          + date + "');";
      sql.query(q, function(err, rows, fields) {
        /*if (err) throw err;*/
        if (arguments.length == 3) {
          return callback(err, q);
        }
      });
    });
  } catch (err) {
    f_sql_failed = true;
    return callback(err);
  }
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
    res.sendfile(path.join(__dirname, 'public', 'socket.html'));
});
app.get('/js/:file', function(req,res) {
    res.sendfile(path.join(__dirname, 'public', 'js', req.params.file));
});
app.get('/css/:file', function(req,res) {
    res.sendfile(path.join(__dirname, 'public', 'css', req.params.file));
});

var msg_nr = 1;
var ftc = io.of('/ftc');
ftc.on('connection', function(socket) {
  log('on connect');
  socket.on('enquiry', function(data) {
    if (data.cmd === 'address') {
      var buf = new Buffer([0x4C, FTC_COMMANDS.CMD_ADDRESS , 0xAA, msg_nr]);

      calculateCheckSum(buf, sendFtc);
    } else if (data.cmd === 'version') {
      var buf = new Buffer([0x4C, FTC_COMMANDS.CMD_SOFT_VERSION , 0xAA, msg_nr]);

      calculateCheckSum(buf, sendFtc);
    }
    msg_nr += 1;
    if (msg_nr > 250)
      msg_nr = 1;
  });
});

server.listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});

