var sockId = -1;
var readCount = 0;
var IP = "192.168.1.101";
var PORT = 1470;
var f_manualConnect = false;

/**
  * write data(string)
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
    readData(cb);
  });
}

/**
  * return String
  */
function readData(cb) {
  chrome.socket.read(sockId, function(readInfo) {
    console.log("read length:", readInfo.data.byteLength);
    if (!readInfo.data.byteLength)
      if (cb) {
        return cb("-");
      } else {
        return;
      }
    extractReadMessage(readInfo.data, function(str) {
      document.getElementById('read-info').innerText = str.replace(/\r/g, "");
      document.getElementById('read-count').innerText = ++readCount;
      if (cb) {
        return cb(str);
      } else {
        return;
      }
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
  if (callback) {
    return callback(str);
  }
}

function writeCommand(cmd, cb) {
  if (f_manualConnect) {
    writeData($("#command").val(), function(res) {
      cb(res);
    });
  } else {
    connectTcp(true, function(res) {
      if (res !== 0) {
        console.log("there is some error");
        chrome.socket.disconnect(sockId);
        return cb(res);
      }
      writeData(cmd, function(res) {
        console.log("Send without connection");
        connectTcp(false, function() {
          cb(res);
        });
      });
    });
  }
}

function sendCommand(cmd, callback) {
  if (isEncryptionCommand(cmd)) {
    var id = getCurrentPhoneId();

    if ((id !== '0') && !$("#uid").is(":checked")) {
      setPhoneId(id, function() {
        writeCommand(encrypt(cmd), function(str) {
          if (callback)
            return callback(str);
        });
      });
    } else {
      requestPhoneId(function() {
        writeCommand(encrypt(cmd), function(str) {
          if (callback)
            return callback(str);
        });
      });
    }
  } else {
    writeCommand(cmd, function(str) {
      if (callback)
        return callback(str);
    });
  }
}

function requestPhoneId(callback) {
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
      setCurrentPhoneId(res[4]);
      storeGateways();
      return callback();
    });
  });
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
  /*$("#connect").prop('checked', connected);*/
}

function waitResponse(on) {
  $("#command").prop('disabled', on);
  $("#write-button").prop('disabled', on);
}


//////////////////////////
// Gateway
//////////////////////////

var gws = null;
var current_gateway = "192.168.1.39";
var command = "E,V";

storeGateways = function() {
  chrome.storage.sync.set({ 'gws': gws });
  chrome.storage.sync.set({ 'current_gateway': current_gateway });
  chrome.storage.sync.set({ 'command': command });
}

function getCurrentPhoneId() {
  var id = gws[current_gateway].phone_id;

  if (!id) {
    id = '0';
    gws[current_gateway].phone_id = id;
  }
  return id;
}

function setCurrentPhoneId(id) {
  gws[current_gateway].phone_id = id;
}

function selectGateway() {
  $("#ip").val(current_gateway);
  $("#command").val(command);
}


//////////////////////////
// Gateway
//////////////////////////

function startJqm() {
  $("#ip").keypress(function (e) {
    if (e.which === 13) {
      e.preventDefault();
      var ip = $(this).val();
      
      if (ip !== current_gateway) {
        current_gateway = ip;
        command = $("#command").val();
      }
      $("#write-button").click();
    }
  });
  $("#ip").focusout(function (e) {
    if (ip !== current_gateway) {
      current_gateway = ip;
      command = $("#command").val();
    }
  });
  /*$("#connect").change(function() {*/
  /*if ($(this).is(":checked")) {*/
  /*console.log("connecting");*/
  /*} else {*/
  /*console.log("disconnecting");*/
  /*}*/
  /*connectTcp($(this).is(":checked"));*/
  /*f_manualConnect = $(this).is(":checked");*/
  /*});*/
  $("#command").keypress(function (e) {
    if (e.which === 13) {
      e.preventDefault();
      $(this).select();
      $("#write-button").click();
    }
  });
  $("#write-button").click(function() {
    var cmd = $("#command").val();
    waitResponse(true);
    var timer1 = setTimeout(function() {
      document.getElementById('read-info').innerText = "Timeout";
      console.log("timeout");
      disableOnConnect(false);
      waitResponse(false);
      chrome.socket.destroy(sockId);
      sockId = -1;
    }, 3000);
    sendCommand(cmd, function() {
      clearTimeout(timer1);
      waitResponse(false);
    });
    command = cmd;
    gws.sync();
  });
  chrome.storage.sync.get(function(items) {
    current_gateway = items.current_gateway;
    command = items.command;
    gws = items.gws;

    if (!current_gateway) {
      current_gateway = "192.168.1.39";
    }
    if (!command) {
      command = "E,L,1";
    }
    if (!gws) {
      gws[current_gateway].phone_id = '0';
    }

    selectGateway();
    disableOnConnect(false);
  });
}

$(document).ready(startJqm());

