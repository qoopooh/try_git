//                                  url.parse(string).query
//                                              |
//              url.parse(string).pathname      |
//                          |                   |
//                          |                   |
//                          ------ -------------------
//     http://localhost:8888/start?foo=bar&hello=world
//                                     ---       -----
//                                     |          |
//                                     |          |
//                  querystring(string)["foo"]    |
//                                                |
//                             querystring(string)["hello"]

var server = require('./server.js');
var socket = require('./client');

server.start(socket);

