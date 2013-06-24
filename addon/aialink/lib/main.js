var data = require("sdk/self").data;
var text_entry = require("sdk/panel").Panel({
  width: 690,
  height: 450,
  contentURL: 'https://direct.aia.co.th/btob/FundQueryServlet?actionType=show'
});
require("sdk/widget").Widget({
  label: "Text entry",
  id: "text-entry",
  contentURL: data.url("aia48.png"),
  panel: text_entry
});
text_entry.on("show", function() {
  text_entry.port.emit("show");
});
text_entry.port.on("text-entered", function(text) {
  console.log(text);
  text_entry.hide();
});

