var data = require("sdk/self").data;
var aia_port = require("sdk/panel").Panel({
  width: 450,
  height: 600,
  contentScriptFile: data.url('load.js'),
  contentURL: data.url('window.html')
});
require("sdk/widget").Widget({
  label: "AIA Link widget",
  id: "aia-link-widget",
  contentURL: data.url("aia48.png"),
  panel: aia_port
});
aia_port.on("show", function() {
  aia_port.port.emit("show");
  loadDoc('https://direct.aia.co.th/btob/FundQueryServlet?actionType=show');
});

function loadDoc(link)
{
  require("sdk/request").Request({
    url: link,
    onComplete: function(response) {
      console.log("loadDoc onComplete");
      aia_port.port.emit("portfolio", response.text);
    }
  }).get();
  console.log("loadDoc()" + link);
}

