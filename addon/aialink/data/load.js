var timeout_count = 0;

/*setTimeout(function() {*/
/*console.log("timeout: " + ++timeout_count);*/
/*}, 2000);*/

setInterval(function() {
  console.log("timeout: " + ++timeout_count);
}, 3000);

function loadXMLDoc(link)
{
  /*var xmlhttp = new XMLHttpRequest();*/

  /*xmlhttp.onreadystatechange=function() {*/
  /*if (xmlhttp.readyState==4 && xmlhttp.status==200) {*/
  /*document.getElementById('fundInfo').innerHTML = xmlhttp.responseText;*/
  /*console.log("readyState()");*/
  /*}*/
  /*};*/
  /*xmlhttp.open("GET",url,true);*/
  /*xmlhttp.send();*/
  console.log("loadXMLDoc()");
}

self.port.on("show", function onShow() {
  loadXMLDoc('https://direct.aia.co.th/btob/FundQueryServlet?actionType=show');
});
self.port.on("portfolio", function onPortfolion(html) {
  document.getElementById('fundInfo').innerHTML = html;
});

