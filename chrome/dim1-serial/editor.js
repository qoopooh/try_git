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

function newFile() {
  fileEntry = null;
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
        setStatus(theFileEntry.fullPath);
      };

      fileReader.onerror = function(e) {
        setStatus("Read failed: " + e.toString());
      };

      fileReader.onloadend = function(e) {
        res = this.result;
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

        sn = lot << 16;
        sn += snrh << 8;
        sn += snrl & 0xFF;
        console.log('onloadend', sn, lot, snrh, snrl);
        document.getElementById("serial").innerHTML = i2hex(sn);
        disableManualButtons(false);
        setHexMenu();
      };

      fileReader.readAsText(file);
    }, errorHandler);
  }
}

function writeEditorToFile(theFileEntry) {
  ++sn;

  res = res.replace(LOT, LOT_NEW + i2hex((sn >> 16) & 0xFF));
  res = res.replace(SNRH, SNRH_NEW + i2hex((sn >> 8) & 0xFF));
  res = res.replace(SNRL, SNRL_NEW + i2hex(sn & 0xFF));
  setStatus(theFileEntry);
  theFileEntry.createWriter(function(fileWriter) {
    fileWriter.onerror = function(e) {
      setStatus("Write failed: " + e.toString());
    };

    var blob = new Blob( [res], {type:'text/plain'});
    fileWriter.truncate(blob.size);
    fileWriter.onwriteend = function() {
      fileWriter.onwriteend = function(e) {
        setStatus("Write " + theFileEntry.fullPath + " completed.");
      };

      fileWriter.write(blob);
    }
  }, errorHandler);
  document.getElementById("serial").innerHTML = i2hex(sn);
}

var onWritableFileToOpen = function(theFileEntry) {
  if (!theFileEntry) {
    return;
  }

  setFile(theFileEntry, true);
  readFileIntoEditor(theFileEntry);
};

function handleNewButton() {
  onWritableFileToOpen(fileEntry);
}

function handleOpenButton() {
  chrome.fileSystem.chooseEntry(
      { type: 'openWritableFile',
        accepts: [{ extensions: ['h'] }] 
      }, onWritableFileToOpen);
}

function handleSaveButton() {
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
      checkHex(hexFileEntry);
      disableManualButtons(open);
      unsetHexMenu();
    });
  } else {
    setHexMenu();
    disableManualButtons(open);
  }
}

function checkHex(theFileEntry) {
  theFileEntry.file(function(file) {
    var fileReader = new FileReader();

    fileReader.onerror = function(e) {
      setStatus("Hex failed: " + e.toString());
    };

    fileReader.onloadend = function(e) {
      hexchange = this.result;
      console.log('hexchange', hexchange);
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
  reloadButton.disabled = dis;
  increaseButton.disabled = dis;
}

/*function i2hex(i) {*/
/*if (i < 16)*/
/*return '0' + i.toString(16).toUpperCase();*/
/*else if (i < 4096)*/
/*return i2hex((i >> 8) & 0xFF) + i2hex(i & 0xFF);*/
/*else if (i < 1048575)*/
/*return i2hex((i >> 16)  & 0xFF)+ i2hex((i >> 8) & 0xFF) + i2hex(i & 0xFF);*/
/*else*/
/*return i.toString(16).toUpperCase();*/
/*}*/

function i2hex(i) {
  if (i < 16)
    return '0' + i.toString(16).toUpperCase();
  else if (i < 256)
    return i.toString(16).toUpperCase();
  else if (i < 4096)
    return i2hex((i >> 8) & 0xFF) + i2hex(i & 0xFF);
  else if (i < 1048575)
    return i2hex((i >> 16)  & 0xFF)+ i2hex((i >> 8) & 0xFF) + i2hex(i & 0xFF);
}

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
  reloadButton = document.getElementById("reload");
  increaseButton = document.getElementById("increase");

  openButton.addEventListener("click", handleOpenButton);
  reloadButton.addEventListener("click", handleNewButton);
  increaseButton.addEventListener("click", handleSaveButton);

  newFile();
  disableManualButtons(true);
};

