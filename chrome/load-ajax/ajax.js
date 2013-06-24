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
      document.getElementById('fundInfo').innerHTML = xmlhttp.responseText;
    }
  };
  xmlhttp.open("GET",url,true);
  xmlhttp.send();
}

$(document).ready(function() {
  loadXMLDoc('https://direct.aia.co.th/btob/FundQueryServlet?actionType=show');
  $("#aialink").click(function() {
    chrome.tabs.create({
      'url':'https://portal.aia.co.th/futuregoals/sec/portal/public/fund-selection/fund-price',
      'selected':true
    });
  });
});
