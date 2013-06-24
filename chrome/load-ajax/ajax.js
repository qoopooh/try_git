function loadXMLDoc(url)
{
  var xmlhttp;
  var txt,x,xx,i;
  if (window.XMLHttpRequest)
  {// code for IE7+, Firefox, Chrome, Opera, Safari
    xmlhttp=new XMLHttpRequest();
  }
  else
  {// code for IE6, IE5
    xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
  }
  xmlhttp.onreadystatechange=function() {
    if (xmlhttp.readyState==4 && xmlhttp.status==200) {
      /*txt = xmlhttp.responseText.split("</head>");*/
      /*console.log ("split " + txt.length);*/
      /**//*console.log("xmlhttp.responseText " + txt[1]);*/
      /*xml = (new DOMParser()).parseFromString(txt[1], "text/xml");*/
      /*// x=xmlhttp.responseXML.documentElement.getElementsByTagName("CD");*/
      /*if (xml) {*/
      /*console.log(xml.documentElement);*/
      /*} else {*/
      /*console.log("xml null");*/
      /*}*/
      /*x=xml.documentElement.getElementsByClassName("thedetial");*/
      /*if (x) {*/
      /*console.log(x.length);*/
      /*} else {*/
      /*console.log("null");*/
      /*}*/
      /*for (i=0;i<x.length;i++) {*/
      /*txt=txt + "<tr>" +  xx[i].firstChild.nodeValue + "</tr>";*/
      /*document.getElementById('fundInfo').innerHTML = txt[1];*/
      document.getElementById('fundInfo').innerHTML = xmlhttp.responseText;
    }
  };
  xmlhttp.open("GET",url,true);
  xmlhttp.send();
}

$(document).ready(function() {
    /*$("#btnLoad").click(function() {*/
  loadXMLDoc('https://direct.aia.co.th/btob/FundQueryServlet?actionType=show');
    /*});*/
    $("#aialink").click(function() {
      chrome.tabs.create({
        'url':'https://portal.aia.co.th/futuregoals/sec/portal/public/fund-selection/fund-price',
        'selected':true
      });
    });
});
