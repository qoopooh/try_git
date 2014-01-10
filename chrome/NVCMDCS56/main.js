var conv = new Converter();
var conn_id = -1;

(function() {
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
  log('Refresh ' + portcount + ' port(s)');
}

function onOpen(openInfo) {
  conn_id = openInfo.connectionId;
  if (conn_id < 1) {
    return;
  }
  console.log('Connected', conn_id);
  disableButton(f_openport);

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
  if (!sPortPicker.length) {
    log("there is no port");
    return;
  }
  var selectedPort = sPortPicker.val();
  console.log("selectedPort", selectedPort);

  chrome.serial.open(selectedPort, { bitrate: 9600 }, onOpen);
}

function closePort(cb) {
  f_openport = false;
  process = '';
  log('Closed');
  if (conn_id < 1) {
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

function timeToString(d) {
  var dt = (d) ? d : new Date();

  var hour   = conv.zeroFill(dt.getHours());
  var minute = conv.zeroFill(dt.getMinutes());
  var second = conv.zeroFill(dt.getSeconds());

  return hour + ':' + minute + ':' + second;
}

function dateToString(d) {
  var dt = (d) ? d : new Date();

  var year   = dt.getFullYear();
  var month  = conv.zeroFill(dt.getMonth() + 1);
  var day    = conv.zeroFill(dt.getDate());

  return year + '-' + month + '-' + day + ' ' + timeToString();
}

function log(msg) {
  $("#messagewindow").prepend(timeToString() + ' ' + msg + '<br/>');
}

function aaelog(msg) {
  var dat = msg.split(':');

  ++readCount;
  log(msg);
}

/**
  * Disable open button when the port is open
  */
function disableButton(open) {
  console.log('Elements disableButton', document.getElementsByTagName('*').length);
  if (open) {
    sBtnOpen.button('disable');
    sBtnClose.button('enable');
    sBtnBootloader.button('enable');
  } else {
    sBtnOpen.button('enable');
    sBtnClose.button('disable');
    sBtnBootloader.button('disable');
  }
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
  });
}

function clearData() {
  baginfo = null;
  cabinetinfo = null;
  chrome.storage.sync.set({ 'baginfo': baginfo });
  chrome.storage.sync.set({ 'cabinetinfo': cabinetinfo });
}

function writeToFile(theFileEntry) {
  if (!theFileEntry)
    return;
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

function cancelProcess() {
  regstate = '';
  readtaginfo_state = '';
}

function saveToStorage() {
  chrome.storage.sync.set({ 'baginfo': baginfo });
  chrome.storage.sync.set({ 'cabinetinfo': cabinetinfo });
  console.log('save', baginfo, cabinetinfo);
}

function loadCabinetInput(cb) {
  cabinetinfo.cabinetcode = $("#cabinetcode").val();
  cabinetinfo.cabinetname = $("#cabinetname").val();
  cb();
}

function onLoad() {
  chrome.serial.getPorts(function(ports) {
    buildPortPicker(ports);
    disableButton(false);
  });
};

function init() {
  sBtnOpen = $("#btnOpen");
  sBtnClose = $("#btnClose");
  sBtnBootloader = $("#btnBootloader");
  sBtnClear = $("#btnClear");
  sPortPicker = $("#port-picker");
  onLoad();

  sBtnOpen.click(openSelectedPort);
  sBtnClose.click(closePort);
  sBtnClear.click(function() {
    $("#messagewindow").text("");
  });
  restoreData();
  /*clearData(); // To clear all sync data*/
}
$(document).ready(init()); // for only jQuery
})();

