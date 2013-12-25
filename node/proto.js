#!/usr/bin/env nodejs

var myObject = function(name) {
  this.name = name;
  return this;
};

myObject.prototype.getName = function(){
  return this.name;
}

console.log(typeof myObject.prototype); // object
console.log(typeof myObject); // function
console.log(myObject.length); // 1
console.log(myObject instanceof Function); // true
console.log(myObject == Function); // false
console.log(myObject.__proto__ === Function.prototype); // true

var myInstance = new myObject("foo");
console.log(myInstance.__proto__ === myObject.prototype); // true
console.log(myInstance.__proto__ === Function.prototype); // false
var mySecondInstance = new myObject("bar");
console.log(myInstance.getName()); // foo 
console.log(mySecondInstance.getName()); // bar 
/*console.log(myObject.getName()); // error*/


/*console.log(myObject()); // so long */


