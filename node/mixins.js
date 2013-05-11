var counter=0;
function Tracked() { this.id = counter++; }
function Logged() { this.log = function() { console.log(this.id); }; }

function ItemType() { Tracked.call(this); Logged.call(this); }
ItemType.prototype = Object.create(null, { type: { value: "generic" }});

function SubItemType() { ItemType.call(this); }
SubItemType.prototype = Object.create(ItemType.prototype, { type: { value: "sub" }});

var i = new ItemType();
var i2 = new ItemType();
var si = new SubItemType();

console.log('counter', counter);
console.log('i.id', i.id); // 0
console.log('i.type', i.type); // "generic"
console.log('i2.id', i2.id); // 1
console.log('i2.type', i2.type); // "generic"
console.log('si.id', si.id); // 2
console.log('si.type', si.type); // "sub"

