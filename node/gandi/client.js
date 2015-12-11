#!/usr/bin/env node

var client = require('node-gandi');
var apikey = 'DHD2JFzaQlaGkbPm20YE6HPA';

var gandi = new client(apikey, true); // OTE Only

domain = 'berm.org';

var cb = function(err, data) {
  if (data[domain] == 'pending') {
    console.log('result is not yet ready')
    setTimeout(function() {
      gandi.call('domain.available', [[domain]], cb);
    }, 700)
  } else {
    console.dir(data);
  }
}

gandi.call('domain.available', [[domain]], cb);
gandi.call('version.info', [], function(err, data){
  if (err) throw err;
  console.log(data);
});
