
/**
 * Module dependencies.
 */

var express = require('express')
  , http = require('http')
  , path = require('path');

var fs = require('fs');
var config = JSON.parse(fs.readFileSync('config.json'));
var app = express();
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

// ap230 init
var f_waitAp230 = false;
var f_ap230Json = true;
var f_open = false;
var resAp230 = null;
var SerialPort = require('serialport').SerialPort;
var serial = new SerialPort(config.serial_port, { baudrate: 9600 }, f_open);

serial.on('open', function() {
  log('open serial port');
  serial.on('data', getSer);
});
serial.on('close', closeAp);

function openAp(callback) {
  serial.open(function() {
    f_open = true;
    if (callback)
      return callback();
  });
}
function closeAp() {
  log('close port');
  f_open = false;
}
function getSer(data) {
  log("Serial received", new Date() + ":", data.length);
  log(data); // Buffer
  if (f_waitAp230) {
    f_waitAp230 = false;
    var ans = data.toString('utf8', 0, data.length - 1);
    if (f_ap230Json) {
      resAp230.json({ 'data': ans });
    } else {
      resAp230.locals.message = ans;
      resAp230.render('ap230command');
    }
  }
}
function write(data) {
  serial.write(data, function (err, results) {
    if (err)
      log('write err: ', err);
    log('Serial wrote: ', results);
    /*setTimeout(getSer("--"), 10000); // not work on pi*/
  });
}

// application
app.get('/', function(req,res) {
  res.redirect('/ap230');
});
app.get('/ap230', function(req, res){
  res.locals.f_ap230Json = f_ap230Json;
  res.locals.message = '-';
  res.render('ap230command');
});
app.post('/ap230', function(req, res){
  log('req.params ' + req.params.command);
  log('req.body ' + req.body.command);
  log('req.query ' + req.query.command);
  var command = ':' + req.body.command + '\r';
  log('Command', command);
  log(new Buffer(command));
  f_waitAp230 = true;
  resAp230 = res;
  if (f_open) {
    write(command);
  } else {
    openAp(function() {
      write(command);
    });
  }
});


http.createServer(app).listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});

