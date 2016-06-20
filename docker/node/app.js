var express = require('express');
var app = express();

PORT = 3001;

app.get('/', function (req, res) {
  res.send('Hello World 1!');
});

app.listen(PORT, function () {
  console.log('Example app listening on port ' + PORT);
});
