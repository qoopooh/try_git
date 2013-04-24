function getCurrentTime() {
  var d = new Date();
  
  return d;
}

function refreshFeed() {
  $("#first_footer").text(getCurrentTime());
}

function startJqm() {
  refreshFeed();
  $("#page1").bind("pageshow", function(event, ui) {
      refreshFeed();
      });
  $(".show-menu").click(function() {
      $(".sliding-menu").toggleClass("reverse out in");
      });
}

$(document).ready(startJqm());

