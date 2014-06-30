function init() {
  $("#btnBOM").click(function() {
    var rev = prompt("Please enter revision number", "0");
    var csv_link = "bom?a=" + QueryString["a"] + "&rev=" + rev;
    document.location.href = csv_link;
    console.log(QueryString);
    console.log(csv_link);
  });
}

$(document).ready(init);

