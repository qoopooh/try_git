(function () {
  (function () {
    Converter = function() {
    };

    /**
      * input: HEX String Array
      * output: String
      */
    Converter.prototype.hexStringToAscii = function(words, offset, len) {
      var str = '';
      var end = words.length;

      if (!offset)
        offset = 0;
      if (len > 0)
        end = offset + len;
      for (var i = offset; i < end; ++i) {
        str += String.fromCharCode(parseInt(words[i], 16));
      }

      return str;
    };

    /**
      * input: Uint8
      * output: HEX String (00)
      */
    Converter.prototype.hexString = function(uint8) {
      var text = uint8.toString(16).toUpperCase();
      if (text.length === 1)
        text = '0' + text;
      return text;
    }

    /**
      * input: Uint8Array
      * output: HEX String
      */
    Converter.prototype.abv2hexstr = function(buf) {
      var msg = "";

      if (buf.length < 1)
        return msg;

      msg = this.hexString(buf[0]);
      for (var i=1; i<buf.length; i++) {
        msg += " " + this.hexString(buf[i]);
      }
      return msg;
    }

    /**
      * input: ArrayBuffer
      * output: String
      */
    Converter.prototype.ab2str = function(buf) {
      return String.fromCharCode.apply(null, new Uint8Array(buf));
    }

    /**
      * input: String
      * output: ArrayBuffer
      */
    Converter.prototype.str2ab = function(str) {
      var buf = new ArrayBuffer(str.length);
      var bufView = new Uint8Array(buf);
      for (var i=0, strLen=str.length; i<strLen; i++) {
        bufView[i] = str.charCodeAt(i);
      }
      return buf;
    }
  }());
}());

