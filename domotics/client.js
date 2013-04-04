var net = require('net');
var enc = require('./encryption.js');
var tcp_port = 1470;
var k_default_host = '192.168.1.39';
var host = k_default_host;
var user_command = "";
var conn = null;
var log = console.log;
var endConn = null;
var f_register = false;

function askGateway(cmd, end) {
  setHost(tcp_port, host);
  if (!cmd && cmd == null) {
    cmd = 'E,V';
  }
  user_command = cmd;
  log('write: ' + cmd);
  if (!enc.isEncryptionCommand(cmd)) {
    conn.write(cmd + '\r\n');
  } else {
    if (!f_register) {
      register();
    } else {
      cmd = enc.encrypt(cmd);
      conn.write(cmd + '\r\n');
    }
  }
  endConn = end;

  return cmd;
}

function setHost(p, h) {
  var f_result = true;

  if (!p) {
    log('Undefine port');
    tcp_port = 1470;
    f_result = false;
  } else {
    tcp_port = p;
  }
  if (!h) {
    log('Undefine host');
    if (host != k_default_host) {
      f_register = false;
      host = k_default_host;
    }
    f_result = false;
  } else {
    if (host != h) {
      f_register = false;
      host = h;
      log('New host');
    }
  }

  if (conn != null) {
    if (f_result)
      return f_result;
    conn.destroy();
    conn = null;
  }
  log('Create connection on ' + host + ':' + tcp_port);
  conn = net.createConnection(tcp_port, host, connectionListener);
  conn.setTimeout(3000);

  return f_result;
}

function getHost() {
  return { ip:host, port:tcp_port };
}

function connectionTimout() {
  log('Timeout in connection');
  endConn("TIME OUT!");
  conn.end();
}

function handleData(data) {
  log('Received: ' + data);
  if (enc.isEncryptedMessage(data)) {
    data = enc.decrypt(data);
  }

  var type = String.fromCharCode(data[0]);
  var code = String.fromCharCode(data[2]);
  if (type == 'C') { // C
    log('Confirmation: ' + code);
    if (code == 'U') {
      var phone_id = String.fromCharCode(data[4]);
      if (phone_id !== '0') {
        log('Phone ID: ' + phone_id);
        enc.setPhoneId(phone_id);
        if (user_command.indexOf("R,U" == -1)) {
          f_register = true;
          log('Registed!');
          cmd = enc.encrypt(user_command);
          conn.write(cmd + '\r\n');
          return;
        }
      }
    }
  } else if  (type == 'I') { // I
    log('Indication: ' + code + '\t' + data);
  }
  endConn(data.toString('utf8', 0, data.length - 2));
  conn.end();
}

function connectionListener() {
  conn.on('connect', function() {
      log('connect to ' + host);
      });

  conn.on('close', function() {
      log('disconnect from ' + host);
      });

  conn.on('error', function(err) {
      log('Error in connection: ' + err);
      });

  conn.on('timeout', connectionTimout);
  conn.on('data', handleData);
}

function register() {
  log('registration');
  setHost(tcp_port, host);
  conn.write('R,U,' + enc.uuid + '\r\n');

}

exports.setHost = setHost;
exports.askGateway = askGateway;
exports.getHost = getHost;

