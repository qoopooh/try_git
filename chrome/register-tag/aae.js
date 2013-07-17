var Command = {
  'GetSerialNumber': 0x0101,
  'GetReaderType': 0x0102,
  'GetHardwareRevision': 0x0103,
  'GetSoftwareRevision': 0x0104,
  'GetBootloaderRevision': 0x0105,
  'GetAntenuation': 0x0201,
  'GetFrequency': 0x0202,
  'GetSensitivity': 0x0203,
  'SetAntenuation': 0x0281,
  'SetFrequency': 0x0282,
  'SetSensitivity': 0x0283,
  'Reboot': 0x0301, // Unused
  'SetHeartBeat': 0x0302,
  'SetAntennaPower': 0x0303,
  'RestoreFactorySettings': 0x0320,
  'SaveSettingsPermanent': 0x0321,
  'SetParam': 0x0330,
  'GetParam': 0x0331,
  'InventorySingle': 0x5001,
  'InventoryCyclic': 0x5002,
  'ReadFromTag': 0x5003,
  'WriteToTag': 0x5004,
  'HearthbeatInterrupt': 0x9001,
  'InventoryCyclicInterrupt': 0x9002
};
var Packet = { 'Start': 0, 'CommandStart': 1, 'Command': 2,
  'LengthStart': 3, 'Length': 4, 'PayloadStart': 5, 'Payload': 6,
  'ChecksumStart': 7, 'Checksum': 8 };

var pState = Packet.Start;
var cmd = 0x0000;
var f_hb = false;
var f_hb_read = false;
var f_single = false;
var f_readtag = false;
var f_written = false;
var f_success = false;
var hbcount = 0;
var epc = null;
var epcstr = '';
var readdata = null;

function extractPackage(arr, idx, cb) {
  var pkgsize = 10; // smallest posible package

  if (idx < pkgsize)
    return cb();
  if ((arr[0] !== 0x41) || (arr[1] !== 0x41) || (arr[2] !== 0x45)
      || (arr[3] !== 0x01)
      || (arr[6] !== 0x02)) {
    return cb();
  }
  cmd = (arr[4] << 8) | arr[5];
  len = arr[7];
  console.log("cmd", cmd.toString(16), len);
  if (len < 1) {
    if (arr[8] !== 0x04) {
      return cb();
    }
  } else {
    if ((arr[8] !== 0x03) || (arr[9 + len] !== 0x04)) {
      return cb();
    }
    var payload = new Uint8Array(len);
    payload.set(arr.subarray(9, 9 + len));
    pkgsize = 11 + len;
  }
  /*console.log('arr', u82hex(arr.subarray(0, idx)));*/
  /*console.log('payload', len, u82hex(payload));*/
  if (!isSum(arr, pkgsize))
    return cb();
  execReceivingMessage(cmd, len, payload, function () {
    if (idx > pkgsize)
      extractPackage(arr.subarray(pkgsize), idx - pkgsize, cb);
    else
      return cb();
  });
}

function execReceivingMessage(cmd, len, payload, cb) {
  var msg = "";
  switch (cmd) {
    case Command.InventorySingle:
      var tagcount = payload[1];

      f_single = true;
      if (tagcount) {
        epc = payload.subarray(6);
        epcstr = u82hex(epc);
        msg = 'EPC: ' + epcstr;
      } else {
        epc = null;
        epcstr = '';
        msg = 'EPC: -';
      }
      aaelog(msg);
      break;
    case Command.ReadFromTag:
      f_readtag = true;

      if (payload[0] === 0 && payload[1] > 0) {
        readdata = payload.subarray(2);
        msg = 'USER: ' + u82hex(readdata);
      } else {
        readdata = null;
        msg = 'USER: -';
      }
      aaelog(msg);
      break;
    case Command.WriteToTag:
      f_written = true;
      if (payload[0] === 0)
        f_success = true;
      break;
    case Command.HearthbeatInterrupt:
      f_hb = true;
      f_hb_read = true;
      msg = 'HB ' + ++hbcount;
      aaelog(msg);
      break;
    case Command.InventoryCyclicInterrupt:
      epc = payload.subarray(2);
      epcstr = u82hex(epc);
      msg = 'EPC: ' + epcstr;
      aaelog(msg);
      break;
    default:
      break;
  }
  cb();
}

function isSum(arr, idx) {
  var len = idx - 1;
  var sum = checksum(arr, len);

  console.log("Checksum", sum === arr[len]);
  if (sum === arr[len]) {
    return true;
  }
  return false;
}

