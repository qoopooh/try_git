var sockId = -1;
var readCount = 0;
var PORT = 1470;
var f_manualConnect = false;
var f_translate = true;
var f_oneline = true;
var f_receive = true;
var f_reading = false;


/**
  * write data(string)
  */
function writeData(data, cb) {
  console.log("write:", data);

  var buffer = new ArrayBuffer(data.length);
  var uint8View = new Uint8Array(buffer);
  var i=0;
  uint8View.set(data);
  chrome.socket.write(sockId, buffer, function(writeInfo) {
    var i = writeInfo.bytesWritten;
    if (i != data.length)
      console.log("writte failed:", i);
    console.log("written length:", i);
    if (!f_reading)
      readData();
    if (cb)
      cb();
  });
}

function readData() {
  chrome.socket.read(sockId, function(readInfo) {
    if (readInfo.resultCode > 0) {
      getFtcMessage(new Uint8Array(readInfo.data), function(msg) {
        if (!msg || !f_receive)
          return;
        addCommandDevices(msg.split(' '));
        if (f_translate) {
          translate(msg.split(' '), function(text) {
            if (f_oneline) {
              log(msg + ' -> ' + text);
            } else {
              log(msg);
              log(text);
            }
          });
        } else {
          log(msg);
        }
      });
      f_reading = true;
    } else {
      console.log("read failed");
      f_reading = false;
      return;
    }
    readData();
  });
}

function extractReadMessage(data, callback) {
  var str = ab2str(data);
  console.log("read:", str);
  return callback(str);
}


function hexString(uint8) {
  var text = uint8.toString(16).toUpperCase();
  if (text.length === 1)
    text = '0' + text;
  return text;
}

function ab2hexstr(buf) {
  var bufView = new Uint8Array(buf);
  var msg = "";

  if (bufView.length < 1)
    return msg;

  msg = hexString(bufView[0]);
  for (var i=1; i<bufView.length; i++) {
    msg += " " + hexString(bufView[i]);
  }
  return msg;
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
        log("Open connection: " + sockId + " " + res);
        callback(res);
      });
      chrome.runtime.sendMessage({type:'report_sockid', val:sockId});
    });
  } else {
    chrome.socket.getInfo(sockId, function(socketInfo) {
      if (socketInfo.connected === connecting)
        return callback(0);
      if (!connecting) {
        chrome.socket.disconnect(sockId);
        log("Close connection: " + sockId);
        sockId = -1;
        return callback(sockId);
      }
    });
  }
}

function disableOnConnect(connected) {
  $("#ip").prop('disabled', connected);
  $("#btnAddress").prop('disabled', !connected);
  $("#btnVersion").prop('disabled', !connected);
}

function zeroPad(number) {
  return (number < 10) ? '0' + number : number;
}

function log(msg) {
  var currentdate = new Date(); var time = zeroPad(currentdate.getHours()) + ":"  
      + zeroPad(currentdate.getMinutes()) + ":" 
      + zeroPad(currentdate.getSeconds());
  $("#messagewindow").prepend(time + '-> ' + msg + '<br/>');
}

//////////////////////////
// Gateway
//////////////////////////

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
  });
}

function startJqm() { $("#currenttime").text(new Date());
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
    var checked = $(this).prop('checked');
    connectTcp(checked, function(res) {
      console.log('connectTcp', checked, res);
      if (!res && checked) {
        disableOnConnect(true);
        readData();
      } else {
        disableOnConnect(false);
      }
    });
  });
  $("#btnClear").click(function() {
    $("#messagewindow").text("");
  });
  $("#btnAddress").click(function() {
    var cmd =  new Uint8Array(3);
    cmd[0] = 0x4C;
    cmd[1] = 0x61;
    cmd[2] = 0xAA;
    buildFtc(cmd, function (buf) {
      writeData(buf);
    });
    clearDevice();
  });
  $("#btnVersion").click(function() {
    var cmd =  new Uint8Array(3);
    cmd[0] = 0x4C;
    cmd[1] = 0x77;
    cmd[2] = 0xAA;
    buildFtc(cmd, function (buf) {
      writeData(buf);
    });
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
    if (!current_gateway) {
      current_gateway = "192.168.1.39";
    }

    clearDevice();
    console.log('chrome.storage.sync.get: ' + current_gateway);
    $("#ip").val(current_gateway);
    $('#chkConnect').trigger('click');
  });
}

$(document).ready(startJqm());


