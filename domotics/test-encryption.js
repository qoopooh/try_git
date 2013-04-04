var log = console.log;
var enc = require('./encryption');
var message = "E,L,1";
var enc_message = "5,p,y,g\r\n";


enc.setPhoneId("5");
/*var result = enc.encrypt(message);*/
var result = enc.decrypt(enc_message);
log('result: ' + result);

