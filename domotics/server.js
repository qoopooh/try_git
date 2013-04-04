var http = require('http');
var url = require('url');
var fs = require('fs');
var log = console.log;
var listenning_port = 8888;
var count = 0;

function start(socket) {
  function onRequest(request, response) {
    var pathname = url.parse(request.url).pathname;
    var param = url.parse(request.url, true).query;

    if(pathname == '/favicon.ico') {
      fs.readFile("./" + request.url, function(err, data) {
          response.writeHead(200, {'Content-Type': 'image/x-icon'} );
          response.end(data);
          log("Load icon");
          });
      return;
    }

    ++count;
    execute(say, 'Request received: ' + count);
    response.writeHead(200, {'Content-Type': 'text/html'});
    response.write("<h1>Request for " + pathname + " recieved.</h1>");
    response.write("<p>count: " + count + "</p>");
    socket.setHost(param.port, param.ip);
    response.write("<p>IP: " + socket.getHost().ip + "</p>");
    response.write("<p>Port: " + socket.getHost().port + "</p>");
    response.write("<p>Command: " + param.cmd + "</p>");
    socket.askGateway(param.cmd, endConn);

    function endConn(resp) {
      response.write("<p>Response: " + resp + "</p>");
      response.end();
    }
  }

  http.createServer(onRequest).listen(listenning_port);
  log('Start server on port ' + listenning_port);
}

function say(word) {
  log(word);
}

function execute(someFunction, value) {
  someFunction(value);
}

exports.start = start;

