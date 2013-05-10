var sockId = -1;
var readString = "";
var readCount = 0;
var IP = "192.168.1.101";
var PORT = 1470;

function setPosition(position) {
  var buffer = new ArrayBuffer(2);
  var uint8View = new Uint8Array(buffer);
  /*chrome.serial.write(sockId, buffer, function() {});*/
};

/**
  * write data + return key
  */
function writeData(data) {
  var buffer = new ArrayBuffer(data.length);
  var uint8View = new Uint8Array(buffer);

  console.log("write:", data);
  var i=0;
  for (; i<data.length; i++) {
    uint8View[i] = data.charCodeAt(i);
  }

  chrome.socket.write(sockId, buffer, function() {
    document.getElementById('write-info').innerText = data;
    chrome.socket.read(sockId, null, function(readInfo) {
      var str = ab2str(readInfo.data);
      console.log("can read:", str);
      document.getElementById('read-info').innerText = str;
    });
  });
}

/**
 * designed for read 1 by 1
 */
function onRead(readInfo) {
  var uint8View = new Uint8Array(readInfo.data);
  var ch = uint8View[0];

  if ($("#connect").is(":unchecked")) {
    console.log("stop read");
    return;
  }
  if (!uint8View.length) {
    chrome.socket.read(sockId, 1, onRead);
    return;
  }
  var value = uint8View[0] - '0'.charCodeAt(0);
  var rotation = value * 18.0;
  document.getElementById('image').style.webkitTransform =
    'rotateZ(' + rotation + 'deg)';

  if (ch !== 0x0d) {
    readString += String.fromCharCode(ch);
  } else {
    console.log("readString: ", readString);
    document.getElementById('read-info').innerText = readString;
    document.getElementById('read-count').innerText = ++readCount;
    readString = "";
  }

  console.log("onRead size: ", uint8View.length);
  // Keep on reading.
  /*chrome.socket.read(sockId, 1, onRead);*/
};

function onOpen(openInfo) {
  sockId = openInfo.connectionId;
  if (sockId == -1) {
    setStatus('Could not open');
    return;
  }
  setStatus('Connected');

  setPosition(0);
  chrome.serial.read(sockId, 1, onRead);
};

function setStatus(status) {
  document.getElementById('status').innerText = status;
}

function buildPortPicker(ports) {
  var eligiblePorts = ports.filter(function(port) {
    return !port.match(/[Bb]luetooth/);
  });

  var portPicker = document.getElementById('port-picker');
  var portcount = -1;
  eligiblePorts.forEach(function(port) {
    var portOption = document.createElement('option');
    portOption.value = portOption.innerText = port;
    portPicker.appendChild(portOption);
    ++portcount;
  });
  portPicker.selectedIndex = portcount;

  portPicker.onchange = function() {
    if (sockId != -1) {
      chrome.serial.close(sockId, openSelectedPort);
      return;
    }
    openSelectedPort();
  };
}

function openSelectedPort() {
  var portPicker = document.getElementById('port-picker');
  var selectedPort = portPicker.options[portPicker.selectedIndex].value;
  chrome.serial.open(selectedPort, { bitrate: 38400 }, onOpen);
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
  $("#connect").prop('checked', connected);
}

function startJqm() {
  $("#ip").keypress(function (e) {
      /*console.log("keypress");*/
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
      writeData($(this).val());
    }
  });
  /*$("#command").disabled = true;*/
  $("#ip").val("192.168.1.32");
  $("#command").val("hello world!");
  disableOnConnect(false);
}

$(document).ready(startJqm());

