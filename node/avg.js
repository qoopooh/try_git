#!/usr/bin/env nodejs

function avg() {
  var sum = 0;
  for (var i = 0, j = arguments.length; i < j; i++) {
    sum += arguments[i];
  }
  return sum / arguments.length;
}

console.log(avg(2, 3, 4, 5));
console.log(avg.apply(null, [3,4,5,6]));

