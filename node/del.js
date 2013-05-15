/*Run on Nodejs*/
var log = console.log;
var delegate = { item: "shadow value" };

var obj = Object.create(delegate, { item: {value:"value"}}); // new item
var obj2 = Object.create(delegate); // seems reference

log(delegate.item == "shadow value"); //true
log(obj2.item == "shadow value"); //true

delegate.item = "new shadow value";
log(obj2.item == "new shadow value"); //true

obj2.item = "even newer!";
log(delegate.item == "new shadow value"); //true
log(obj2.item == "even newer!"); //true
log(obj.item == "value"); //true


