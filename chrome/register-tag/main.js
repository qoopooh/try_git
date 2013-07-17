var conn_id = 0;
var readBuffSize = 256;
var readBuff = new ArrayBuffer(readBuffSize);
var readArray = new Uint8Array(readBuff);
var readIndex = 0;
var readCount = 0;
var f_openport = false;
var taginfo = {
  productcode: '',
  batchnumber: '',
  mandate: '',
  expdate: '',
  quantity: ''
};
var progress = 0;
var progressmax = 15;
var regstate = '';
var prev_regstate = regstate;
var readtaginfo_state = '';
var regtimeout;
var regtimeoutCount = 0;
var newepc = new Uint8Array(12);
var user = new Uint8Array(10);
var userwriting = new Uint8Array(2);
var userword = 0;
var commandtimeout = 0;
var process = '';

String.prototype.splice = function( idx, rem, s ) {
  return (this.slice(0,idx) + s + this.slice(idx + Math.abs(rem)));
};


function writeArrayBuffer(buf) {
  if (!conn_id) {
    console.log('No connection to write');
    return;
  }
  chrome.serial.write(conn_id, buf, function() {
      /*document.getElementById('write-info').innerText = ab2hex(buf);*/
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

function setStatus(st, cls) {
  $('#status').text(st);
  if (cls)
    document.getElementById("status").className = cls;
  /*else*/
  /*document.getElementById("status").className = 'normal';*/
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
  log('Opened');
  if (conn_id > 0) {
    console.log("openSelectedPort", conn_id);
    return;
  }
  var portPicker = document.getElementById('port-picker');
  console.log("portPicker", portPicker, portPicker.length);
  if (!portPicker.length) {
    log("there is no port");
    return;
  }
  var selectedPort = portPicker.options[portPicker.selectedIndex].value;
  console.log("selectedPort", selectedPort);

  chrome.serial.open(selectedPort, { bitrate: 115200 }, onOpen);
  setStatus('Connected');
}

function closePort(cb) {
  f_openport = false;
  setStatus('Disconnected');
  log('Closed');
  if (conn_id < 1) {
    console.log("closePort", conn_id);
    if (cb) return cb();
    return;
  }
  chrome.serial.flush(conn_id, function() {
    console.log("closing", conn_id);
    chrome.serial.close(conn_id, function() {
      console.log("closed", conn_id);
      conn_id = 0;
      disableButton(f_openport);
      if (cb) return cb();
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
      hex += '0' + arr[i].toString(16).toUpperCase();
    } else {
      hex += arr[i].toString(16).toUpperCase();
    }
  }

  return hex;
}

function zeroPad(number, d) {
  if (!d)
    d = 2;
  if (d === 2) {
    return (number < 10) ? '0' + number : number;
  } else if (d === 3) {
    if (number < 10)
      return '00' + number;
    else if (number < 100)
      return '0' + number;
    else
      return number;
  } else if (d === 4) {
    if (number < 10)
      return '000' + number;
    else if (number < 100)
      return '00' + number;
    else if (number < 1000)
      return '0' + number;
    else
      return number;
  }
  
  return number;
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
};

function log(msg) {
  $("#messagewindow").prepend(timeToString() + ' ' + msg + '<br/>');
  /*var height = $("#messagewindow")[0].scrollHeight;*/
  /*$("#messagewindow").scrollTop(height);*/
}

function aaelog(msg) {
  var dat = msg.split(':');

  /*if (dat.length > 1) {*/
  /*$("#read-info").text(dat[1]);*/
  /*}*/
  ++readCount;
  setPosition(readCount);
  /*document.getElementById('read-count').innerText = readCount.toString();*/
  log(msg);
}

function resizeMessageWindow() {
  var bodyheight = $(window).height();
  var bodywidth = $(window).width();

  /*$("#messagewindow").width(bodywidth - 360);*/
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

function verifyForm(cb) {
  var res = false;
  var val = $("#productcode").val();
  var len = val.length;
  var patt = /\d{2}\.\d{3}\.\d{3}/g;

  if (len > 10)
    patt = /\d{2}\.\d{3}\.\d{3}\.\d{1}/g;
  res = patt.test(val);
  if (!res) {
    setStatus("Product code format failed (e.g. 00.000.000.0)", "fail");
    return res;
  }
  taginfo.productcode = val;
  updateProgress();
  val = $("#batchnumber").val();
  patt = /^[A-Z]+\d{3}\/\d{2}/g;
  res = patt.test(val);
  if (!res) {
    setStatus("Batch number format failed (e.g. ESE001/13)", "fail");
    return res;
  }
  taginfo.batchnumber = val;
  updateProgress();
  val = $("#mandate").val();
  /*log(val + ' ' + val.length);*/
  patt = /\d{2}\/\d{2}/g;
  res = patt.test(val);
  if (!res) {
    setStatus("Manufacture date format failed (e.g. 01/13)", "fail");
    return res;
  }
  taginfo.mandate = val;
  updateProgress();
  val = $("#expdate").val();
  patt = /\d{2}\/\d{2}/g;
  res = patt.test(val);
  if (!res) {
    setStatus("Expiration date format failed (e.g. 01/14)", "fail");
    return res;
  }
  taginfo.expdate = val;
  updateProgress();
  val = $("#quantity").val();
  patt = /\d{4},\d{3}\w/g;
  res = patt.test(val);
  if (!res) {
    setStatus("Quantity format failed (e.g. 9999,999p)", "fail");
    return res;
  }
  taginfo.quantity = val;
  updateProgress();
  if (res)
    setStatus("Verified", "ok");
  return res;
}

function register() {
  var state = regstate;
  var tempstate = state;

  switch (state) {
    case 'genepc':
      setStatus("Generate EPC");
      genepc(function () {
        var hex = u82hex(newepc);
        console.log('genepc done', hex);
        $("#epc").text(hex);
        updateProgress();
        state = 'genuser';
      });
      break;
    case 'genuser':
      setStatus("Generate USER memory");
      genuser(function () {
        var hex = u82hex(user);
        console.log('genuser done', hex);
        $("#user").text(hex);
        updateProgress();
        state = 'openport';
      });
      break;
    case 'openport':
      setStatus("Open " + $("#port-picker").val());
      openSelectedPort();
      updateProgress();
      state = 'waitport';
      break;
    case 'waitport':
      if (conn_id < 1)
        break;
      setStatus('Set Heartbeat');
      setHeartbeat(true, function (buf) {
        writeArrayBuffer(buf);
        updateProgress();
        state = 'waithb';
      });
      break;
    case 'single':
      commandtimeout = 20;
      inventorySingle(function (buf) {
        writeArrayBuffer(buf);
        f_single = false;
        state = 'waithb';
      });
      break;
    case 'writeEpc':
      writeEpc(epc, new Uint8Array([ 0x00, 0x00, 0x00, 0x00 ]),
          newepc, function (buf) {
        writeArrayBuffer(buf);
        console.log(state, buf);
        state = 'waithb';
      });
      break;
    case 'writeUser':
      userwriting = user.subarray(userword * 2, userword * 2 + 2);
      writeToTag(epc, 3, userword, null, userwriting, function (buf) {
        writeArrayBuffer(buf);
        console.log(state, buf);
        state = 'waitwrite';
      });
      break;
    case 'waitwrite':
      if (!f_written)
        break;
      if (f_success) {
        state = 'readUser';
      } else {
        state = 'writeUser';
      }
      break;
    case 'readUser':
      readFromTag(epc, 3, userword, null, 2, function (buf) {
        writeArrayBuffer(buf);
        console.log(state, buf);
        state = 'waitread';
      });
      break;
    case 'waitread':
      if (!f_readtag)
        break;
      if (!readdata) {
        state = 'writeUser';
        break;
      }
      if (userwriting[0] === readdata[0]
          && userwriting[1] === readdata[1]) {
        updateProgress();
        ++userword;
        if (userword < 5) {
          state = 'writeUser';
        } else {
          closePort(function () {
            setStatus('Finished');
            log('Succeed');
            process = '';
          });
        }
      }
      break;
    case 'waithb':
      if (!f_hb_read)
        break;
      f_hb_read = false;
      log("f_hb_read " + prev_regstate);
      switch (prev_regstate) {
        case 'waitport':
          setStatus('Read EPC');
          state = 'single';
          break;
        case 'single':
          if (!f_single)
            break;
          if (!epc || epc.length < 1) {
            state = 'single';
          } else {
            setStatus('Write EPC');
            state = 'writeEpc';
          }
          break;
        case 'writeEpc':
          if (!f_written)
            break;
          if (f_success) {
            epc = newepc;
            userword = 0;
            updateProgress();
            state = 'writeUser';
            setStatus('Write USER');
          } else {
            state = 'single';
          }
          break;
        case 'readUser':
          break;
        case 'writeUser':
          break;
        default:
          state = 'noop';
      }
      break;
    case 'noop':
    default:
      log('No operation');
      closePort();
      process = '';
      console.log(taginfo);
      break;
  }
  if (tempstate !== state)
    prev_regstate = tempstate;
  regstate = state;
}

function genepc(cb) {
  var batch = taginfo.batchnumber;
  var code = taginfo.productcode.split('.');
  var i = 0;
  var j = 0;
  
  newepc[0] = batch.charCodeAt(j);
  if (isNumChar(batch.charCodeAt(++j))) {
    newepc[1] = 0;
    newepc[2] = 0;
  } else {
    newepc[1] = batch.charCodeAt(j);
    if (isNumChar(batch.charCodeAt(++j)))
      newepc[2] = 0;
    else
      newepc[2] = batch.charCodeAt(j++);
  }
  i = parseInt(batch.substr(j, 3));
  newepc[3] = (i >> 8) & 0xFF;
  newepc[4] = i & 0xFF;
  i = parseInt(batch.substr(j += 4));
  newepc[5] = i & 0xFF;

  newepc[6] = parseInt(code[0]);
  i = parseInt(code[1]);
  newepc[7] = (i >> 8) & 0xFF;
  newepc[8] = i & 0xFF;
  i = parseInt(code[2]);
  newepc[9] = (i >> 8) & 0xFF;
  newepc[10] = i & 0xFF;
  if (code.length < 4)
    newepc[11] = 0xFF;
  else
    newepc[11] = parseInt(code[3]);

  cb();
}

function genuser(cb) {
  var date = taginfo.mandate.split('/');
  var quantity = taginfo.quantity.split(',');
  var i = 0;
  
  user[0] = parseInt(date[0]);
  user[1] = parseInt(date[1]);
  date = taginfo.expdate.split('/');
  user[2] = parseInt(date[0]);
  user[3] = parseInt(date[1]);
  i = parseInt(quantity[0]);
  user[4] = (i >> 8) & 0xFF;
  user[5] = i & 0xFF;
  i = parseInt(quantity[1]);
  user[6] = (i >> 8) & 0xFF;
  user[7] = i & 0xFF;
  user[8] = quantity[1].charCodeAt(3);

  cb();
}

function isMaxLength(o, len) {
  return (o.val().length >= len);
}

function isNumChar(c) {
  return (c >= 0x30 && c <= 0x39);
}

function isBatchLetter(c) {
  if (c === 0x41 || c === 0x42
      || c === 0x45 || c === 0x4C
      || c === 0x4D || c === 0x50
      || c === 0x53 || c === 0x54
     )
    return true;
  return false;
}

function isUnit(c) {
  if (c === 0x6b || c === 0x70
      || c === 0x75
     )
    return true;
  return false;
}

function isDateFormat(o, e) {
  if (isMaxLength(o, 5))
    return false;
  var c = (e.which) ? e.which : e.keyCode;
  if (!isNumChar(c)
    && c !== 0x2F // '/'
    )
    return false;
  return true;
}

function submit(c) {
  if (c === 13) {
    $("#btnSubmit").click();
    return true;
  }
  return false;
}

function updateProgress() {
  $("#progress").val(++progress);
  var percent = progress * 100 / progressmax;
  if (percent > 100) {
    $("#percent").text(progress);
  } else {
    percent = percent.toFixed(2);
    $("#percent").text('' + percent + ' %');
  }
}

function readTagInfo() {
  var state = readtaginfo_state;
  var tempstate = state;

  switch (state) {
    case 'openport':
      setStatus("Open " + $("#port-picker").val());
      openSelectedPort();
      state = 'waitport';
      break;
    case 'waitport':
      if (conn_id < 1)
        break;
      state = 'single';
      break;
    case 'single':
      commandtimeout = 10;
      inventorySingle(function (buf) {
        writeArrayBuffer(buf);
        state = 'waitepc';
      });
      break;
    case 'waitepc':
      if (!epc || epc.length < 1) {
        if (!commandtimeout)
          state = 'single';
        break;
      }
      extractEpcInfo();
      state = 'readUser';
      break;
    case 'readUser':
      commandtimeout = 10;
      readFromTag(epc, 3, null, null, 10, function (buf) {
        writeArrayBuffer(buf);
        console.log(state, buf);
        state = 'waitread';
      });
      break;
    case 'waitread':
      if (!readdata) {
        if (!commandtimeout)
          state = 'readUser';
        break;
      }
      extractUserInfo();
      state = 'noop';
      break;
    default:
      closePort();
      process = '';
      break;
  }
  if (tempstate !== state)
    prev_regstate = tempstate;
  readtaginfo_state = state;
}

function extractEpcInfo() {
  var batch = '';
  var code = '';
  var i = 0;

  batch = String.fromCharCode(epc[0]);
  if (epc[1])
    batch += String.fromCharCode(epc[1]);
  if (epc[2])
    batch += String.fromCharCode(epc[2]);
  i = ((epc[3] << 8) + epc[4]) & 0x03FF;
  batch += zeroPad(i, 3) + '/' + zeroPad(epc[5]);

  i = ((epc[7] << 8) + epc[8]) & 0x03FF;
  code = zeroPad(epc[6]) + '.' + zeroPad(i, 3) + '.';
  i = ((epc[9] << 8) + epc[10]) & 0x03FF;
  code += zeroPad(i, 3);
  if (epc[11] !== 255)
  code += '.' + epc[11];

  log('Product code: ' + code);
  log('Batch: ' + batch);
}

function extractUserInfo() {
  var mandate = '';
  var expdate = '';
  var quantity = '';
  var i = 0;

  mandate = zeroPad(readdata[0]) + '/' + zeroPad(readdata[1]);
  log('Manufacture date: ' + mandate);
  expdate = zeroPad(readdata[2]) + '/' + zeroPad(readdata[3]);
  log('Expiration date: ' + expdate);
  i = ((readdata[4] << 8) + readdata[5]) & 0x3FFF;
  quantity = zeroPad(i, 4) + ',';
  i = ((readdata[6] << 8) + readdata[7]) & 0x3FFF;
  quantity += zeroPad(i, 3) + String.fromCharCode(readdata[8]);
  log('Quantity: ' + quantity);
}

setInterval(function() {
  switch (process) {
    case 'register':
      register();
      break;
    case 'readTagInfo':
      readTagInfo();
      break;
    default:
      break;
  }
  if (commandtimeout)
    --commandtimeout;
}, 100);

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
  $("#productcode").keypress(function (e) {
    var c = (e.which) ? e.which : e.keyCode;
    if (submit(c)) {
      e.preventDefault();
      return;
    }
    if (isMaxLength($(this), 12))
      return false;
    if (!isNumChar(c)
      && c !== 0x2E // '.'
      )
      return false;
  });
  $("#batchnumber").keypress(function (e) {
    var c = (e.which) ? e.which : e.keyCode;
    if (submit(c)) {
      e.preventDefault();
      return;
    }
    if (isMaxLength($(this), 9))
      return false;
    if (!isNumChar(c)
      && !isBatchLetter(c)
      && c !== 0x2F // '/'
      )
      return false;
  });
  $("#mandate").keypress(function (e) {
    var c = (e.which) ? e.which : e.keyCode;
    if (submit(c)) {
      e.preventDefault();
      return;
    }
    return isDateFormat($(this), e);
  });
  $("#expdate").keypress(function (e) {
    var c = (e.which) ? e.which : e.keyCode;
    if (submit(c)) {
      e.preventDefault();
      return;
    }
    return isDateFormat($(this), e);
  });
  $("#quantity").keypress(function (e) {
    var c = (e.which) ? e.which : e.keyCode;
    if (submit(c)) {
      e.preventDefault();
      return;
    }
    if (isMaxLength($(this), 9))
      return false;
    if (!isNumChar(c)
      && !isUnit(c)
      && c !== 0x2C // ','
      )
      return false;
  });
  $("#btnSubmit").click(function() {
    progress = 0;
    if (verifyForm()) {
      process = 'register';
      regstate = 'genepc';
    }
  });
  $("#btnReadTag").click(function() {
    process = 'readTagInfo';
    readtaginfo_state = 'openport';
  });
  $("#progress").val(0);
  document.getElementById('progress').setAttribute('max', '' + progressmax);
  $("#port-picker").change(function() {
    closePort();
  });

  $("#messagewindow").html('Start: ' + dateToString() + '<br/>');
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

