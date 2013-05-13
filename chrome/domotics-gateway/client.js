var sockId = -1;
var readString = "";
var readCount = 0;
var IP = "192.168.1.101";
var PORT = 1470;

/**
  * write data + return key
  */
function writeData(data) {
  console.log("write:", data);
  data += "\r\n";

  var buffer = new ArrayBuffer(data.length);
  var uint8View = new Uint8Array(buffer);
  var i=0;
  for (; i<data.length; i++) {
    uint8View[i] = data.charCodeAt(i);
  }

  chrome.socket.write(sockId, buffer, function() {
    document.getElementById('write-info').innerText = data;
    chrome.socket.read(sockId, null, function(readInfo) {
      var str = ab2str(readInfo.data);
      console.log("read:", str);
      document.getElementById('read-info').innerText = str;
    });
  });
}

function setStatus(status) {
  document.getElementById('status').innerText = status;
}

function ab2str(buf) {
  return String.fromCharCode.apply(null, new Uint8Array(buf));
}

function onConnected(result) {
  console.log("Connect result:", result);
  if (!result) {
    setStatus('Connected');
    disableOnConnect(true);
  } else {
    setStatus('Failed ' + result);
    disableOnConnect(false);
  }
}

function connectTcp(connecting) {
  if (sockId === -1) {
    if (!connecting)
      return;
    IP = $("#ip").val();
    chrome.socket.create('tcp', {}, function(createInfo) {
      sockId = createInfo.socketId;
      chrome.socket.connect(sockId, IP, PORT, onConnected);
    });
  } else {
    chrome.socket.getInfo(sockId, function(socketInfo) {
      if (socketInfo.connected === connecting)
        return;
      if (connecting) {
        IP = $("#ip").val();
        chrome.socket.connect(sockId, IP, PORT, onConnected);
      } else {
        chrome.socket.disconnect(sockId);
        setStatus('Disconnected');
        disableOnConnect(false);
      }
    });
  }
}

function disableOnConnect(connected) {
  $("#ip").prop('disabled', connected);
  $("#command").prop('disabled', !connected);
  $("#write-button").prop('disabled', !connected);
  $("#connect").prop('checked', connected);
}

function startJqm() {
  $("#ip").keypress(function (e) {
    if (e.which === 13) {
      e.preventDefault();
      connectTcp(true);
    }
  });
  $("#connect").change(function() {
    if ($(this).is(":checked")) {
      console.log("connecting");
    } else {
      console.log("disconnecting");
    }
    connectTcp($(this).is(":checked"));
  });
  $("#command").keypress(function (e) {
    if (e.which === 13) {
      e.preventDefault();
      $(this).select();
      $("#write-button").click();
    }
  });
  $("#write-button").click(function() {
    writeData($("#command").val());
  });
  $("#ip").val("192.168.1.33");
  $("#command").val("E,V");
  disableOnConnect(false);
}

$(document).ready(startJqm());

