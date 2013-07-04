var sockId = -1;
var readCount = 0;
var IP = "192.168.1.101";
var PORT = 1470;
var f_manualConnect = false;
var f_zero = false;

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
      return cb("-");
    } else {
      document.getElementById('write-info').innerText = data;
      readData(cb);
    }
  });
}

/**
  * return String
  */
function readData(cb) {
  chrome.socket.read(sockId, function(readInfo) {
    console.log("read length:", readInfo.data.byteLength);
    if (!readInfo.data.byteLength)
      return cb("-");
    extractReadMessage(readInfo.data, function(str) {
      document.getElementById('read-info').innerText = str.replace(/\r/g, "");
      document.getElementById('read-count').innerText = ++readCount;
      setStatus("-");
      return cb(str);
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
    writeData($("#command").val(), function(str) {
      cb(str);
    });
  } else {
    connectTcp(true, function(res) {
      if (res !== 0) {
        console.log("there is some error");
        chrome.socket.disconnect(sockId);
        return cb("" + res);
      }
      writeData(cmd, function(str) {
        console.log("Send without connection");
        connectTcp(false, function() {
          cb(str);
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
          return callback();
        });
      });
    } else {
      requestPhoneId(function(success) {
        if (success)
          writeCommand(encrypt(cmd), function(str) {
            return callback();
          });
        else
          return callback();
      });
    }
  } else {
    writeCommand(cmd, function(str) {
      return callback();
    });
  }
}

function requestPhoneId(callback) {
  writeCommand("R,U," + uuid, function(str) {
    if (str[0] === '-')
      return callback(false);
    if (str.length < 9) {
      setStatus("R,U response error");
      return callback(false);
    }
    if (str[6] !== '1') {
      setStatus("Cannot get Phone ID");
      return callback(false);
    }
    setPhoneId(str[4], function() {
      setCurrentPhoneId(str[4]);
      return callback(true);
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

var gws = {};
var current_gateway = "192.168.1.39";
var command = "E,V";

function getCurrentPhoneId() {
  current_gateway = $("#ip").val();
  if (!gws[current_gateway]) {
    gws[current_gateway] = '0';
  }
  console.log("getCurrentPhoneId", current_gateway, gws);
  return gws[current_gateway];
}

function setCurrentPhoneId(id) {
  if (gws[current_gateway] === id)
    return;
  current_gateway = $("#ip").val();
  gws[current_gateway] = id;
  chrome.storage.sync.set({ 'gws': gws });
  console.log("setCurrentPhoneId", current_gateway, id, gws);
}

function setCurrentGateway(cb) {
  if ($("#ip").val() !== current_gateway) {
    current_gateway = $("#ip").val();
    chrome.storage.sync.set({ 'current_gateway': current_gateway });
    if (cb)
      cb();
  } else if (cb) {
    cb();
  }
}

function selectGateway() {
  $("#ip").val(current_gateway);
  $("#command").val(command);
}
//////////////////////////
// Gateway
//////////////////////////


function clearStorage() {
  chrome.storage.sync.set({
      /*'myUuid': '',*/
    'command': '',
    'current_gateway': '',
    'gws': {}
  });
}

function startJqm() {
  $("#ip").keypress(function(e) {
    if (e.which === 13) {
      e.preventDefault();
      setCurrentGateway(function() {
        $("#write-button").click();
      });
    } else if (e.which === 96) { // ` undo
      e.preventDefault();
      $(this).val(current_gateway);
    }
  });
  $("#ip").focusout(function (e) {
    setCurrentGateway();
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
    } else if (e.which === 0x60) { // ` undo
      e.preventDefault();
      $(this).val(command);
    }
  });
  $("#write-button").click(function() {
    var cmd = $("#command").val();
    waitResponse(true);
    var timer1 = setTimeout(function() {
      setStatus("Timeout");
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
    if (cmd !== command) {
      command = cmd;
      chrome.storage.sync.set({ 'command': cmd });
    }
  });
  $("#zero").change(function() {
    if ($(this).is(":checked"))
      f_zero = true;
    else
      f_zero = false;
  });
  /*clearStorage();*/
  chrome.storage.sync.get(function(items) {
    current_gateway = items.current_gateway;
    command = items.command;
    gws = items.gws; // comment out to clear storage
    if (!gws) {
      gws = {};
    }
    if (!current_gateway) {
      current_gateway = "192.168.1.39";
    }
    if (!command) {
      command = "E,L,1";
    }

    selectGateway();
    disableOnConnect(false);
    console.log('chrome.storage.sync.get: ' + current_gateway + ':' + command);
    console.log('gws:', gws);
  });
}

$(document).ready(startJqm());

