var conn_id = 0;
var read_string = "";
var previous_position = 0;
var position_count = 0;
var readBuffSize = 256;
var readBuff = new ArrayBuffer(readBuffSize);
var readArray = new Uint8Array(readBuff);
var readIndex = 0;
var readCount = 0;
var f_openport = false;

/**
  * write data + return key
  */
function writeData(data) {
  var buffer = new ArrayBuffer(data.length + 1);
  var uint8View = new Uint8Array(buffer);

  if (!conn_id) {
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
var timeoutReadBuffer = 0;
function onRead(readInfo) {
  if (conn_id < 1 || !f_openport) {
    return;
  }
  var uint8View = new Uint8Array(readInfo.data);
  var len = uint8View.length;

  if (!len) {
    console.log('read null');
    var timer1 = setTimeout(function() {
      chrome.serial.read(conn_id, 1, onRead);
    }, 250);
    return;
  }
  /*console.log(uint8View[0]);*/
  if (readIndex + len >= readBuffSize) {
    for (var i = 0; i < len; i++) {
      console.log(uint8View[i].toString(16));
    }
    log(ab2str(readBuff));
    readIndex = 0;
  }
  readArray.set(uint8View, readIndex);
  readIndex += len;
  readCount += len;
  document.getElementById('read-count').innerText = readCount.toString();

  /*var value = uint8View[0] - '0'.charCodeAt(0);*/
  /*setPosition(value);*/

  /*if (ch !== 0x0d) {*/
  /*read_string += String.fromCharCode(ch);*/
  /*} else {*/
  /*console.log("read_string: ", read_string);*/
  /*document.getElementById('read-info').innerText = read_string;*/
  /*document.getElementById('read-count').innerText = ++readCount;*/
  /*read_string = "";*/
  /*}*/

  /*console.log("onRead size: ", uint8View.length);*/
  console.log('get', len);
  // Keep on reading.
  chrome.serial.read(conn_id, 64, onRead);
  clearTimeout(timeoutReadBuffer);
  timeoutReadBuffer = setTimeout(function() {
    for (var i = 0; i < readIndex; i++) {
      console.log(readArray[i].toString(16));
    }
    log(u82str(readArray.subarray(0, readIndex)));
    console.log("timeout", readIndex);
    readIndex = 0;
  }, 1000);
};

function setStatus(st) {
  $('#status').text(st);
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
  var portcount = 0;
  eligiblePorts.forEach(function(port) {
    var portOption = document.createElement('option');
    portOption.value = portOption.innerText = port;
    portPicker.appendChild(portOption);
    ++portcount;
  });
  portPicker.selectedIndex = portcount - 1;
  setStatus('Refresh ' + portcount + ' port(s)');
}

function onOpen(openInfo) {
  conn_id = openInfo.connectionId;
  if (conn_id < 1) {
    setStatus('Could not open');
    return;
  }
  console.log('Connected', conn_id);

  setPosition(0);
  readIndex = 0;
  readCount = 0;
  chrome.serial.read(conn_id, 1, onRead);
};

function openSelectedPort() {
  setStatus('Connected');
  f_openport = true;
  if (conn_id > 0) {
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
  /*chrome.serial.open(selectedPort, { bitrate: 115200 }, onOpen);*/
  /*chrome.serial.open(selectedPort, { bitrate: 38400 }, onOpen);*/
  chrome.serial.open(selectedPort, { bitrate: 9600 }, onOpen);
}
function closePort() {
  f_openport = false;
  setStatus('Closed');
  log('Closed');
  if (conn_id < 1) {
    console.log("closePort", conn_id);
    return;
  }
  chrome.serial.flush(conn_id, function() {
    console.log("closing", conn_id);
    chrome.serial.close(conn_id, function() {
      console.log("closed", conn_id);
      conn_id = 0;
    });
  });
}

function ab2str(buf) {
  return String.fromCharCode.apply(null, new Uint8Array(buf));
}
function u82str(uint) {
  return String.fromCharCode.apply(null, uint);
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
  /*var rotation = position * 18.0;*/
  /*document.getElementById('image').style.webkitTransform =*/
  /*'rotateZ(' + rotation + 'deg)';*/
  /*if (position !== previous_position) {*/
  /*previous_position = position;*/
  /*if (++position_count > 20) {*/
  /*position_count = 0;*/
  /**//*log('pos ' + position);*/
  /*}*/
  /*}*/
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
  $("#btnRefresh").click(function() {
    var select = document.getElementById("port-picker");
    var length = select.options.length;
    for (i = 0; i < length; i++) {
      select.options[i] = null;
    }
    console.log("refresh");
    onload();
  });
  $("#btnOpen").click(function() {
    openSelectedPort();
  });
  $("#btnClose").click(function() {
    closePort();
  });
  $("#btnStart").click(function() {
    log("Start");
  });
  $("#btnStop").click(function() {
    log("Stop");
  });
  $("#btnSingle").click(function() {
    log("Single");
  });
  $("#btnClear").click(function() {
    $("#messagewindow").text("");
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

