var timeout_count = 0;

setInterval(function() {
  console.log("timeout: " + ++timeout_count);
}, 10000);

self.port.on("show", function onShow() {
  document.getElementById('fundInfo').innerHTML = "Loading...";
});
self.port.on("portfolio", function onPortfolion(html) {
  document.getElementById('fundInfo').innerHTML = html;
});

