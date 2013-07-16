var reloadButton, openButton, increaseButton;
var editor;
var fileEntry;
var hexFileEntry;
var hexchange;
var hasWriteAccess;
var sn = 0;
var res = "";
var LOT = /\#define\s+PROD_LOT\s+\b0x..*/g;
var SNRH = /\#define\s+SNRH\s+\b0x..*/g;
var SNRL = /\#define\s+SNRL\s+\b0x..*/g;
var LOT_NEW  = "#define PROD_LOT 0x";
var SNRH_NEW = "#define SNRH     0x";
var SNRL_NEW = "#define SNRL     0x";
var HEX = /\:08040000....0000../g;
var timeout;

function errorHandler(e) {
  var msg = "";

  switch (e.code) {
    case FileError.QUOTA_EXCEEDED_ERR:
    msg = "QUOTA_EXCEEDED_ERR";
    break;
    case FileError.NOT_FOUND_ERR:
    msg = "NOT_FOUND_ERR";
    break;
    case FileError.SECURITY_ERR:
    msg = "SECURITY_ERR";
    break;
    case FileError.INVALID_MODIFICATION_ERR:
    msg = "INVALID_MODIFICATION_ERR";
    break;
    case FileError.INVALID_STATE_ERR:
    msg = "INVALID_STATE_ERR";
    break;
    default:
    msg = "Unknown Error";
    break;
  };

  setStatus("Error: " + msg);
}

function setStatus(title) {
  if (!title)
    document.getElementById("title").innerHTML = "[no document loaded]";
  else
    document.getElementById("title").innerHTML = title;
  console.log(title);
}

function showSerial(serial) {
  document.getElementById("serial").innerHTML = i2hex(serial);
}

function newFile() {
  fileEntry = null;
  hexFileEntry = null;
  hasWriteAccess = false;
  setStatus(null);
}

function setFile(theFileEntry, isWritable) {
  fileEntry = theFileEntry;
  hasWriteAccess = isWritable;
  console.log(fileEntry, hasWriteAccess);
}

function readFileIntoEditor(theFileEntry) {
  if (theFileEntry) {
    theFileEntry.file(function(file) {
      var fileReader = new FileReader();

      fileReader.onload = function(e) {
        setStatus(theFileEntry.name);
      };

      fileReader.onerror = function(e) {
        setStatus("Read failed: " + e.toString());
      };

      fileReader.onloadend = function(e) {
        res = fileReader.result;
        openHexButton.disabled = false;
      };

      fileReader.readAsText(file);
    }, errorHandler);
  }
}

function loadDef(res) {
  var serial = 0;
  var match = res.match(LOT);
  if (!match) {
    setStatus("cannot find PROD_LOT");
    return;
  }
  var n = match[0]; 
  n = n.replace(/\s+/g, ' ');
  var lot = n.split(" ")[2];
  n = res.match(SNRH)[0];
  if (!n) {
    setStatus("cannot find SNRH");
    return;
  }
  n = n.replace(/\s+/g, ' ');
  var snrh = n.split(" ")[2];
  n = res.match(SNRL)[0];
  if (!n) {
    setStatus("cannot find SNRL");
    return;
  }
  n = n.replace(/\s+/g, ' ');
  var snrl = n.split(" ")[2];

  serial = lot << 16;
  serial += snrh << 8;
  serial += snrl & 0xFF;
  console.log('onloadend', serial, lot, snrh, snrl);
  showSerial(serial);
  disableManualButtons(false);
  /*setHexMenu();*/
}

function loadHex(res, cb) {
  var serial = 0;
  var match = res.match(HEX);
  if (!match) {
    setStatus("cannot find HEX");
    return;
  }
  var lot = match[0].substr(17, 2);
  var snrh = match[0].substr(11, 2);
  var snrl = match[0].substr(9, 2);

  serial = parseInt(lot, 16) << 16;
  serial += parseInt(snrh, 16) << 8;
  serial += parseInt(snrl, 16) & 0xFF;
  console.log('onloadend', serial, lot, snrh, snrl);
  showSerial(serial);
  disableManualButtons(false);
  if (cb)
    cb(serial);
}

function writeEditorToFile(theFileEntry) {
  ++sn;

  res = res.replace(LOT, LOT_NEW + i2hex((sn >> 16) & 0xFF));
  res = res.replace(SNRH, SNRH_NEW + i2hex((sn >> 8) & 0xFF));
  res = res.replace(SNRL, SNRL_NEW + i2hex(sn & 0xFF));
  /*console.log(res);*/
  setStatus(theFileEntry);
  theFileEntry.createWriter(function(fileWriter) {
    fileWriter.onerror = function(e) {
      setStatus("Write failed: " + e.toString());
    };

    var blob = new Blob( [res], {type:'text/plain'});
    fileWriter.truncate(blob.size);
    fileWriter.onwriteend = function() {
      fileWriter.onwriteend = function(e) {
        setStatus("Write " + theFileEntry.name + " completed.");
      };

      fileWriter.write(blob);
    }
  }, errorHandler);
  showSerial(sn);
}

