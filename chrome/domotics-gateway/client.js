var sockId = -1;
var readString = "";
var readCount = 0;
var IP = "192.168.1.101";
var PORT = 1470;
var f_manualConnect = false;

/**
  * write data(string) + return key
  */
function writeData(data, cb) {
  console.log("write:", data);
  data += "\r\n";

  var buffer = new ArrayBuffer(data.length);
  var uint8View = new Uint8Array(buffer);
  var i=0;
  for (; i<data.length; i++) {
    uint8View[i] = data.charCodeAt(i);
  }

  chrome.socket.write(sockId, buffer, function(writeInfo) {
    var i = writeInfo.bytesWritten;
    console.log("written length:", i);
    
    if (i<0) {
      document.getElementById('write-info').innerText = "-";
      setStatus("Cannot write data");
      if (cb) {
        return cb();
      } else {
        return;
      }
    }

    document.getElementById('write-info').innerText = data;
    chrome.socket.read(sockId, null, function(readInfo) {
      console.log("read length:", readInfo.data.byteLength);
      if (!readInfo.data.byteLength)
        return;
      extractReadMessage(readInfo.data, function(str) {
        document.getElementById('read-info').innerText = str;
        document.getElementById('read-count').innerText = ++readCount;
        if (cb) {
          return cb(str);
        } else {
          return;
        }
      });
    });
  });
}

function extractReadMessage(data, callback) {
  var str = "";
  if (isEncryptedMessage(data)) {
    str = decrypt(data);
  } else {
    str = ab2str(data);
  }
  console.log("read:", str);
  console.log("read byte:", str.length);
  if (callback) {
    return callback(str);
  }
}

function writeCommand(cmd, callback) {
  if (f_manualConnect) {
    writeData($("#command").val(), function(res) {
      callback(res);
    });
  } else {
    connectTcp(true, function(res) {
      if (res !== 0) {
        console.log("there is some error");
        connectTcp(false);
        return;
      }
      writeData(cmd, function(res) {
        console.log("Send without connection");
        connectTcp(false, function() {
          callback(res);
        });
      });
    });
  }
}

function sendCommand(cmd, callback) {
  if (isEncryptionCommand(cmd)) {
    writeCommand("R,U," + uuid, function(res) {
      if (res.length < 9) {
        setStatus("R,U response error");
        return;
      }
      if (res[6] !== '1') {
        setStatus("Cannot get Phone ID");
        return;
      }
      setPhoneId(res[4], function() {
        writeCommand(encrypt(cmd), function(str) {
        });
      });
    });
  } else {
    writeCommand(cmd, function(str) {
      if (callback)
        return callback(str);
    });
  }
}

function setStatus(status) {
  document.getElementById('status').innerText = status;
}

function ab2str(buf) {
  return String.fromCharCode.apply(null, new Uint8Array(buf));
}

function str2ab(str) {
  var buf = new ArrayBuffer(str.length);
  var bufView = new Uint8Array(buf);
  for (var i=0, strLen=str.length; i<strLen; i++) {
    bufView[i] = str.charCodeAt(i);
  }
  return buf;
}

function onConnected(res, cb) {
  console.log("Connect result:", res);
  if (!res) {
    setStatus('Connected');
    disableOnConnect(true);
  } else {
    setStatus('Failed ' + res);
    disableOnConnect(false);
  }
  if (cb) {
    return cb(res);
  } else {
    return;
  }
}

function connectTcp(connecting, callback) {
  if (sockId === -1) {
    if (!connecting)
      if (callback)
        return callback(0);
      else
        return;
    IP = $("#ip").val();
    chrome.socket.create('tcp', {}, function(createInfo) {
      sockId = createInfo.socketId;
      chrome.socket.connect(sockId, IP, PORT, function(res) {
        console.log("Firsly open connection");
        onConnected(res, callback);
      });
    });
  } else {
    chrome.socket.getInfo(sockId, function(socketInfo) {
      if (socketInfo.connected === connecting)
        if (callback)
          return callback(0);
        else
          return;
      if (connecting) {
        IP = $("#ip").val();
        chrome.socket.connect(sockId, IP, PORT, function(res) {
          console.log("Open connection");
          onConnected(res, callback);
        });
      } else {
        chrome.socket.disconnect(sockId);
        console.log("Close connection");
        setStatus('Disconnected');
        disableOnConnect(false);
        if (callback)
          return callback(0);
        else
          return;
      }
    });
  }
}

