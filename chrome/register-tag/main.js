var conn_id = 0;
var readBuffSize = 256;
var readBuff = new ArrayBuffer(readBuffSize);
var readArray = new Uint8Array(readBuff);
var readIndex = 0;
var readCount = 0;
var f_openport = false;
var baginfo = {
  type: '',
  productcode: '',
  batchnumber: '',
  mandate: '',
  expdate: '',
  quantity: '',
  qc: false,
  cabinetcode: '',
  cabinetname: '',
};
var cabinetinfo = {
  type: '',
  cabinetcode: '',
  cabinetname: '',
};
var tagarr = [];
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
var sBtnSave = null;
var sBtnOpen = null;
var sBtnClose = null;
var sBtnStart = null;
var sBtnStop = null;
var sBtnSingle = null;
var sBtnHB = null;
var sPortPicker = null;

String.prototype.splice = function( idx, rem, s ) {
  return (this.slice(0,idx) + s + this.slice(idx + Math.abs(rem)));
};

function errorHandler(e) {
  console.log('error', e.toString());
}

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

function setStatus(msg, cls) {
  var status1 = document.getElementById("status");
  var status2 = document.getElementById("status2");

  status1.innerText = msg;
  status2.innerText = msg;
  if (cls) {
    status1.className = cls;
    status2.className = cls;
  }
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
  portPicker.selectedIndex = portcount;
  sPortPicker.selectmenu('refresh', true);
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
  /*var portPicker = document.getElementById('port-picker');*/
  /*console.log("portPicker", sPortPicker, sPortPicker.length);*/
  if (!sPortPicker.length) {
    log("there is no port");
    return;
  }
  /*var selectedPort = portPicker.options[portPicker.selectedIndex].value;*/
  var selectedPort = sPortPicker.val();
  console.log("selectedPort", selectedPort);

  chrome.serial.open(selectedPort, { bitrate: 115200 }, onOpen);
  setStatus('Connected ' + selectedPort);
}

