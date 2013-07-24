var appWindow;

chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('main.html', {
    minWidth: 300,
    minHeight: 280,
    bounds: {
      top: 20,
      left: 20,
      width: 700,
      height: 400
    }
  }, function(app) {
    app.contentWindow.onclose = function() {
      console.log("onClosed");
    };
  });

  chrome.runtime.onSuspend.addListener(function() { 
  });
});

