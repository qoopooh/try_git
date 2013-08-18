
function aia_calc() {
  console.log('calc');
}

$(document).ready(function() {
  $("#calc").click(aia_calc);
  $("#birth").datepicker();
});