function checksum(arr, len) {
  var sum = 0;

  for (var i = 0; i < len; ++i) {
    sum ^= arr[i];
  }
  console.log('sum', sum.toString(16));
  return sum;
}

function buildPackage(cmd, payload) {
  var bufLen = 10;
  var len = 0;
  if (payload) {
    bufLen = payload.length + 11;
    len = payload.length;
  }
  var buf = new ArrayBuffer(bufLen);
  var arr = new Uint8Array(buf);
  var cmd1 = cmd >> 8;
  var cmd2 = cmd & 0x00ff;

  arr.set([0x41, 0x41, 0x45, 0x01, cmd1, cmd2, 0x02, len]);
  if (len) {
    arr.set(new Uint8Array([0x03]), 8);
    arr.set(payload, 9);
    len += 9;
  } else {
    len = 8;
  }
  arr.set(new Uint8Array([0x04]), len);
  ++len;
  arr.set(new Uint8Array([checksum(arr, len)]), len);

  console.log('build', u82hex(arr));

  return buf;
}

function getSoftwareRevision(cb) {
  var buf = buildPackage(Command.GetSoftwareRevision);
  cb(buf);
}

function inventorySingle(cb) {
  var buf = buildPackage(Command.InventorySingle);

  epc = null;
  cb(buf);
}

function inventoryCyclic(on, cb) {
  if (on)
    var buf = buildPackage(Command.InventoryCyclic, new Uint8Array([1]));
  else
    var buf = buildPackage(Command.InventoryCyclic, new Uint8Array([0]));

  epc = null;
  cb(buf);
}

function setHeartbeat(on, cb) {
  f_hb = on;
  if (f_hb) {
    var buf = buildPackage(Command.SetHeartBeat, new Uint8Array([1]));
    hbcount = 0;
  } else {
    var buf = buildPackage(Command.SetHeartBeat, new Uint8Array([0]));
  }
  cb(buf);
}

function toggleHeartbeat(cb) {
  setHeartbeat(!f_hb, cb);
}

function readFromTag(epc, bank, addr, pass, count, cb) {
  var epclen = epc.length;
  var payload = new Uint8Array(epclen + 9);
  var i = 0;

  payload.set(new Uint8Array([epclen]), i);
  payload.set(epc, ++i);
  payload.set(new Uint8Array([bank]), epclen + i);
  if (!addr)
    addr = new Uint8Array([0, 0]);
  else
    addr = new Uint8Array([(addr >> 8) & 0xFF, addr & 0xFF]);
  payload.set(addr, epclen + (++i));
  if (!pass)
    pass = new Uint8Array([0, 0, 0, 0]);
  payload.set(pass, epclen + (i += 2));
  payload.set(new Uint8Array([count]), epclen + (i += 4));

  f_readtag = false;
  f_success = false;
  readdata = null;
  var buf = buildPackage(Command.ReadFromTag, payload);
  cb(buf);
}

function writeToTag(epc, bank, addr, pass, data, cb) {
  var epclen = epc.length;
  var datalen = data.length;
  var payload = new Uint8Array(epclen + 9 + datalen);
  var i = 0;

  payload.set(new Uint8Array([epclen]), i);
  payload.set(epc, ++i);
  payload.set(new Uint8Array([bank]), epclen + i);
  if (!addr)
    addr = new Uint8Array([0, 0]);
  else
    addr = new Uint8Array([(addr >> 8) & 0xFF, addr & 0xFF]);
  payload.set(addr, epclen + (++i));
  if (!pass)
    pass = new Uint8Array([0, 0, 0, 0]);
  payload.set(pass, epclen + (i += 2));
  payload.set(new Uint8Array([datalen]), epclen + (i += 4));
  payload.set(data, epclen + (++i));

  f_written = false;
  f_success = false;
  var buf = buildPackage(Command.WriteToTag, payload);
  cb(buf);
}

function writeEpc(epc, pass, newepc, cb) {
  var pc = new Uint8Array([0x30, 0x00]); // default is 12 byte
  var newepclen = newepc.length;

  if (newepclen !== 12) {
    var epcwordlen = Math.round(newepclen / 2);
    console.log("newepclen", newepclen);
    pc[0] = epcwordlen << 3;
    if (newepclen % 2)
      ++newepclen;
  }
  var data = new Uint8Array(newepclen + 2);
  data.set(pc);
  data.set(newepc, 2);
  writeToTag(epc, 1, 1, pass, data, cb);
}