function closePort(cb) {
  f_openport = false;
  process = '';
  setStatus('Disconnected');
  log('Closed');
  if (conn_id < 1) {
    console.log("closePort", conn_id);
    disableButton(f_openport);
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

/**
  * Disable open button when the port is open
  */
function disableButton(open) {
  console.log('Elements disableButton', document.getElementsByTagName('*').length );
  if (open) {
    /*$('#btnRefresh').button('disable');*/
    sBtnOpen.button('disable');
    sBtnClose.button('enable');
    sBtnStart.button('enable');
    sBtnStop.button('enable');
    sBtnSingle.button('enable');
    sBtnHB.button('enable');
  } else {
    /*$('#btnRefresh').button('enable');*/
    sBtnOpen.button('enable');
    sBtnClose.button('disable');
    sBtnStart.button('disable');
    sBtnStop.button('disable');
    sBtnSingle.button('disable');
    sBtnHB.button('disable');
  }
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
  baginfo.productcode = val;
  updateProgress();
  val = $("#batchnumber").val();
  patt = /^[A-Z]+\d{3}\/\d{2}/g;
  res = patt.test(val);
  if (!res) {
    setStatus("Batch number format failed (e.g. ESE001/13)", "fail");
    return res;
  }
  baginfo.batchnumber = val;
  updateProgress();
  val = $("#mandate").val();
  /*log(val + ' ' + val.length);*/
  patt = /\d{2}\/\d{2}/g;
  res = patt.test(val);
  if (!res) {
    setStatus("Manufacture date format failed (e.g. 01/13)", "fail");
    return res;
  }
  baginfo.mandate = val;
  updateProgress();
  val = $("#expdate").val();
  patt = /\d{2}\/\d{2}/g;
  res = patt.test(val);
  if (!res) {
    setStatus("Expiration date format failed (e.g. 01/14)", "fail");
    return res;
  }
  baginfo.expdate = val;
  updateProgress();
  val = $("#quantity").val();
  patt = /\d{4},\d{3}\w/g;
  res = patt.test(val);
  if (!res) {
    setStatus("Quantity format failed (e.g. 9999,999p)", "fail");
    return res;
  }
  baginfo.quantity = val;
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
      setStatus("Open " + sPortPicker.val());
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
      var w = userword * 2;
      userwriting = user.subarray(w, w + 2);
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
      console.log(baginfo);
      break;
  }
  if (tempstate !== state)
    prev_regstate = tempstate;
  regstate = state;
}

function genepc(cb) {
  var batch = baginfo.batchnumber;
  var code = baginfo.productcode.split('.');
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
  var date = baginfo.mandate.split('/');
  var quantity = baginfo.quantity.split(',');
  var i = 0;
  
  user[0] = parseInt(date[0]);
  user[1] = parseInt(date[1]);
  date = baginfo.expdate.split('/');
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

function isUpperCase(c) {
  return (c >= 0x41 && c <= 0x5A);
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
      setStatus("Open " + sPortPicker.val());
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
      extractUserInfo(collectReadbag);
      closePort(function() {
        setStatus(baginfo.type + ' '
          + baginfo.productcode + ' '
          + baginfo.batchnumber + ' '
          + baginfo.quantity + ' qc:'
          + baginfo.qc);
        process = '';
      });
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

function readCabinet() {
  var state = readtaginfo_state;
  var tempstate = state;

  switch (state) {
    case 'openport':
      setStatus("Open " + sPortPicker.val());
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
      extractEpcInfo(collectReadcabinet);
      closePort(function() {
        setStatus(cabinetinfo.type + ' '
          + cabinetinfo.cabinetcode + ' '
          + cabinetinfo.cabinetname + ' '
          );
        process = '';
      });
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

function extractEpcInfo(cb) {
  var batch = '';
  var code = '';
  var type = '';
  var qc = false;
  var i = 0;

  /*Byte 7:*/
  /*  bit 7: 1 is Bag, 0 is Cabinet*/
  /*  bit 6: 1 is QC passed*/
  i = epc[7];
  if (i & 0x80)
    type = 'cabinet';
  else
    type = 'bag';
  if (type === 'bag') {
    if (i & 0x40)
      qc = true;
    else
      qc = false;

    batch = String.fromCharCode(epc[0]);
    if (epc[1])
      batch += String.fromCharCode(epc[1]);
    if (epc[2])
      batch += String.fromCharCode(epc[2]);
    i = ((epc[3] << 8) + epc[4]) & 0x03FF;
    batch += zeroPad(i, 3) + '/' + zeroPad(epc[5]);
  } else {
    batch = String.fromCharCode(epc[0]);
    if (epc[1])
      batch += String.fromCharCode(epc[1]);
    i = ((epc[2] << 8) + epc[3]) & 0x03FF;
    batch += zeroPad(i, 4);
  }


  i = (((epc[7] & 0x03) << 8) + epc[8]) & 0x03FF;
  code = zeroPad(epc[6]) + '.' + zeroPad(i, 3) + '.';
  i = ((epc[9] << 8) + epc[10]) & 0x03FF;
  code += zeroPad(i, 3);
  if (epc[11] !== 255)
  code += '.' + epc[11];

  log('Product code: ' + code);
  log('Batch: ' + batch);
  if (type === 'bag') {
    baginfo.productcode = code;
    baginfo.batchnumber = batch;
    baginfo.type = type;
    baginfo.qc = qc;
  } else {
    cabinetinfo.cabinetcode = code;
    cabinetinfo.cabinetname = batch;
    cabinetinfo.type = type;
  }

  if (cb)
    cb();
}

function extractUserInfo(cb) {
  var mandate = '';
  var expdate = '';
  var quantity = '';
  var i = 0;

  mandate = zeroPad(readdata[0]) + '/' + zeroPad(readdata[1]);
  expdate = zeroPad(readdata[2]) + '/' + zeroPad(readdata[3]);
  i = ((readdata[4] << 8) + readdata[5]) & 0x3FFF;
  quantity = zeroPad(i, 4) + ',';
  i = ((readdata[6] << 8) + readdata[7]) & 0x3FFF;
  quantity += zeroPad(i, 3) + String.fromCharCode(readdata[8]);
  log('Manufacture date: ' + mandate);
  log('Expiration date: ' + expdate);
  log('Quantity: ' + quantity);
  baginfo.mandate = mandate;
  baginfo.expdate = expdate;
  baginfo.quantity = quantity;
  cb();
}

function refreshPort() {
  var select = document.getElementById("port-picker");
  var len = select.options.length;

  for (i = len - 1; i > 0; i--) {
    select.options[i] = null;
  }
  console.log("refresh");
  onLoad();
}

function restoreData() {
  chrome.storage.sync.get(function(items) {
    baginfo = items.baginfo;
    if (!baginfo) {
      baginfo = {
        type: 'bag', /* 'bag' or 'cabinet' */
        productcode: '00.000.000',
        batchnumber: 'M001/13',
        mandate: '03/13',
        expdate: '03/14',
        quantity: '9999,999u',
        qc: false, /* passed is true */
        cabinetcode: '',
        cabinetname: ''
      };
    }
    $("#productcode").val(baginfo.productcode);
    $('input[id=batchnumber]').val(baginfo.batchnumber);
    $("#mandate").val(baginfo.mandate);
    $("#expdate").val(baginfo.expdate);
    $("#quantity").val(baginfo.quantity);
    console.log('baginfo:', baginfo);
    cabinetinfo = items.cabinetinfo;
    if (!cabinetinfo) {
      cabinetinfo = {
        type: 'cabinet',
        cabinetcode: '00.000.000',
        cabinetname: 'AB0001'
      };
    }
    $("#cabinetcode").val(cabinetinfo.cabinetcode);
    $("#cabinetname").val(cabinetinfo.cabinetname);
    console.log('cabinetinfo:', cabinetinfo);
  });
}

function clearData() {
  baginfo = null;
  cabinetinfo = null;
  chrome.storage.sync.set({ 'baginfo': baginfo });
  chrome.storage.sync.set({ 'cabinetinfo': cabinetinfo });
}

function writeToFile(theFileEntry) {
  theFileEntry.createWriter(function(fileWriter) {
    fileWriter.onerror = function(e) {
      console.log("Write failed: " + e.toString());
    };

    var res = JSON.stringify(tagarr, undefined, 2);
    var blob = new Blob([res]);
    fileWriter.truncate(blob.size);
    fileWriter.onwriteend = function() {
      fileWriter.onwriteend = function(e) {
        console.log("Write " + theFileEntry.name + " completed.");
      };

      fileWriter.write(blob);
    }
    console.log('writeToFile', res, theFileEntry);
  }, errorHandler);
}

function openFile(theFileEntry) {
  if (theFileEntry) {
    theFileEntry.file(function(file) {
      var fileReader = new FileReader();

      fileReader.onload = function(e) {
        console.log(theFileEntry.name);
      };

      fileReader.onerror = function(e) {
        errorHandler(e);
      };

      fileReader.onloadend = function(e) {
        var arr = JSON.parse(fileReader.result);
        console.log(arr);
      };

      fileReader.readAsText(file);
    }, errorHandler);
  }
}

function handleExportButton() {
  chrome.fileSystem.chooseEntry(
      { type: 'saveFile',
        accepts: [{ extensions: ['json'] }] 
      }, writeToFile);
}

function handleOpenJson() {
  chrome.fileSystem.chooseEntry(
      { type: 'openFile',
        accepts: [{ extensions: ['json'] }] 
      }, openFile);
}

function collectReadbag() {
  var tag = {};

  for (var i = 0, len = tagarr.length; i < len; ++i) {
    tag = tagarr[i];

    if (tag.type !== 'bag')
      continue;
    if ((tag.productcode === baginfo.productcode)
      && (tag.batchnumber === baginfo.batchnumber))
      return;
  }
  tagarr.push(jQuery.extend({}, baginfo)); // copy new object
  console.log('collectReadbag', tagarr);
}

function collectReadcabinet() {
  var tag = {};

  for (var i = 0, len = tagarr.length; i < len; ++i) {
    tag = tagarr[i];

    if (tag.type !== 'cabinet')
      continue;
    if ((tag.cabinetcode === cabinetinfo.cabinetcode)
      && (tag.cabinetname === cabinetinfo.cabinetname))
      return;
  }
  tagarr.push(jQuery.extend({}, cabinetinfo)); // copy new object
  console.log('collectReadcabinet', tagarr);
}

function saveToStorage() {
  chrome.storage.sync.set({ 'baginfo': baginfo });
  chrome.storage.sync.set({ 'cabinetinfo': cabinetinfo });
  console.log('save', baginfo, cabinetinfo);
}

function onLoad() {
  chrome.serial.getPorts(function(ports) {
    buildPortPicker(ports)
    /*openSelectedPort();*/
    disableButton(false);
  });
};

setInterval(function() {
  switch (process) {
    case 'register':
      register();
      break;
    case 'readTagInfo':
      readTagInfo();
      break;
    case 'writCabinet':
      writCabinet();
      break;
    case 'readCabinet':
      readCabinet();
      break;
    default:
      break;
  }
  if (commandtimeout)
    --commandtimeout;
}, 100);

function init() {
  sBtnSave = $("#btnSave");
  sBtnOpen = $("#btnOpen");
  sBtnClose = $("#btnClose");
  sBtnStart = $("#btnStart");
  sBtnStop = $("#btnStop");
  sBtnSingle = $("#btnSingle");
  sBtnHB = $("#btnHB");
  sPortPicker = $("#port-picker");
  /*$("#btnRefresh").click(refreshPort());*/
  sBtnSave.click(saveToStorage);
  $("#btnCabinetSave").click(saveToStorage);
  sBtnOpen.click(openSelectedPort);
  sBtnClose.click(function() {
    closePort();
  });
  sBtnStart.click(function() {
    log("Start");
    inventoryCyclic(true, function (buf) {
      writeArrayBuffer(buf);
    });
  });
  sBtnStop.click(function() {
    log("Stop");
    inventoryCyclic(false, function (buf) {
      writeArrayBuffer(buf);
    });
  });
  sBtnSingle.click(function() {
    log("Single");
    inventorySingle(function (buf) {
      writeArrayBuffer(buf);
    });
  });
  sBtnHB.click(function() {
    log("Heartbeat toggle");
    toggleHeartbeat(function (buf) {
      writeArrayBuffer(buf);
    });
  });
  $("#btnClear").click(function() {
    $("#messagewindow").text("");
  });
  sPortPicker.change(function() {
    closePort();
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
  $("#productcode").change(function () {
    baginfo.productcode = $(this).val();
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
  $("#batchnumber").change(function () {
    baginfo.batchnumber = $(this).val();
  });
  $("#mandate").keypress(function (e) {
    var c = (e.which) ? e.which : e.keyCode;
    if (submit(c)) {
      e.preventDefault();
      return;
    }
    return isDateFormat($(this), e);
  });
  $("#mandate").change(function () {
    baginfo.mandate = $(this).val();
  });
  $("#expdate").keypress(function (e) {
    var c = (e.which) ? e.which : e.keyCode;
    if (submit(c)) {
      e.preventDefault();
      return;
    }
    baginfo.expdate = $(this).val();
    return isDateFormat($(this), e);
  });
  $("#expdate").change(function () {
    baginfo.expdate = $(this).val();
  });
  $("#quantity").keypress(function (e) {
    var c = (e.which) ? e.which : e.keyCode;
    if (submit(c)) {
      e.preventDefault();
      return;
    }
    baginfo.quantity = $(this).val();
    if (isMaxLength($(this), 9))
      return false;
    if (!isNumChar(c)
      && !isUnit(c)
      && c !== 0x2C // ','
      )
      return false;
  });
  $("#quantity").change(function () {
    baginfo.quantity = $(this).val();
  });
  $("#cabinetcode").keypress(function (e) {
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
  $("#cabinetcode").change(function () {
    cabinetinfo.cabinetcode = $(this).val();
  });
  $("#cabinetname").keypress(function (e) {
    var c = (e.which) ? e.which : e.keyCode;
    if (submit(c)) {
      e.preventDefault();
      return;
    }
    if (isMaxLength($(this), 6))
      return false;
    if (!isNumChar(c)
      && !isUpperCase(c)
      )
      return false;
  });
  $("#cabinetname").change(function () {
    cabinetinfo.cabinetname = $(this).val();
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
  $("#btnBagCancel").click(function() {
    regstate = '';
    readtaginfo_state = '';
  });
  $("#btnExportBag").click(handleExportButton);
  $("#btnOpenJson").click(handleOpenJson);
  $("#progress").val(0);
  document.getElementById('progress').setAttribute('max', '' + progressmax);

  $("#messagewindow").html('Start: ' + dateToString() + '<br/>');
  resizeMessageWindow();

  $("#debug").off("pageinit").on("pageinit", function(e) {
    $.mobile.changePage( $("#main"), { transition: "none", reverse: false,
      changeHash: false });
  });


  onLoad();

  restoreData();
  /*clearData();*/
}

/*$(document).bind('pageinit', function() {*/
/*$(document).bind('mobileinit', function() {*/
/*init();*/
/*});*/
$(document).ready(init()); // for only jQuery
$(window).resize(function() {
  resizeMessageWindow();
});

