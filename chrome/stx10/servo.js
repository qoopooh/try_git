var conn_id = -1;
var read_string = "";
var read_count = 0;
var previous_position = 0;
var position_count = 0;

/**
  * write data + return key
  */
function writeData(data) {
  var buffer = new ArrayBuffer(data.length + 1);
  var uint8View = new Uint8Array(buffer);

  if (conn_id === -1) {
    console.log('No connection to write');
    return;
  }
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
  if (conn_id === -1) {
    return;
  }
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

function setStatus(status) {
  document.getElementById('status').innerText = status;
}

function buildPortPicker(ports) {
  var eligiblePorts = ports.filter(function(port) {
    if (port.match(/[Bb]luetooth/))
      return false;
    else if (port.match(/cu./)) // Mac
      return false;
    return true;
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
}

function onOpen(openInfo) {
  conn_id = openInfo.connectionId;
  if (!conn_id) {
    setStatus('Could not open');
    return;
  }
  console.log('Connected', conn_id);
  setStatus('Connected');

  setPosition(0);
  chrome.serial.read(conn_id, 1, onRead);
};

function openSelectedPort() {
  if (conn_id !== -1) {
    console.log("openSelectedPort", conn_id);
    return;
  }
  var portPicker = document.getElementById('port-picker');
  if (!portPicker.length) {
    console.log("there is no port");
    return;
  }
  console.log("portPicker", portPicker, portPicker.length);
  var selectedPort = portPicker.options[portPicker.selectedIndex].value;

  console.log("selectedPort", selectedPort);
  chrome.serial.open(selectedPort, { bitrate: 115200 }, onOpen);
}
function closePort() {
  if (conn_id === -1) {
    console.log("closePort", conn_id);
    return;
  }
  console.log("closing", conn_id);
  chrome.serial.close(conn_id, function() {
    console.log("closed", conn_id);
    conn_id = -1;
  });
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
    if (++position_count > 20) {
      position_count = 0;
      log('pos ' + position);
    }
  }
};

function log(msg) {
  $("#messagewindow").append(timeToString() + ' ' + msg + '<br/>');
  var height = $("#messagewindow")[0].scrollHeight;
  $("#messagewindow").scrollTop(height);
}

function resizeMessageWindow() {
  var bodyheight = $(window).height();
  var bodywidth = $(window).width();

  $("#messagewindow").width(bodywidth - 360);
  $("#image").css({ left: bodywidth - 90 });
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
  $("#btnRefresh").click(function() {
    var select = document.getElementById("port-picker");
    var length = select.options.length;
    for (i = 0; i < length; i++) {
      select.options[i] = null;
    }
    onload();
  });
  $("#btnOpen").click(function() {
    openSelectedPort();
  });
  $("#btnClose").click(function() {
    closePort();
  });
  $("#port-picker").change(function() {
    /*closePort(openSelectedPort);*/
    closePort();
  });

  $("#messagewindow").text('Start: ' + dateToString());
  resizeMessageWindow();
}

onload = function() {
  chrome.serial.getPorts(function(ports) {
    buildPortPicker(ports)
    /*openSelectedPort();*/
  });
};

$(document).ready(init());
$(window).resize(function() {
  resizeMessageWindow();
});

