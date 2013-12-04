var uuid = '0123456789abcdef0123456789abcdef01234567';
/*var uuid = '1234567890123456789012345678901234567890';*/
var k_line_feed = 13;
var log = console.log;
var phone_id = '0';
var f_debug = false;

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
 Input: Buffer
 */
function isEncryptedMessage(data) {
  var value = data[0];

  if ((value < 48)
      || (value > 57)) {
    return false;
  }
  return true;
}

function setPhoneId(id) {
  phone_id = id;
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
  var inp = new Buffer(data);
  var out = new Buffer(data.length + 2);
  var uid = getUidBuffer();

  out[0] = phone_id.charCodeAt(0);
  out[1] = ','.charCodeAt(0);
  if (f_debug) {
    log('out[0]: ' + out[0]);
    log('out[1]: ' + out[1]);
    log('data.length: ' + data.length);
  }

  i = 2;
  while (i < data.length + 2) {
    out[i] = inp[i-2];
    if (((out[i] >= 48) && (out[i] <= 57))
        || ((out[i] >= 65) && (out[i] <= 90))
        || ((out[i] >= 97) && (out[i] <= 122))) {
      shift = uid[i-1] + uid[0] - 0x60;
      if (f_debug) {
        log('shift: ' + shift);
        log('out[' + i + ']: ' + out[i]);
      }
      while (shift) {
        --shift;
        ++out[i];
        if (out[i] > 57 && out[i] < 65)
          out[i] = 65;
        else if (out[i] > 90 && out[i] < 97)
          out[i] = 97;
        else if (out[i] > 122)
          out[i] = 48;
      }
      if (f_debug) {
        log('out[' + i + ']: ' + out[i]);
      }
    }
    ++i;
  }

  return out.toString();
}

/*
 Input: Buffer + line feed
 */
function decrypt(data) {
  var inp = new Buffer(data);
  var out = new Buffer(data.length - 2);
  var uid = getUidBuffer();

  i=0;
  while (inp[i+2] != k_line_feed) {
    out[i] = inp[i+2];
    if (((out[i] >= 48) && (out[i] <= 57))
        || ((out[i] >= 65) && (out[i] <= 90))
        || ((out[i] >= 97) && (out[i] <= 122))) {
      shift = uid[i+1] + uid[0] - 0x60;
      if (f_debug) {
        log('shift: ' + shift);
        log('out[' + i + ']: ' + out[i]);
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
        log('out[' + i + ']: ' + out[i]);
      }
    }
    ++i;
  }

  return out;
}

module.exports.uuid = uuid;
module.exports.setPhoneId = setPhoneId;
module.exports.isEncryptionCommand = isEncryptionCommand;
module.exports.isEncryptedMessage = isEncryptedMessage;
module.exports.encrypt = encrypt;
module.exports.decrypt = decrypt;

