chrome.app.runtime.onLaunched.addListener(function() {
  // width 640 for font size 12
  //       720 for font size 14
  chrome.app.window.create('main.html', {
    id: 'dim1', // to remember position
    frame: 'chrome', bounds: { width: 320, height: 145 },
    minWidth:200, minHeight:140
  });
});

