var f_translate = true;
var f_oneline = true;
var f_receive = true;

var productCode = {
  '31':'W-4SEC',
  '32':'W-4SEC MTI_4INTR',
  '33':'W-4SEC MTI_16KEY',
  '41':'D-8ONOFF',
  '43':'W-2ONOFF',
  '44':'W-1SHUTT',
  '45':'W-1DIM',
  '47':'B-8SEC',
  '4A':'RC-4BRF',
  '4C':'W-INPKEY',
  '50':'W-1DIM010V',
  '54':'W-THERMO',
  '55':'D-4SHUTT',
  '5B':'W-2VLESS',
  '5C':'D-BUSSUPIP',
  '5E':'W-1DIMRGB',
  '57':'W-FM2BT',
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
  '50':'CMD_BABY_RX',
  '51':'CMD_BABY_DETECT',
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
var SRC = {
  '00':'s-all',
  '01':'s-sct',
  '02':'s-aut',
  '03':'s-sca',
  '04':'s-scn',
  '06':'s-mod',
  '07':'s-rs2'
}
var DST = {
  '00':'d-all',
  '01':'d-sct',
  '02':'d-aut',
  '03':'d-sca',
  '04':'d-scn',
  '06':'d-mod',
  '07':'d-rs2'
}
var MSG_TYPE = {
  '00':'E',
  '01':'I',
  '02':'R',
  '03':'C'
}
var TEMP_DEC = {
  '00':'0',
  '20':'1',
  '40':'3',
  '60':'4',
  '80':'5',
  'A0':'6',
  'C0':'8',
  'E0':'9'
}

function getTimeStamp() {
  var d = new Date();
  return d;
}

function zeroPad(number) {
  return (number < 10) ? '0' + number : number;
}

function translate(words, cb) {
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
      var tz = parseInt(words[11], 16);
      if (tz > 12)
        tz -= 256;
      text = product + ',' + sn + ',' + cmd + ',' + SRC[words[0]] + ',' + DST[words[1]]
        + ',' + MSG_TYPE[words[2]] + ', ' + words[4] + ':' + words[5] 
        + ' ' + words[6] + words[7] + '-' + words[8] + '-' + words[9]
        + ' (' + weekDay[words[10]] + ') timezone ' + tz;
      cb(text);
      break;
    case 'CMD_TOGGLE_MASTER':
      text = cmd + ',' + SRC[words[0]] + ',' + DST[words[1]]
        + ',' + MSG_TYPE[words[2]] + ': ' + words[4];
      cb(text);
      break;
    case 'CMD_TEMPERATURE':
      translateProductCommand(cmd, words, function(text) {
        if (words[8] === '01') {
          var temp = parseInt(words[9], 16);
          cb(text + ', ' + temp + '.' + TEMP_DEC[words[10]] + ' degree(s)');
        } else {
          cb(text + ', - degree');
        }
      });
      break;;
    case 'CMD_ANALOG':
      translateProductCommand(cmd, words, function(text) {
        cb(text + ',out' + parseInt(words[8], 16) + ',' + parseInt(words[9], 16) + '%');
      });
      break;
    case 'CMD_TOGGLE':
    case 'CMD_SET':
    case 'CMD_RESET':
      translateProductCommand(cmd, words, function(text) {
        cb(text + ',out' + parseInt(words[8], 16));
      });
      break;
    case 'CMD_STATUS':
      translateProductCommand(cmd, words, function(text) {
        getStatus(words, text, cb);
      });
      break;
    case 'CMD_CLIMA':
      translateProductCommand(cmd, words, function(text) {
        var temp = parseInt(words[13], 16) + '.' + TEMP_DEC[words[14]];

        text += ', ' + temp + ' degree(s)';
        cb(text);
      });
      break;
    case 'CMD_NAME':
    case 'CMD_SOFT_VERSION':
    case 'CMD_LEDS_BUZZ':
      translateProductCommand(cmd, words, function(text) {
        text += ', ' + hexStringToAscii(words, 8);
        cb(text);
      });
      break;
    default:
      translateProductCommand(cmd, words, function(text) {
        cb(text);
      });
      break;
  }
}

function hexStringToAscii(w, offset, len) {
  var str = '';
  
  if (!offset)
    offset = 0;
  if (!len)
    len = w.length;
  for (var i = offset; i < len; ++i) {
    var ch = parseInt(w[i], 16);
    if ((ch < 32) || (ch > 126))
      ch = 63; // ?
    str += String.fromCharCode(ch);
  }

  return str;
}

