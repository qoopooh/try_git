var conn_id = -1;
var read_string = "";
var read_count = 0;
var previous_position = 0;

/**
  * write data + return key
  */
function writeData(data) {
  var buffer = new ArrayBuffer(data.length + 1);
  var uint8View = new Uint8Array(buffer);

  console.log(data);
  var i=0;
  for (; i<data.length; i++) {
    uint8View[i] = data.charCodeAt(i);
  }
  uint8View[i] = '\r'.charCodeAt(0);

  chrome.serial.write(conn_id, buffer, function() {
    document.getElementById('write-info').innerText = data;
  });
}

/**
 * designed for read 1 by 1
 */
function onRead(readInfo) {
  var uint8View = new Uint8Array(readInfo.data);
  var ch = uint8View[0];

  if (!uint8View.length) {
    chrome.serial.read(conn_id, 1, onRead);
    return;
  }
  var value = uint8View[0] - '0'.charCodeAt(0);
  setPosition(value);

  if (ch !== 0x0d) {
    read_string += String.fromCharCode(ch);
  } else {
    console.log("read_string: ", read_string);
    document.getElementById('read-info').innerText = read_string;
    document.getElementById('read-count').innerText = ++read_count;
    read_string = "";
  }

  console.log("onRead size: ", uint8View.length);
  // Keep on reading.
  chrome.serial.read(conn_id, 1, onRead);
};

function onOpen(openInfo) {
  conn_id = openInfo.connectionId;
  if (conn_id == -1) {
    setStatus('Could not open');
    return;
  }
  setStatus('Connected');

  setPosition(0);
  chrome.serial.read(conn_id, 1, onRead);
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
    if (conn_id != -1) {
      chrome.serial.close(conn_id, openSelectedPort);
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

function zeroPad(number) {
  return (number < 10) ? '0' + number : number;
}

function timeToString(d) {
  var dt = (d) ? d : new Date();

  var hour   = zeroPad(dt.getHours());
  var minute = zeroPad(dt.getMinutes());
  var second = zeroPad(dt.getSeconds());

  return hour + ':' + minute + ':' + second;
}

function dateToString(d) {
  var dt = (d) ? d : new Date();

  var year   = dt.getFullYear();
  var month  = zeroPad(dt.getMonth() + 1);
  var day    = zeroPad(dt.getDate());

  return year + '-' + month + '-' + day + ' ' + timeToString();
}

function setPosition(position) {
  var rotation = position * 18.0;
  document.getElementById('image').style.webkitTransform =
    'rotateZ(' + rotation + 'deg)';
  if (position !== previous_position) {
    previous_position = position;
    log('pos ' + position);
  }
};

function log(msg) {
  $("#messagewindow").append(timeToString() + '-> ' + msg + '<br/>');
  var height = $("#messagewindow")[0].scrollHeight;
  $("#messagewindow").scrollTop(height);
}

function init() {
  $("#command").keypress(function (e) {
    if (e.which === 13) {
      e.preventDefault();
      $(this).select();
      writeData($(this).val());
    }
  });
  $("#btnClear").click(function() {
    $("#messagewindow").text("");
  });
  $("#messagewindow").text('Start: ' + dateToString());
}

onload = function() {

  chrome.serial.getPorts(function(ports) {
    buildPortPicker(ports)
    openSelectedPort();
  });
};

$(document).ready(init());

