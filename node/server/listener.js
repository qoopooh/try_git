#!/usr/bin/env node

var PORT = 8000;

var count = 0;
var http = require('http');
var server = http.createServer(function(req, resp) {
  console.log(++count, req.headers['user-agent']);

  resp.writeHead(200, {'Content-Type':'text/plain'});
  for (var i=0; i<100; i++) {
    resp.write('text:item: ' + i);
  }

  var data = [];
  for (var i=0; i<100; i++) {
    if (i % 2 == 0) {
      data.push('other:item. ' + i);
    } else {
      for (var x=0; x<i; x++) {
        data.push('other:subitem. ' + x);
      }
    }
  }

  resp.end(data.join('\n'));
});

server.listen(PORT);
console.log('Server is running on', PORT);

