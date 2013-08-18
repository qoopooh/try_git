chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('window.html', {
    /*id: 'aia', // to remember position*/
    frame: 'chrome', bounds: { width: 320, height: 300 },
    minWidth:200, minHeight:140, maxHeight:600
  });
});

