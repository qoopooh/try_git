var connectionId = -1;
var readString = "";

function setPosition(position) {
  var buffer = new ArrayBuffer(2);
  var uint8View = new Uint8Array(buffer);
  /*chrome.serial.write(connectionId, buffer, function() {});*/
  document.getElementById('write-info').innerText = ab2str(buffer);
};

function writeData(data) {
  var buffer = new ArrayBuffer(data.length + 1);
  var uint8View = new Uint8Array(buffer);

  console.log(data);
  var i=0;
  for (; i<data.length; i++) {
    uint8View[i] = data.charCodeAt(i);
  }
  uint8View[i] = '\r'.charCodeAt(0);

  chrome.serial.write(connectionId, buffer, function() {});
}

function onRead(readInfo) {
  var uint8View = new Uint8Array(readInfo.data);

  if (!uint8View.length) {
    chrome.serial.read(connectionId, 1, onRead);
    return;
  }
  var value = uint8View[0] - '0'.charCodeAt(0);
  var rotation = value * 18.0;

  document.getElementById('image').style.webkitTransform =
    'rotateZ(' + rotation + 'deg)';
  if (uint8View[0] && uint8View[0] !== 255) {
    var dat = readInfo.data;
    document.getElementById('read-info').innerText = ab2str(dat)
      + ", " + dat.byteLength + ", " + uint8View[0];
  }

  console.log("onRead size: ", uint8View.length);
  // Keep on reading.
  chrome.serial.read(connectionId, 1, onRead);
};

function onOpen(openInfo) {
  connectionId = openInfo.connectionId;
  if (connectionId == -1) {
    setStatus('Could not open');
    return;
  }
  setStatus('Connected');

  setPosition(0);
  chrome.serial.read(connectionId, 1, onRead);
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
    if (connectionId != -1) {
      chrome.serial.close(connectionId, openSelectedPort);
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

function startJqm() {
  $("#command").keypress(function (e) {
    if (e.which === 13) {
      e.preventDefault();
      $(this).select();
      writeData($(this).val());
    }
  });
}

onload = function() {
  var tv = document.getElementById('tv');
  navigator.webkitGetUserMedia(
      {video: true},
      function(stream) {
        tv.classList.add('working');
        document.getElementById('camera-output').src =
            webkitURL.createObjectURL(stream);
      },
      function() {
        tv.classList.add('broken');
      });

  document.getElementById('position-input').onchange = function() {
    setPosition(parseInt(this.value, 10));
  };

  chrome.serial.getPorts(function(ports) {
    buildPortPicker(ports)
    openSelectedPort();
  });
};

$(document).ready(startJqm());

