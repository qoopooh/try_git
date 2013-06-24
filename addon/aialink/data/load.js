self.port.on("show", function onShow() {
  document.getElementById('fundInfo').innerHTML = "Loading...";
});
self.port.on("portfolio", function onPortfolion(html) {
  document.getElementById('fundInfo').innerHTML = html;
});

