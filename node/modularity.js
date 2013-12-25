#!/usr/bin/env nodejs

(function(){
  var local = "this is local, thanks to var";
  
  nonLocal = function() {
    console.log(local);
    return local;
  }
})();

console.log(typeof nonLocal == "function"); // true
console.log(typeof nonLocal() == "string"); // true
/*console.log(local); // not defined*/

