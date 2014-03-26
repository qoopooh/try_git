var productCode = {
  '31':'W-4Sec',
  '32':'W-4Sec',
  '33':'W-4Sec',
  '41':'D-8ONOFF',
  '43':'W-2ONOFF',
  '44':'W-1SHUTT',
  '45':'W-1DIM',
  '47':'B-8SEC',
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

/************************************************************/
/*                        FTC layer                         */
/************************************************************/
// ftc init
FTC_STATE = {
  kInit: {value:0, name:"Initial", code:"I"},
  kMessageSize: {value:1, name:"Message Size", code:"S"},
  kMessageReceive: {value:2, name:"Message Receive", code:"R"},
  kMessageVerify: {value:3, name:"Message Verify", code:"V"},
  kEnd: {value:127, name:"End", code:"E"}
};

var recvBuffer = new Uint8Array(32 + 1); // 32 byte the maximum length allowed in FTC
var recvBufferIndex = 0;
var ftcRcvState = FTC_STATE.kInit;
var ftcMessageSize = 0;
var ftc_msg_count = 0;

function getFtcMessage(data, cb) {
  var conv = new Converter();

  for (var index=0; index<data.length; index++) {
    var dat = data[index];
    if (ftcRcvState.value === FTC_STATE.kInit.value) {
      if (dat !== 0x7D) {
        log("ftc message error: " + ftcRcvState.name + dat);
        return cb();
      }
      ftcRcvState = FTC_STATE.kMessageSize;
    } else if (ftcRcvState.value === FTC_STATE.kMessageSize.value) {
      if (dat >= 32) {
        log("ftc message error: " + ftcRcvState.name + dat);
        recvBufferIndex = 0;
        return cb();
      }
      ftcMessageSize = dat;
      ftcRcvState = FTC_STATE.kMessageReceive;
    } else if (ftcRcvState.value === FTC_STATE.kMessageReceive.value) {
      if (dat === 0x7E) {
        ftcRcvState = FTC_STATE.kMessageVerify;
        continue;
      }
      recvBuffer[recvBufferIndex] = dat;
      ++recvBufferIndex;
      if (recvBufferIndex >= ftcMessageSize)
        ftcRcvState = FTC_STATE.kEnd;
    } else if (ftcRcvState.value === FTC_STATE.kMessageVerify.value) {
      if ((dat !== 0x5D) && (dat !== 0x5E) && (dat !== 0x5F)) {
        log("ftc message error: " + ftcRcvState.name + dat);
        recvBufferIndex = 0;
        return cb();
      }
      recvBuffer[recvBufferIndex] = dat + 32;
      ++recvBufferIndex;
      if (recvBufferIndex >= ftcMessageSize)
        ftcRcvState = FTC_STATE.kEnd;
      else
        ftcRcvState = FTC_STATE.kMessageReceive;
    } else if (ftcRcvState.value === FTC_STATE.kEnd.value) {
      if (dat !== 0x7F) {
        log("ftc message error: " + ftcRcvState.name + dat);
        recvBufferIndex = 0;
        return cb();
      }
      //TODO: crc check

      var buf = new Uint8Array(recvBufferIndex);
      buf[0] = recvBuffer[0] >> 5; // can use >>>
      buf[1] = (recvBuffer[0] & 0x1C) >> 2;
      buf[2] = recvBuffer[0] & 0x03;
      for (i=1; i<recvBufferIndex-2; i++) {
        buf[i+2] = recvBuffer[i];
      }
      var bufString = conv.abv2hexstr(buf);
      console.log(ftc_msg_count, recvBufferIndex, bufString);
      recvBufferIndex = 0;
      ftcRcvState = FTC_STATE.kInit;
      ++ftc_msg_count;
      return cb(bufString);
    } else {
    ftcRcvState = FTC_STATE.kInit;
    }
  }
  return cb();
}


function calculateCheckSum(data) {
  var sum = data.length + 1; // data length + itself;

  for (var i=0; i<data.length; i++) {
    sum += data[i];
  }
  sum &= 0xFF;
  sum ^= 0xFF;
  return sum;
}

function isReservedCode(c) {
  if ((c == 0x7D)
      || (c == 0x7E)
      || (c == 0x7F)) {
    return true;
  }
  return false;
}

var msg_nr = 0;
function buildFtc(data, cb) {
  var input = new Uint8Array(data.length + 1);

  input.set(data);
  input[input.length - 1] = (++msg_nr) & 0xFF;
  var sum = calculateCheckSum(input);
  var special = 0;

  if (isReservedCode(sum))
    ++special;

  for (var i=0; i<input.length; i++) {
    if (isReservedCode(input[i])) {
      ++special;
    }
  }

  var buf = new Uint8Array(data.length + 5 + special);
  var j = 2;
  buf[0] = 0x7D; //start
  buf[1] = data.length + j;

  /*buf.set(input, 2);*/
  for (var i=0; i<input.length; i++) {
    var c = input[i];
    if (isReservedCode(c)) {
      buf[j++] = 0x7E;
      buf[j++] = (c & 0xDF);
    } else {
      buf[j++] = c;
    }
  }
  if (isReservedCode(sum)) {
    buf[j++] = 0x7E;
    buf[j++] = sum & 0xDF;
  } else {
    buf[j++] = sum;
  }
  buf[j++] = 0x7F; //stop
  cb(buf);
}

/************************************************************/
/*                        FTC layer                         */
/************************************************************/





function translate(words, cb) {
  var cmd = commandCode[words[3]];
  var text = "";
  var product = "";
  var sn = "";
  var conv = new Converter();

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
      translateProductCommand(cmd, words, function(text) {
        text += ', ' + conv.hexStringToAscii(words, 8);
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
    case 'W-2ONOFF':
    case 'W-2VLESS':
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
    case 'W-1SHUTT':
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
    case 'D-8ONOFF':
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
    case 'D-4SHUTT':
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
      break;
    case 'W-1DIM':
      if (words[9] === '00') {
        text += ',stop,';
      } else if (words[9] === '01') {
        text += ',up,';
      } else if (words[9] === '02') {
        text += ',down,';
      }
      text += parseInt(words[10], 16) + '%';
      break;
    default:
      break;
  }
  cb(text);
}

function addCommandDevices(words) {
  var cmd = commandCode[words[3]];
  if ((cmd != 'CMD_ADDRESS') && (words.length < 8))
    return;
  var uid = words[4] + words[5] + words[6] + words[7];
  var exist = false;
  var commandCount = 0;

  var product = productCode[words[4]];
  if (!product)
    return;

  $("#selectCommandDevices option").each(function() {
    $("#txtCommandCount").text(++commandCount + " mti(s)");
    if (uid == $(this).val()) {
      exist = true;
    }
  });

  if (!exist) {
    $("#txtCommandCount").text(++commandCount + " mti(s)");
    $("#selectCommandDevices").append("<option>" + uid + "</option>");
  }
}

function clearDevice() {
  $("#txtCommandCount").text("0 mti");
  $("#selectCommandDevices").html("");
}



