
function aia_calc() {
  (function () {
   var birthday = new Date($("#birth").val());
   var old = Math.floor(((Date.now() - birthday) / (31557600000)));
   var fund = Number($("#fund").val());
   var payment = parseInt($("#payment").val());
   var sex = $("input[name=sex]:checked").val();
   var type = $("input[name=type]:checked").val();
   var year = $("input[name=year]:checked").val();

   console.log(old);
   console.log(fund);
   console.log(sex);
   console.log(type);
   console.log(year);
   console.log(payment);
  })();
}

$(document).ready(function() {
  $("#calc").click(aia_calc);
  $("#birth").datepicker();
});

