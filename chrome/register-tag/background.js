var appWindow;

chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('main.html', {
    minWidth: 330,
    minHeight: 355,
    bounds: {
      top: 20,
      left: 20,
      width: 720,
      height: 473
    }
  }, function(app) {
    app.contentWindow.onclose = function() {
      console.log("onClosed");
    };
  });

  chrome.runtime.onSuspend.addListener(function() { 
  });
  /*chrome.app.window.onClosed.addListener(function() {*/
  /*console.log("onClosed");*/
  /*});*/
});


