var Aia = (function () {
  /** Private variables **/
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
    calPayment: function (fund, age, sex, type) {
      var s = Sex[sex];
      var r = 0;
      var res = 0;

      if (fund < 0)
        fund = 0;
      if (age < 0)
        age = 0;
      if (!s)
        s = 0;
      switch (type) {
        /*case '15p25': r = p1525[(age * 2) + s]; break;*/
        case '20plnp': r = plnp20[(age * 2) + s]; break;
        case '20plp':
        default: r = plp20[(age * 2) + s]; break;
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


