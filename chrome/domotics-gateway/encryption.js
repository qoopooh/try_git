var uuid = '0123456789abcdef0123456789abcdef01234567';
/*var uuid = '1234567890123456789012345678901234567890';*/
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

/*
Input: ArrayBuffer
*/
function isEncryptedMessage(data) {
var value = new Uint8Array(data)[0];

  if ((value < 48)
      || (value > 57)) {
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

/*
Input: ArrayBuffer
Output: Uint8Array
*/
function getUidBuffer() {
  var uid = new ArrayBuffer(uuid.length * 2);
  var uidView = new Uint8Array(uid);
  var uuidView = new Uint8Array(str2ab(uuid));

  uidView.set(uuidView, 0);
  uidView.set(uuidView, uuidView.length);

  return uidView;
}

/*
 Input: String
 */
function encrypt(data) {
  var inp = new Uint8Array(str2ab(data));
  var out = new ArrayBuffer(data.length + 2);
  var out_u8 = new Uint8Array(out);
  var uid = getUidBuffer();

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
      shift = uid[i-1] + uid[0];
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

  return ab2str(out);
}

/*
 Input: ArrayBuffer + line feed
 */
function decrypt(data) {
  var inp = new Uint8Array(data);
  var out = new ArrayBuffer(data.byteLength + 2);
  var out_u8 = new Uint8Array(out);
  var uid = getUidBuffer();

  i=0;
  while (inp[i+2] != k_line_feed) {
    var dat = inp[i+2];
    if (((dat >= 48) && (dat <= 57))
        || ((dat >= 65) && (dat <= 90))
        || ((dat >= 97) && (dat <= 122))) {
      shift = uid[i+1] + uid[0];
      if (f_debug) {
        console.log('shift: ' + shift);
        console.log('out[' + i + ']: ' + dat);
      }
      while (shift) {
        --shift;
        --dat;
        if (dat < 48)
          dat = 122;
        else if (dat > 57 && dat < 65)
          dat = 57;
        else if (dat > 90 && dat < 97)
          dat = 90;
      }
      if (f_debug) {
        console.log('out[' + i + ']: ' + dat);
      }
    }
    out_u8[i] = dat;
    ++i;
  }

  return ab2str(out);
}

function setUuid(id) {
  /*console.log("id size", id.length);*/

  var id_max_size = 40;
  var id_size = id.length;
  var dat = "";

  uuid = "";
  for (var i = 0; i < id_max_size; i++) {
    if (i < id_size) {
      dat = id[i];
    } else {
      dat = id[i-id_size];
    }

    var num = dat.charCodeAt(0);

    // hex encode
    num %= 16;
    if (num < 10) {
      dat = "" + num;
    } else {
      dat = String.fromCharCode(('a'.charCodeAt(0) + (num - 10)));
    }

    // dec encode
    /*num %= 10;*/
    /*dat = "" + num;*/

    uuid += dat;
  }

  console.log("uuid:", uuid);
  /*console.log("uuid size", uuid.length);*/
}


