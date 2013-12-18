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
    console.log("writeInfo: ", writeInfo);
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
      console.log("read failed", readInfo);
      if (readInfo.resultCode != -1)
        toggleLostConnect("Connection Failed on reading " + readInfo.resultCode);
      f_reading = false;
      return;
    }
    readData();
  });
}

function connectTcp(connecting, callback) {
  if (sockId === -1) {
    if (!connecting)
      return callback(0);
    chrome.socket.create('tcp', {}, function(createInfo) {
      console.log('create', createInfo);
      sockId = createInfo.socketId;
      chrome.socket.connect(sockId, current_gateway, PORT, function(res) {
        log("Open connection: " + sockId + " " + res);
        if (res < 0)
          toggleLostConnect("Connection Failed " + res);
        else
          chrome.storage.sync.set({ 'f_failed_last_connect': false });
        callback(res);
      });
      chrome.runtime.sendMessage({type:'report_sockid', val:sockId});
    });
  } else {
    chrome.socket.getInfo(sockId, function(socketInfo) {
      console.log('getInfo', socketInfo);
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

function zeroFill( number, width )
{
  width -= number.toString().length;
  if ( width > 0 ) {
    return new Array( width + (/\./.test( number ) ? 2 : 1) ).join( '0' ) + number;
  }
  return number + ""; // always return a string
}

var bright_dark = true;
function log(msg) {
  var currentdate = new Date();
  var time = zeroFill(currentdate.getHours(), 2) + ":"
      + zeroFill(currentdate.getMinutes(), 2) + ":"
      + zeroFill(currentdate.getSeconds(), 2) + "."
      + zeroFill(currentdate.getMilliseconds(), 3);
  /*$("#messagewindow").prepend(time + '-> ' + msg + '<br/>');*/
  var list = $("#messagewindow").find('ul');
  /*list.append('<li>something</li>');*/
  if (bright_dark) {
    bright_dark = false;
    list.prepend('<li class="bright">' + time + '-> ' + msg + '</li>');
  } else {
    bright_dark = true;
    list.prepend('<li class="dark">' + time + '-> ' + msg + '</li>');
  }
}

function toggleClearLogDialog() {
  var el = document.getElementById("overlay_log");
  el.style.visibility = (el.style.visibility == "visible") ? "hidden" : "visible";
}

function toggleLostConnect(msg) {
  $("#lostMsg").text(msg);
  var el = document.getElementById("overlay_lost");
  el.style.visibility = (el.style.visibility == "visible") ? "hidden" : "visible";
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
    'f_failed_last_connect': '',
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
  $("#btnClear").click(toggleClearLogDialog);
  $("#btnClearLog").click(function() {
      /*$("#messagewindow").text("");*/
      $("#messagewindow").html("<ul></ul>");
    toggleClearLogDialog();
  });
  $("#btnNotClearLog").click(toggleClearLogDialog);
  $("#btnLostConfirm").click(function() {
    chrome.storage.sync.set({ 'f_failed_last_connect': true });
    chrome.app.window.current().close();
      /*$('#chkConnect').trigger('click');*/
      /*toggleLostConnect();*/
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
    setTimeout(function() {
      function NASort(a, b) {
        if (a.innerHTML == '-') {
          return 1;
        } else if (b.innerHTML == '-') {
          return -1;
        }
        return (a.innerHTML > b.innerHTML) ? 1 : -1;
      };

      $('select option').sort(NASort).appendTo('select');
    }, 3000);
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
    if (!current_gateway)
      current_gateway = "192.168.1.39";
    $("#ip").val(current_gateway);
    if (!items.f_failed_last_connect)
      $('#chkConnect').trigger('click');
  });
}

$(document).ready(startJqm());


