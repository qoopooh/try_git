chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('main.html', {
    bounds: {
      width: 800,
      height: 480
    }
  });
  console.log('launched');
})
chrome.runtime.onInstalled.addListener(function() {
  console.log('installed');
});
chrome.runtime.onSuspend.addListener(function() { 
  console.log('suspend');
});

