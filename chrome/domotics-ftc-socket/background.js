chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('main.html', {
    bounds: {
      width: 800,
      height: 480
    }
  }, winHandler);
  console.log('launched');
});
chrome.runtime.onInstalled.addListener(function() {
  console.log('installed');
});
chrome.runtime.onSuspend.addListener(function() {
  console.log('suspend');
});
chrome.runtime.onMessage.addListener(function(request) {
  console.log(request);
  switch (request.type) {
    case 'report_sockid':
      console.log('sockId', request.val);
      break;
    default:
      break;
  }
});

function winHandler(win) {
  win.onClosed.addListener(function() {
    console.log('closed', win.contentWindow.sockId);
    chrome.socket.disconnect(win.contentWindow.sockId);
  });
}


