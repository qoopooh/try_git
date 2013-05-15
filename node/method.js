var log = console.log;
var obj = {
  item: "value",
  method: function() {
    log(this.item);
  }
};

log('property de-reference:', obj.item === obj["item"]);
log('scope modifier:', obj.method() === obj["method"].call(obj));

