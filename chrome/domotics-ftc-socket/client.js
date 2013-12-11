var sockId = -1;
var readCount = 0;
var PORT = 1470;
var f_manualConnect = false;
var f_translate = true;
var f_oneline = true;
var f_receive = true;


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

function connectTcp(connecting, callback) {
  if (sockId === -1) {
    if (!connecting)
      return callback(0);
    chrome.socket.create('tcp', {}, function(createInfo) {
      sockId = createInfo.socketId;
      chrome.socket.connect(sockId, current_gateway, PORT, function(res) {
        log("Firsly open connection");
        callback(res);
      });
    });
  } else {
    chrome.socket.getInfo(sockId, function(socketInfo) {
      if (socketInfo.connected === connecting)
        return callback(0);
      if (connecting) {
        chrome.socket.connect(sockId, current_gateway, PORT, function(res) {
          log("Open connection");
          callback(res);
        });
      } else {
        chrome.socket.disconnect(sockId);
        log("Close connection");
        setStatus('Disconnected');
        disableOnConnect(false);
        return callback(0);
      }
    });
  }
}

function disableOnConnect(connected) {
  $("#ip").prop('disabled', connected);
  $("#btnAddress").prop('disabled', !connected);
  $("#btnVersion").prop('disabled', !connected);
}

function log(msg) {
  var currentdate = new Date(); 
  var time = zeroPad(currentdate.getHours()) + ":"  
      + zeroPad(currentdate.getMinutes()) + ":" 
      + zeroPad(currentdate.getSeconds());
  $("#messagewindow").prepend(time + '-> ' + msg + '<br/>');
}

//////////////////////////
// Gateway
//////////////////////////

var gws = {};
var current_gateway = "192.168.1.39";

function setCurrentGateway(obj, cb) {
  var _ip = $(obj).val();
  if (current_gateway != _ip) {
    current_gateway = _ip;
    chrome.storage.sync.set({ 'current_gateway': current_gateway });
  }
  if (cb)
    cb();
}
//////////////////////////
// Gateway
//////////////////////////


function clearStorage() {
  chrome.storage.sync.set({
    'current_gateway': '',
    'gws': {}
  });
}

function startJqm() {
  $("#currenttime").text(new Date());
  $("#ip").keypress(function(e) {
    if (e.which === 13) {
      e.preventDefault();
      setCurrentGateway(this, function() {
        $('#chkConnect').trigger('click');
      });
    } else if (e.which === 96) { // ` undo
      e.preventDefault();
      $(this).val(current_gateway);
    }
  });
  $("#ip").focusout(function (e) {
    setCurrentGateway(this);
  });
  $("#chkConnect").change(function (e) {
    connectTcp($(this).prop('checked'), function(res) {
      if (!res) {
        disableOnConnect(true);
      } else {
        disableOnConnect(false);
      }
    });
  });
  $("#btnClear").click(function() {
    $("#messagewindow").text("");
  });
  $("#chkTranslate").click(function() {
    f_translate = $(this).prop('checked');
  });
  $("#chkOneLine").click(function() {
    f_oneline = $(this).prop('checked');
  });
  $("#chkReceive").click(function() {
    f_receive = $(this).prop('checked');
  });

  f_translate = $("#chkTranslate").prop('checked');
  f_oneline = $("#chkOneLine").prop('checked');
  f_receive = $("#chkReceive").prop('checked');

  /*clearStorage();*/
  chrome.storage.sync.get(function(items) {
    current_gateway = items.current_gateway;
    gws = items.gws; // comment out to clear storage
    if (!gws) {
      gws = {};
    }
    if (!current_gateway) {
      current_gateway = "192.168.1.39";
    }

    $("#ip").val(current_gateway);
    disableOnConnect(false);
    clearDevice();
    console.log('chrome.storage.sync.get: ' + current_gateway);
    console.log('gws:', gws);
  });
}

$(document).ready(startJqm());

