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
var hbcount = 0;

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
      
      if (tagcount)
        msg = 'EPC: ' + u82hex(payload.subarray(6));
      else
        msg = 'EPC: -';
      aaelog(msg);
      break;
    case Command.HearthbeatInterrupt:
      f_hb = true;
      msg = 'HB ' + ++hbcount;
      aaelog(msg);
      break;
    case Command.InventoryCyclicInterrupt:
      msg = 'EPC: ' + u82hex(payload.subarray(2));
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

function buildPackage(cmd, len, payload) {
  var bufLen = 10;
  if (payload)
    bufLen = payload.length + 11;
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

  var build = "";
  for (var i = 0; i < bufLen; ++i) {
    var val = arr[i];
    if (val < 10)
      build += '0' + val;
    else
      build += val.toString(16);
  }
  console.log('build', build);

  return buf;
}

function getSoftwareRevision(cb) {
  var buf = buildPackage(Command.GetSoftwareRevision, 0);
  cb(buf);
}

function inventorySingle(cb) {
  var buf = buildPackage(Command.InventorySingle, 0);
  cb(buf);
}

function inventoryCyclic(on, cb) {
  if (on)
    var buf = buildPackage(Command.InventoryCyclic, 1, new Uint8Array([1]));
  else
    var buf = buildPackage(Command.InventoryCyclic, 1, new Uint8Array([0]));
  cb(buf);
}

function toggleHeartbeat(cb) {
  f_hb = !f_hb;
  if (f_hb) {
    var buf = buildPackage(Command.SetHeartBeat, 1, new Uint8Array([1]));
    hbcount = 0;
  } else {
    var buf = buildPackage(Command.SetHeartBeat, 1, new Uint8Array([0]));
  }
  cb(buf);
}

