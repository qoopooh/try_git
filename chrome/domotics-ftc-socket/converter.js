var Converter = function() {};
Converter.prototype = (function() {
  var me = {};
  me.publicVariable = "foo bar";
  var privateVariable = 42;

  me.hexStringToAscii = function(words, offset, len) {
    var str = '';
    if (!offset)
      offset = 0;
    if (!len)
      len = words.length;
    for (var i = offset; i < len; ++i) {
      str += String.fromCharCode(parseInt(words[i], 16));
    }

    return str;
  };

  return me;
})();
