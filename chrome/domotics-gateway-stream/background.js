chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('main.html', {
    bounds: {
      width: 480,
      height: 375
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

