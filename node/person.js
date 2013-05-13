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
  this.fullName = function() {
    return this.first + ' ' + this.last;
  }
  this.fullNameReversed = function() {
    return this.last + ', ' + this.first;
  }
}

var p = new Person("Phuchit", "Kanjanakhan");
console.log(p.fullNameReversed());


