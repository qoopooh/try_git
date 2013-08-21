
/** Class Foo **/
var Foo = (function () {
  // Private variables
  var private_number = 200
  /** class Constructor **/
  var Foo = function () {
    this.bar = 0;
  };
  /** class methods **/
  Foo.prototype = {
    add: function () {
      this.bar += private_number;
    }
  };
  return Foo;
})();

/*foo = new Foo();*/
/*alert(foo.bar); // 0*/
/*foo.add(); */
/*alert(foo.bar);// 200*/
/*alert(foo.private_number) //undefined*/

var Aia = (function () {
  // Private variables
  var res = 0;
  var Sex = {
    'male': 0,
    'female': 1
  };

  /** class Constructor **/
  var Aia = function () {
    this.rate = 0;
    this.result = 0;
  };

  /** class methods **/
  Aia.prototype = {
    calPayment: function (fund, year, sex, type) {
      var s = Sex[sex];
      var r = 0;

      if (fund < 0)
        fund = 0;
      if (year < 0)
        year = 0;
      if (!s)
        s = 0;
      switch (type) {
        /*case '15p25': r = p1525[(year * 2) + s]; break;*/
        case '20plnp': r = plnp20[(year * 2) + s]; break;
        case '20plp':
        default: r = plp20[(year * 2) + s]; break;
      }
      if (!r)
        r = 1000;

      res = (fund * r) / 1000;
      this.rate = r;
      this.result = res;
      return res;
    },
    calFund: function () {
      this.result = res;
      return res;
    }
  };

  return Aia;
})();


