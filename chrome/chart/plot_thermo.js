var data = {
  labels : ["January","February","March","April","May","June","July"],
  datasets : [
    {
    fillColor : "rgba(220,220,220,0.5)",
    strokeColor : "rgba(220,220,220,1)",
    pointColor : "rgba(220,220,220,1)",
    pointStrokeColor : "#fff",
    data : [65,59,90,81,56,55,40]
    },
    {
    fillColor : "rgba(151,187,205,0.5)",
    strokeColor : "rgba(151,187,205,1)",
    pointColor : "rgba(151,187,205,1)",
    pointStrokeColor : "#fff",
    data : [28,48,40,19,96,27,100]
    }
  ]
}

function init() {
  //Get context with jQuery - using jQuery's .get() method.
  var ctx = $("#myChart").get(0).getContext("2d");
  //This will get the first returned node in the jQuery collection.
  /*var myNewChart = new Chart(ctx);*/
  var myNewChart = new Chart(ctx).PolarArea(data);
  /*var myNewChart = new Chart(ctx).PolarArea(data,options);*/
}

function start() {
  //Get the context of the canvas element we want to select
  var ctx = document.getElementById("myChart").getContext("2d");
  var myNewChart = new Chart(ctx).PolarArea(data);
  console.log("on");
}

$(document).ready(init());
/*$(document).ready(start());*/

