var express = require('express');
var app = express();

PORT = 3001;

var tools = function (req, res) {
  tools = require('./tools.js');
  var dat =  tools.foo();
  res.send('Hello World 1!<br>' + dat);
};

app.get('/', tools);

app.listen(PORT, function () {
  console.log('Example app listening on port ' + PORT);
});