function translateProductCommand(cmd, words, cb) {
  var text = "";
  var product = productCode[words[4]];
  if (!product)
    product = words[4];
  var sn = "";
  if (words.length < 6) {
    sn = '-';
  } else {
    sn = words[5] + words[6] + words[7];
  }

  text = product + ',' + sn + ',' + cmd + ',' + SRC[words[0]] + ','
      + DST[words[1]] + ',' + MSG_TYPE[words[2]];
  cb(text);
}

function getStatus(words, text, cb) {
  var product = productCode[words[4]];

  switch (product) {
    case 'W-2OnOff':
      if (words.length !== 11)
        break;
      var out = parseInt(words[8], 16);
      var out1 = String.fromCharCode(parseInt(words[9], 16));

      if (out === 0xFF) {
        var out2 = String.fromCharCode(parseInt(words[10], 16));
        text += ',' + out1 + out2;
      } else if (out === 1) {
        text += ',out1-' + out1;
      } else if (out === 2) {
        text += ',out2-' + out1;
      }
      break;
    case 'W-1Shutter':
      if (words.length !== 11)
        break;
      var out = parseInt(words[8], 16);

      text += ',out' + out;
      if (words[9] === '00') {
        text += ',stop,';
      } else if (words[9] === '01') {
        text += ',up,';
      } else if (words[9] === '02') {
        text += ',down,';
      }
      text += parseInt(words[10], 16) + '%';
      break;
    case 'D-8OnOff':
      var out = parseInt(words[8], 16);

      if (out === 0xFF) {
        if (words.length !== 17)
          break;
        text += ',';
        for (var i=9; i<17; i++) {
          text += String.fromCharCode(parseInt(words[i], 16));
        }
      } else {
        if (words.length !== 10)
          break;
        var out_s = String.fromCharCode(parseInt(words[9], 16));
        text += ',out' + out + '-' + out_s;
      }
      break;
    case 'D-4Shutter':
      var out = parseInt(words[8], 16);

      if (out === 0xFF) {
        if (words.length !== 17)
          break;
        for (var i=9; i<17; i++) {
          if (words[i] === '00') {
            text += ',stop,';
          } else if (words[i] === '01') {
            text += ',up,';
          } else if (words[i] === '02') {
            text += ',down,';
          }
          ++i;
          text += parseInt(words[i], 16) + '%';
        }
      } else {
        if (words.length !== 11)
          break;
        text += ',out' + out;
        if (words[9] === '00') {
          text += ',stop,';
        } else if (words[9] === '01') {
          text += ',up,';
        } else if (words[9] === '02') {
          text += ',down,';
        }
        text += parseInt(words[10], 16) + '%';
      }
    case 'W-1DIMRGB':
      if (words.length !== 16)
        break;
      var out = parseInt(words[8], 16);
      var d_stat = getStatText(parseInt(words[9], 16));
      var d_val = parseInt(words[10], 16);
      var h_stat = getStatText(parseInt(words[11], 16));
      var h_val = (parseInt(words[12], 16) * 256) + parseInt(words[13], 16);
      var s_stat = getStatText(parseInt(words[14], 16));
      var s_val = parseInt(words[15], 16);

      /*console.log("W-1DIMRGB", d_stat, d_val, h_stat, h_val, s_stat, s_val);*/
      text += ',d-' + d_stat + ',dv-' + d_val;
      text += ',h-' + h_stat + ',hv-' + h_val;
      text += ',s-' + s_stat + ',sv-' + s_val;
      break;
    default:
      break;
  }
  cb(text);
}

function getStatText(stat) {
  if (stat == 0) {
    return "STOP";
  } else if (stat == 1) {
    return "UP";
  } else if (stat == 2) {
    return "DOWN";
  }
  return "UNKNOWN";
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
  document.getElementById("chkReceive").checked = f_receive;
  $("#chkTranslate").click(function() {
    f_translate = document.getElementById("chkTranslate").checked;
    document.getElementById("chkOneLine").disabled = !f_translate;
  });
  $("#chkOneLine").click(function() {
    f_oneline = document.getElementById("chkOneLine").checked;
  });
  $("#chkReceive").click(function() {
    f_receive = document.getElementById("chkReceive").checked;
  });
  $("#btnClear").click(function() {
    $("#messagewindow").text("");
  });
  $("#btnAddress").click(function() {
    chat.emit('enquiry', { 'cmd':'address' });
    clearDevice();
  });
  $("#btnVersion").click(function() {
    chat.emit('enquiry', { 'cmd':'version' });
  });
  var chat = io.connect('/ftc');
  chat.on('message', function(msg) {
    if (!f_receive)
      return;

    var words = msg.split(' ');
    var currentdate = new Date(); 
    var time = zeroPad(currentdate.getHours()) + ":"  
        + zeroPad(currentdate.getMinutes()) + ":" 
        + zeroPad(currentdate.getSeconds());
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

