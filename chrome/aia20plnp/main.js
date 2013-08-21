
function aia_calc() {
  (function () {
   console.log($("#birth").val());
   console.log($("#fund").val());
   console.log($("input[name=year]:radio").val());
   console.log($("input[name=type]:radio").val());
   console.log($("input[name=year]:radio").val());
   console.log($("#payment").val());
  })();
}

$(document).ready(function() {
  $("#calc").click(aia_calc);
  $("#birth").datepicker();
});

