
function getURLParameter(name) {
    return decodeURIComponent((new RegExp('[?|&]' + name + '=' + '([^&;]+?)(&|#|;|$)').exec(location.search)||[,""])[1].replace(/\+/g, '%20'))||null
}

function onSearch() {
  var s = $("#search").val();
  var g = $("#group").val();

  console.log("req", s, g);
}

function init() {
    /*$("#search").keypress(function(e) {*/
    /*if(e.keyCode == 13) {*/
    /*e.defaultPrevented;*/
    /*alert('You pressed enter!');*/
    /*} else {*/
    /*super.keypress(e);*/
    /*}*/
    /*});*/
    /*$(document).keypress(function(e) {*/
    /*if (e.which === 13)*/
    /*onSearch();*/
    /*});*/
  $("#btnSearch").click(onSearch);

  console.log('param 1', getURLParameter('group'));
}

$(document).ready(init);

