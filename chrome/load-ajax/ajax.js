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
      //console.log("xmlhttp.responseText " + xmlhttp.responseText);
      xml = (new DOMParser()).parseFromString(xmlhttp.responseText, "text/xml");
      // x=xmlhttp.responseXML.documentElement.getElementsByTagName("CD");
      if (xml) {
        console.log(xml.documentElement);
      } else {
        console.log("xml null");
      }
      x=xml.getElementsByClassName("thedetial");
      txt = "1.0";
      if (x) {
        console.log(x.length);
      } else {
        console.log("null");
      }
      //for (i=0;i<x.length;i++) {
      //  txt=txt + "<tr>" +  xx[i].firstChild.nodeValue + "</tr>";
      //}
      document.getElementById('txtCDInfo').innerHTML = txt;
      //document.getElementById('txtCDInfo').innerHTML=xmlhttp.responseText;
    }
  };
  xmlhttp.open("GET",url,true);
  xmlhttp.send();
}

$(document).ready(function() {
  $("#btnLoad").click(function() {
    loadXMLDoc('https://direct.aia.co.th/btob/FundQueryServlet?actionType=show');
  });
});
