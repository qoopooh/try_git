chrome.app.runtime.onLaunch.addListener(function() {
  chrome.app.window.create('window.html', {
    'width': 400,
    'height': 500
  });
});

