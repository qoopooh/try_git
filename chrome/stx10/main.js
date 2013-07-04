var conn_id = 0;
var readBuffSize = 256;
var readBuff = new ArrayBuffer(readBuffSize);
var readArray = new Uint8Array(readBuff);
var readIndex = 0;
var readCount = 0;
var f_openport = false;

function writeArrayBuffer(buf) {
  if (!conn_id) {
    console.log('No connection to write');
    return;
  }
  chrome.serial.write(conn_id, buf, function() {
    document.getElementById('write-info').innerText = ab2hex(buf);
  });
}

/**
 * designed for read 1 by 1
 */
function onRead(readInfo) {
  if (conn_id < 1 || !f_openport) {
    return;
  }
  var uint8View = new Uint8Array(readInfo.data);
  var len = uint8View.length;

  if (!len) {
    console.log('read null', readIndex);
    extractPackage(readArray, readIndex, function() {
      readIndex = 0;
      setTimeout(function() {
        chrome.serial.read(conn_id, 1, onRead);
      }, 200);
    });
    return;
  }
  if (readIndex + len >= readBuffSize) {
    extractPackage(readArray, readIndex, function() {
      readIndex = 0;
    });
  }
  readArray.set(uint8View, readIndex);
  readIndex += len;
  // Keep on reading.
  chrome.serial.read(conn_id, 64, onRead);
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
  disableButton(f_openport);

  setPosition(0);
  readIndex = 0;
  readCount = 0;
  chrome.serial.read(conn_id, 1, onRead);
};

function openSelectedPort() {
  f_openport = true;
  setStatus('Connected');
  log('Opened');
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
  chrome.serial.open(selectedPort, { bitrate: 115200 }, onOpen);
}

function closePort() {
  f_openport = false;
  setStatus('Disconnected');
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
      disableButton(f_openport);
    });
  });
}

function ab2str(buf) {
  return u82str(new Uint8Array(buf));
}

function u82str(uint) {
  return String.fromCharCode.apply(null, uint);
}

function ab2hex(buf) {
  return u82hex(new Uint8Array(buf));
}

function u82hex(arr) {
  var hex = "";
  
  for (var i = 0, len = arr.length; i < len; ++i) {
    if (arr[i] < 16) {
      hex += '0' + arr[i];
    } else {
      hex += arr[i].toString(16).toUpperCase();
    }
  }

  return hex;
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
};

function log(msg) {
  $("#messagewindow").append(timeToString() + ' ' + msg + '<br/>');
  var height = $("#messagewindow")[0].scrollHeight;
  $("#messagewindow").scrollTop(height);
}

function aaelog(msg) {
  var dat = msg.split(':');

  if (dat.length < 2) {
    $("#read-info").text(dat[0]);
  } else {
    $("#read-info").text(dat[1]);
  }
  ++readCount;
  setPosition(readCount);
  document.getElementById('read-count').innerText = readCount.toString();
  log(msg);
}

function resizeMessageWindow() {
  var bodyheight = $(window).height();
  var bodywidth = $(window).width();

  $("#messagewindow").width(bodywidth - 360);
  $("#image").css({ left: bodywidth - 90 });
}

function disableButton(open) {
  $('#btnRefresh').prop('disabled', open);
  $('#btnOpen').prop('disabled', open);
  $('#btnClose').prop('disabled', !open);
  $('#btnStart').prop('disabled', !open);
  $('#btnStop').prop('disabled', !open);
  $('#btnSingle').prop('disabled', !open);
  $('#btnHB').prop('disabled', !open);
  /*$('#readcontrol').prop('disabled', !open); // does not work*/
}

function init() {
  $("#btnRefresh").click(function() {
    var select = document.getElementById("port-picker");
    var len = select.options.length;

    for (i = 0; i < len; i++) {
      select.options[select.selectedIndex] = null;
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
    inventoryCyclic(true, function (buf) {
      writeArrayBuffer(buf);
    });
  });
  $("#btnStop").click(function() {
    log("Stop");
    inventoryCyclic(false, function (buf) {
      writeArrayBuffer(buf);
    });
  });
  $("#btnSingle").click(function() {
    log("Single");
    inventorySingle(function (buf) {
      writeArrayBuffer(buf);
    });
  });
  $("#btnHB").click(function() {
    log("Heartbeat toggle");
    toggleHeartbeat(function (buf) {
      writeArrayBuffer(buf);
    });
  });
  $("#btnClear").click(function() {
    $("#messagewindow").text("");
  });
  $("#port-picker").change(function() {
    closePort();
  });

  $("#messagewindow").html('Start: ' + dateToString() + '<br/>');
  resizeMessageWindow();
}

onload = function() {
  chrome.serial.getPorts(function(ports) {
    buildPortPicker(ports)
    openSelectedPort();
  });
};

$(document).ready(init());
$(window).resize(function() {
  resizeMessageWindow();
});

