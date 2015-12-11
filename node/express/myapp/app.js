#!/usr/bin/env node

var express = require('express');
var app = express();

app.get('/', function (req, res) {
  res.send('Hi 3');
});

/**
 * To use nodemon:
 *
 * $ npm install nodemon -g
 * $ nodemon app.js
 */
var server = app.listen(3000, function () {
  var host = server.address().address;
  var port = server.address().port;

  console.log('App listening at http://%s:%s', host, port);
});
