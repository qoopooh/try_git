function makePerson(first, last) {
  return {
      first: first,
      last: last
  }
}
function personFullName(person) {
  return person.first + ' ' + person.last;
}
function personFullNameReverse(person) {
  return person.last + ', ' + person.first;
}

var s = makePerson("Simon", "Willison");
console.log(personFullName(s));
console.log(personFullNameReverse(s));

function Person(first, last) {
  this.first = first;
  this.last = last;
}
Person.prototype.fullName = function() {
  return this.first + ' ' + this.last;
}
Person.prototype.fullNameReversed = function() {
  return this.last + ', ' + this.first;
}
Person.prototype.firstNameCaps = function() {
  return this.first.toUpperCase();
}
Person.prototype.reversed = function() {
  var r = "";
  var full = this.fullName();
  for (var i = full.length - 1; i >= 0; i--) {
    r += full[i];
  }

  return r;
}
Person.prototype.toString = function() {
  return '<Person: ' + this.fullName() + '>';
}
function lastNameCaps() {
  return this.last.toUpperCase();
}

var p = new Person("Phuchit", "Kanjanakhan");
console.log(p.firstNameCaps());
console.log(p.reversed());
console.log(p.toString());
console.log(lastNameCaps);
console.log(lastNameCaps.call(p));
console.log(personFullNameReverse.apply(null, [p]));

