function remove(number, date) {
}

function init() {
  $("#date").change(function() {
  });
  $("#remove").click(function() {
      $("#number").val("100");
  });
  $("#number").val("50");
  $("#table").text("None");
  $("#date").datepicker();
}

$(document).ready(init);

