chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('main.html', {
    bounds: {
      top: 20,
      left: 20,
      width: 650,
      height: 480
    }
  });
})
