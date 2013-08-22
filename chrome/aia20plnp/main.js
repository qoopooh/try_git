
function aia_calc() {
  (function () {
   var birthday = new Date($("#birth").val());
   var age = Math.floor(((Date.now() - birthday) / (31557600000)));
   var fund = Number($("#fund").val());
   var payment = parseInt($("#payment").val());
   var sex = $("input[name=sex]:checked").val();
   var type = $("input[name=type]:checked").val();
   var aia = new Aia();

   console.log(age);
   console.log(fund);
   console.log(sex);
   console.log(type);
   console.log(payment);
   console.log(aia.calPayment(fund, age, sex, type));
   $("#age").text(age);
   $("#rate").text(aia.rate);
   $("#payment").val(aia.result);
  })();
}

$(document).ready(function() {
  $("#calc").click(aia_calc);
  $("#birth").datepicker();
});

