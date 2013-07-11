var appWindow;

chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('main.html', {
    minWidth: 320,
    minHeight: 400,
    bounds: {
      top: 20,
      left: 20,
      width: 740,
      height: 480
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