function disableOnConnect(connected) {
  $("#ip").prop('disabled', connected);
  /*$("#command").prop('disabled', !connected);*/
  /*$("#write-button").prop('disabled', !connected);*/
  $("#connect").prop('checked', connected);
}

//////////////////////////
// Gateway
//////////////////////////

function Gateway(address) {
  this.id = address;
  this.command = "E,V";
  this.phone_id = '0';
  this.date = new Date();
}

function Gateways() {
  this.gateways = [];
  this.version = Gateways.CurrentVersion;
}

Gateways.CurrentVersion = 1;
Gateways.prototype.sync = function() {
  this.gateways.forEach(function(gateway) {
    gateway.date = gateway.date.toJSON();
  });

  chrome.storage.sync.set({ 'gateways': this });
  chrome.storage.sync.set({ 'current_gateway': current_gateway });
}
Gateways.prototype.add = function(gateway) {
  this.gateways.push(gateway);
  this.sync();
}
Gateways.prototype.remove = function(gateway) {
  this.gateways.splice(this.gateways.indexOf(gateway), 1);
  this.sync();
}
Gateways.prototype.length = function() {
  return this.gateways.length;
}
Gateways.prototype.findByKey = function(key, value) {
  for (var i = 0; i < this.gateways.length; ++i) {
    var gateway = this.gateways[i];
    if (gateway[key] === value)
      return gateway;
  }
  return null;
}
Gateways.prototype.findById = function(value) {
  return findByKey("id", value);
}
Gateways.prototype.sortedByKey = function(key) {
  return this.gateways.slice(0).sort(function(a,b){
    var ret = (typeof a[key] === 'string') ? a[key].localeCompare(b[key]) : a[key] - b[key];
    return ret;
  });
}
Gateways.prototype.ordered = function() {
  return this.sortedByKey('date');
}
Gateways.prototype.asArray = function() {
  return this.gateways;
}
function GatewayData(host, cmd, phone_id) {
  this.id = host;
  this.command = cmd;
  this.phone_id = phone_id;
}

var gateways = null;
var current_gateway = null;
var gateway_data = {}; // map gateway.id->GatewayData
//////////////////////////
// Gateway
//////////////////////////

function getInitInfo() {
}

function startJqm() {
  $("#ip").keypress(function (e) {
    if (e.which === 13) {
      e.preventDefault();
      $("#write-button").click();
    }
  });
  $("#connect").change(function() {
    if ($(this).is(":checked")) {
      console.log("connecting");
    } else {
      console.log("disconnecting");
    }
    connectTcp($(this).is(":checked"));
    f_manualConnect = $(this).is(":checked");
  });
  $("#command").keypress(function (e) {
    if (e.which === 13) {
      e.preventDefault();
      $(this).select();
      $("#write-button").click();
    }
  });
  $("#write-button").click(function() {
    sendCommand($("#command").val());
  });
  chrome.storage.sync.get(function(items) {
    if (items.gateways !== undefined && items.gateways.version == Gateways.CurrentVersion) {
      gateways = items.gateways;
      gateways.__proto__ = Gateways.prototype;
      gateways.asArray().forEach(function(gateway) {
        gateway.__proto__ = Gateway.prototype;
        gateway.date = new Date(gateway.date);
      });
    } else {
      gateways = new Gateways();
    }
    if (items.current_gateway !== undefined) {
    } else {
      $("#ip").val("192.168.1.39");
      $("#command").val("E,L,1");
    }
    getInitInfo();
    disableOnConnect(false);
  });
}

$(document).ready(startJqm());

