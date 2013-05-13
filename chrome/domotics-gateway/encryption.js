/*var uuid = '0123456789abcdef0123456789abcdef01234567';*/
var uuid = '1234567890123456789012345678901234567890';
var k_line_feed = 13;
var phone_id = '0';
var f_debug = false;

// others data will return true
function isEncryptionCommand(data) {
  var code = data[2];

  if ((code == 'U')
      || (code == 'V')
      || (code == 'N')) {
    return false;
  }
  return true;
}

function setPhoneId(id, cb) {
  phone_id = id;
  console.log("Set phone id to", phone_id);
  if (cb)
    return cb();
}

function getUidBuffer() {
  var uid = new Buffer(uuid.length * 2);

  len = uid.write(uuid, 0);
  len += uid.write(uuid, len);
  
  return uid;
}

/*
 Input: String
 */
function encrypt(data) {
  var inp = new Uint8Array(str2ab(data));
  var out = new ArrayBuffer(data.length + 2);
  var out_u8 = new Uint8Array(out);

  out_u8[0] = phone_id.charCodeAt(0);
  out_u8[1] = ','.charCodeAt(0);
  if (f_debug) {
    console.log('out[0]: ' + out_u8[0]);
    console.log('out[1]: ' + out_u8[1]);
    console.log('data.length: ' + data.length);
    console.log('data: ' + data);
  }

  i = 2;
  while (i < data.length + 2) {
    var dat = inp[i-2];

    if (f_debug)
      console.log(i, dat);
    if (((dat >= 48) && (dat <= 57))
        || ((dat >= 65) && (dat <= 90))
        || ((dat >= 97) && (dat <= 122))) {
      shift = uuid.charCodeAt(i-1) + uuid.charCodeAt(0);
      if (f_debug) {
        console.log('shift: ' + shift);
        console.log('out[' + i + ']: ' + dat);
      }
      while (shift) {
        --shift;
        ++dat;
        if (dat > 57 && dat < 65)
          dat = 65;
        else if (dat > 90 && dat < 97)
          dat = 97;
        else if (dat > 122)
          dat = 48;
      }
    }
    out_u8[i] = dat;
    if (f_debug)
      console.log('out[' + i + ']: ' + dat);
    ++i;
  }

  var out_str = ab2str(out);
  console.log("encrypt", out_str);
  return out_str;
}

/*
 Input: Buffer + line feed
 */
function decrypt(data) {
  var inp = new Buffer(data);
  var out = new Buffer(data.length - 2);

  i=0;
  while (inp[i+2] != k_line_feed) {
    out[i] = inp[i+2];
    if (((out[i] >= 48) && (out[i] <= 57))
        || ((out[i] >= 65) && (out[i] <= 90))
        || ((out[i] >= 97) && (out[i] <= 122))) {
      shift = uuid.charCodeAt(i+1) + uuid.charCodeAt(0);
      if (f_debug) {
        console.log('shift: ' + shift);
        console.log('out[' + i + ']: ' + out[i]);
      }
      while (shift) {
        --shift;
        --out[i];
        if (out[i] < 48)
          out[i] = 122;
        else if (out[i] > 57 && out[i] < 65)
          out[i] = 57;
        else if (out[i] > 90 && out[i] < 97)
          out[i] = 90;
      }
      if (f_debug) {
        console.log('out[' + i + ']: ' + out[i]);
      }
    }
    ++i;
  }

  return out;
}

