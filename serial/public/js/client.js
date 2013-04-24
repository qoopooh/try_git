var f_translate = true;
var f_oneline = false;
var productCode = {
  '41':'D-8OnOff',
  '43':'W-2OnOff',
  '44':'W-1Shutter',
  '4C':'W-InpKeyb',
  '50':'W-Dim010V',
  '54':'W-Themo',
  '55':'D-4Shutter',
  '5B':'W-2VLess',
  '5C':'Gateway'
}
var commandCode = {
  '41':'CMD_ANALOG',
  '42':'CMD_BLOCK',
  '43':'CMD_SCENARIO',
  '44':'CMD_DEVICE',
  '45':'CMD_ERASE_INPUT',
  '49':'CMD_INIT_OUTPUT',
  '4B':'CMD_KEY_REMOTE',
  '4C':'CMD_TIMER',
  '4D':'CMD_MODE',
  '4E':'CMD_NAME',
  '4F':'CMD_DIRECT',
  '52':'CMD_RESET',
  '53':'CMD_SET',
  '54':'CMD_TOGGLE',
  '55':'CMD_UPDATE_SCENARIO',
  '59':'CMD_CLIMA',
  '5A':'CMD_TOGGLE_MASTER',
  '61':'CMD_ADDRESS',
  '62':'CMD_BACKUP',
  '63':'CMD_CONFIG',
  '64':'CMD_DELAY',
  '65':'CMD_ERROR',
  '66':'CMD_FACTORY_RESET',
  '68':'CMD_BLINDS_AUTO',
  '6B':'CMD_TEMPERATURE',
  '6C':'CMD_LEDS_BUZZ',
  '6D':'CMD_BLINDS_MANUAL',
  '6F':'CMD_BLINDS_OPEN',
  '70':'CMD_PROG_DATA',
  '72':'CMD_REPEAT',
  '73':'CMD_STATUS',
  '74':'CMD_TIME_DATE',
  '75':'CMD_UNIT_CMD',
  '76':'CMD_TEMPCTRL',
  '77':'CMD_SOFT_VERSION',
  '78':'CMD_OUTPUTSCTRL',
  '79':'CMD_TEST',
  '7A':'CMD_RESTORE'
}
var weekDay = {
  '01':'SUN',
  '02':'MON',
  '03':'TUE',
  '04':'WED',
  '05':'THU',
  '06':'FRI',
  '07':'SAT'
}

function getTimeStamp() {
  var d = new Date();
  return d;
}
function timeString(uint8) {
  var text = uint8.toString()
  if (text.length === 1)
    text = '0' + text;
  return text;
}
function translate(words, callback) {
  var cmd = commandCode[words[3]];
  var text = "";
  var product = "";
  var sn = "";

  if (!cmd)
    cmd = words[3];
  switch (cmd) {
    case 'CMD_TIME_DATE':
      product = productCode[words[12]];
      if (!product)
        product = words[12];
      sn = words[13] + words[14] + words[15];
      text = product + ',' + sn + ': ' + words[4] + '.' + words[5] 
        + ' ' + words[6] + words[7] + '/' + words[8] + '/' + words[9]
        + ' (' + weekDay[words[10]] + ') Timezone: ' + words[11];
      break;
    case 'CMD_TOGGLE_MASTER':
      text = cmd + ': ' + words[4];
      break;
    default:
      product = productCode[words[4]];
      if (!product)
        product = words[4];
      sn = words[5] + words[6] + words[7];
      text = product + ',' + sn + ',' + cmd;
      break;
  }
  callback(text);
}

function addCommandDevices(words) {
  var cmd = commandCode[words[3]];
  if (cmd != 'CMD_ADDRESS')
    return;
  var uid = words[4] + words[5] + words[6] + words[7];
  var exist = false;
  var commandCount = 0;

  var product = productCode[words[4]];
  if (!product)
    return;

  $("#selectCommandDevices option").each(function() {
    ++commandCount;
    if (uid == $(this).val()) {
    /*$("#currenttime").text($(this).val());*/
      exist = true;
    }
  });

  if (!exist) {
    $("#selectCommandDevices").append("<option>" + uid + "</option>");
  }
  ++commandCount;
  $("#txtCommandCount").html(commandCount + " Device(s):");
}

function clearDevice() {
  $("#txtCommandCount").html("0 Device:");
  $("#selectCommandDevices").html("");
}

function resizeMessageWindow() {
  var bodyheight = $(window).height();

  if (bodyheight > 300)
    $("#chatwindow").height(bodyheight - 200);
  /*document.getElementById("currenttime").innerHTML = bodyheight;*/
}

$(document).ready(function() {
  document.getElementById("currenttime").innerHTML = getTimeStamp();
  resizeMessageWindow();
  document.getElementById("chkTranslate").checked = f_translate;
  document.getElementById("chkOneLine").disabled = !f_translate;
  document.getElementById("chkOneLine").checked = f_oneline;
  $("#chkTranslate").click(function() {
    f_translate = document.getElementById("chkTranslate").checked;
    document.getElementById("chkOneLine").disabled = !f_translate;
  });
  $("#chkOneLine").click(function() {
    f_oneline = document.getElementById("chkOneLine").checked;
  });
  $("#btnClear").click(function() {
    $("#messagewindow").text("");
  });
  $("#btnAddress").click(function() {
    chat.emit('enquiry', { 'cmd':'address' });
    clearDevice();
  });
  var chat = io.connect('/ftc');
  chat.on('message', function(msg) {
    var words = msg.split(' ');
    var currentdate = new Date(); 
    var time = timeString(currentdate.getHours()) + ":"  
        + timeString(currentdate.getMinutes()) + ":" 
        + timeString(currentdate.getSeconds());
    if (f_translate) {
      translate(words, function(text) {
        var message = "";

        if (f_oneline) {
          message = time + '-> ' + msg + ' ->  ' + text
              + '<br/>';
        } else {
          message = time + '-> ' + msg + '<br/>' + text
              + '<br/>';
        }
        $("#messagewindow").append(message);
      });
    } else {
      $("#messagewindow").append(time + '-> ' + msg + '<br/>');
    }
    addCommandDevices(words);
    var height = $("#messagewindow")[0].scrollHeight;
    $("#messagewindow").scrollTop(height);
  });

  clearDevice();
});

$(window).resize(function() {
  resizeMessageWindow();
});