var onWritableFileToOpen = function(theFileEntry) {
  if (!theFileEntry) {
    return;
  }

  setFile(theFileEntry, true);
  readFileIntoEditor(theFileEntry);
};

function handleReloadButton() {
  /*onWritableFileToOpen(fileEntry);*/
  checkHex(hexFileEntry, function(serial) {
    console.log(serial, sn);
    if (serial < sn) {
      sn = serial + 1;
      showSerial(sn);
      return;
    }
    /*if (serial > sn)*/
      handleIncreaseButton();
  });
}

function handleOpenButton() {
  chrome.fileSystem.chooseEntry(
      { type: 'openWritableFile',
        accepts: [{ extensions: ['h'] }] 
      }, onWritableFileToOpen);
}

function handleOpenHexButton() {
  /*chrome.fileSystem.chooseEntry(*/
  /*{ type: 'openFile',*/
  /*accepts: [{ extensions: ['hex'] }] */
  /*}, onReadableFileToOpen);*/
  handleHexFile(true);
}

function handleIncreaseButton() {
  /*if (fileEntry && hasWriteAccess) {*/
  writeEditorToFile(fileEntry);
  /*}*/
}

function handleHexFile(open) {
  if (open) {
    chrome.fileSystem.chooseEntry(
    { type: 'openFile', accepts: [{ extensions: ['hex'] }] }, function(fileentry) {
      if (!fileentry)
        return;
      hexFileEntry = fileentry;
      console.log('hex entry', hexFileEntry);
      checkHex(hexFileEntry, function(serial) {
        sn = serial;
      });
      disableManualButtons(open);
      /*unsetHexMenu();*/
    });
  } else {
    /*setHexMenu();*/
    /*disableManualButtons(open);*/
  }
}

function checkHex(theFileEntry, cb) {
  theFileEntry.file(function(file) {
    var fileReader = new FileReader();

    fileReader.onload = function(e) {
      setStatus(theFileEntry.name);
    };

    fileReader.onerror = function(e) {
      setStatus("Hex failed: " + e.toString());
    };

    fileReader.onloadend = function(e) {
      loadHex(fileReader.result, cb);
      delete file;
      delete this;
    };

    fileReader.readAsText(file);
  }, errorHandler);
}

function setHexMenu() {
  chrome.contextMenus.removeAll(function() {
    hexFileEntry = null;
    hexchange = null;
      /*for (var snippetName in SNIPPETS) {*/
    chrome.contextMenus.create({
      title: "Set HEX",
      id: "sethex",
      contexts: ['all']
    });
      /*}*/
  });
}

function unsetHexMenu() {
  chrome.contextMenus.removeAll(function() {
    chrome.contextMenus.create({
      title: "Unset HEX",
      id: "unsethex",
      contexts: ['all']
    });
  });
}

function disableManualButtons(dis) {
  /*reloadButton.disabled = dis;*/
  /*increaseButton.disabled = dis;*/
}


function i2hex(i) {
  if (i < 0x10)
    return '0' + i.toString(16).toUpperCase();
  else if (i < 0x0100)
    return i.toString(16).toUpperCase();
  else if (i < 0x00010000)
    return i2hex((i >> 8) & 0xFF) + i2hex(i & 0xFF);
  else if (i < 0x01000000)
    return i2hex((i >> 16)  & 0xFF) + i2hex((i >> 8) & 0xFF) + i2hex(i & 0xFF);
  else
    return 'xx';
}

timeout = setInterval(function() {
  if (fileEntry && hexFileEntry && res && sn) {
    console.log("Check serial update");
    handleReloadButton();
  }
}, 3000);

chrome.contextMenus.onClicked.addListener(function(info) {
  // Context menu command wasn't meant for us.
  if (!document.hasFocus()) {
    return;
  }

  if (info.menuItemId === 'sethex') {
    handleHexFile(true);
  } else if (info.menuItemId === 'unsethex') {
    handleHexFile(false);
  }
});

onload = function() {
  openButton = document.getElementById("open");
  openHexButton = document.getElementById("openhex");
  /*reloadButton = document.getElementById("reload");*/
  /*increaseButton = document.getElementById("increase");*/

  openButton.addEventListener("click", handleOpenButton);
  openHexButton.addEventListener("click", handleOpenHexButton);
  /*reloadButton.addEventListener("click", handleReloadButton);*/
  /*increaseButton.addEventListener("click", handleIncreaseButton);*/

  newFile();
  disableManualButtons(true);
  openHexButton.disabled = true;
};

