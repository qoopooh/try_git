var recv_count = 0;

function getTimeStamp() {
  var d = new Date();
  return d;
}

function zeroPad(number) {
  return (number < 10) ? '0' + number : number;
}

function resizeMessageWindow() {
  var bodyheight = $(window).height();
  var bodywidth = $(window).width();

  if (bodyheight > 300) {
    $("#chatwindow").height(bodyheight - 200);
    $("#inputcontainer").height(70);
  } else {
    $("#inputcontainer").height(bodyheight - 230);
  }
  $("#command").width(bodywidth - ($("#btnWrite").width() + 60));
}

$(document).ready(function() {
  document.getElementById("currenttime").innerHTML = getTimeStamp();
  resizeMessageWindow();
  $("#btnClear").click(function() {
    $("#messagewindow").text("");
  });
  $("#btnVersion").click(function() {
    chat.emit('enquiry', { 'cmd':'address' });
  });
  $("#btnWrite").click(function() {
    chat.emit('enquiry', { 'cmd':'raw', 'raw':$('#command').val()  });
  });
  $("#command").keypress(function (e) {
    if (e.which === 0x0D) {
      e.preventDefault();
      $(this).select();
      $("#btnWrite").click();
    }
  });
  var chat = io.connect('/pr');
  chat.on('message', function(msg) {
    var words = msg.split(' ');
    var currentdate = new Date(); 
    var time = zeroPad(currentdate.getHours()) + ":"  
        + zeroPad(currentdate.getMinutes()) + ":" 
        + zeroPad(currentdate.getSeconds());

    ++recv_count;
    $('#txtCommandCount').text(recv_count);
    $("#messagewindow").append(time + '-> ' + msg + '<br/>');
    var height = $("#messagewindow")[0].scrollHeight;
    $("#messagewindow").scrollTop(height);
  });
});

$(window).resize(function() {
  resizeMessageWindow();
});

