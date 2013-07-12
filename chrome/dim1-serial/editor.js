var reloadButton, openButton, increaseButton;
var editor;
var fileEntry;
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

  console.log("Error: " + msg);
}

function handleDocumentChange(title) {
  var mode = "javascript";
  var modeName = "JavaScript";
  if (title) {
    title = title.match(/[^/]+$/)[0];
    document.getElementById("title").innerHTML = title;
    document.title = title;
    if (title.match(/.json$/)) {
      mode = {name: "javascript", json: true};
      modeName = "JavaScript (JSON)";
    } else if (title.match(/.html$/)) {
      mode = "htmlmixed";
      modeName = "HTML";
    } else if (title.match(/.css$/)) {
      mode = "css";
      modeName = "CSS";
    }
  } else {
    document.getElementById("title").innerHTML = "[no document loaded]";
  }
}

function newFile() {
  fileEntry = null;
  hasWriteAccess = false;
  handleDocumentChange(null);
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
        handleDocumentChange(theFileEntry.fullPath);
      };

      fileReader.onerror = function(e) {
        console.log("Read failed: " + e.toString());
      };

      fileReader.onloadend = function(e) {
        res = this.result;
        var n = res.match(LOT)[0];
        if (!n) {
          console.log("cannot find PROD_LOT");
          return;
        }
        n = n.replace(/\s+/g, ' ');
        var lot = n.split(" ")[2];
        n = res.match(SNRH)[0];
        if (!n) {
          console.log("cannot find SNRH");
          return;
        }
        n = n.replace(/\s+/g, ' ');
        var snrh = n.split(" ")[2];
        n = res.match(SNRL)[0];
        if (!n) {
          console.log("cannot find SNRL");
          return;
        }
        n = n.replace(/\s+/g, ' ');
        var snrl = n.split(" ")[2];

        sn = lot << 16;
        sn += snrh << 8;
        sn += snrl & 0xFF;
        console.log('onloadend', sn, lot, snrh, snrl);
        document.getElementById("serial").innerHTML =
          sn.toString(16).toUpperCase();
      };

      fileReader.readAsText(file);
    }, errorHandler);
  }
}

function writeEditorToFile(theFileEntry) {
  ++sn;
  console.log(theFileEntry);

  res.replace(SNRL, SNRL_NEW + (sn & 0xFF));
  theFileEntry.createWriter(function(fileWriter) {
    fileWriter.onerror = function(e) {
      console.log("Write failed: " + e.toString());
    };

    var blob = new Blob(res);
    fileWriter.truncate(blob.size);
    fileWriter.onwriteend = function() {
      fileWriter.onwriteend = function(e) {
        handleDocumentChange(theFileEntry.fullPath);
        console.log("Write completed.");
      };

      fileWriter.write(blob);
      console.log("Write", blob);
    }
  }, errorHandler);
  document.getElementById("serial").innerHTML =
    sn.toString(16).toUpperCase();
}

var onWritableFileToOpen = function(theFileEntry) {
  setFile(theFileEntry, true);
  readFileIntoEditor(theFileEntry);
};

function handleNewButton() {
  onWritableFileToOpen(fileEntry);
}

function handleOpenButton() {
  chrome.fileSystem.chooseEntry({ type: 'openWritableFile' }, onWritableFileToOpen);
}

function handleSaveButton() {
  /*if (fileEntry && hasWriteAccess) {*/
  writeEditorToFile(fileEntry);
  /*}*/
}

function initContextMenu() {
  chrome.contextMenus.removeAll(function() {
      /*for (var snippetName in SNIPPETS) {*/
      /*chrome.contextMenus.create({*/
    /*title: snippetName,*/
    /*id: snippetName,*/
    /*contexts: ['all']*/
    /*});*/
      /*}*/
  });
}

chrome.contextMenus.onClicked.addListener(function(info) {
  // Context menu command wasn't meant for us.
  if (!document.hasFocus()) {
    return;
  }

  /*editor.replaceSelection(SNIPPETS[info.menuItemId]);*/
});

onload = function() {
  initContextMenu();

  openButton = document.getElementById("open");
  reloadButton = document.getElementById("reload");
  increaseButton = document.getElementById("increase");

  openButton.addEventListener("click", handleOpenButton);
  reloadButton.addEventListener("click", handleNewButton);
  increaseButton.addEventListener("click", handleSaveButton);

  /*editor = CodeMirror(*/
  /*document.getElementById("editor"),*/
  /*{*/
  /*mode: {name: "javascript", json: true },*/
  /*lineNumbers: true,*/
  /*theme: "lesser-dark",*/
  /*fixedGutter: true,*/
  /*extraKeys: {*/
  /*"Cmd-S": function(instance) { handleSaveButton() },*/
  /*"Ctrl-S": function(instance) { handleSaveButton() },*/
  /*}*/
  /*});*/

  newFile();
  onresize();
};

onresize = function() {
  var container = document.getElementById('editor');
  var containerWidth = container.offsetWidth;
  var containerHeight = container.offsetHeight;

  /*editor.refresh();*/
}

