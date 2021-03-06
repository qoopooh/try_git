
function Person(gender) {
  this.gender = gender;
  console.log('Person instantiated');
}

Person.prototype.sayHello = function() {
  console.log('hello');
}

Person.prototype.sayGender = function() {
  console.log(this.gender);
}

Person.prototype.walk = function() {
  console.log('I am walking!');
}

function Student(gender) {
  /*Person.call(this);*/
  this.gender = gender;
}

/*Student.prototype = new Person();*/
Student.prototype = Object.create(Person.prototype); // Replace above line
Student.prototype.constructor = Student;

Student.prototype.sayHello = function() {
  console.log('hi, I am a student');
}

Student.prototype.sayGoodBye = function() {
  console.log('good bye');
}

function initPerson() {
  /*$("#msg").innerText = "Hi"; // does not work*/
  var person1 = new Person('Male');
  var genderTeller = person1.sayGender;

  console.log('person1 is a ' + person1.gender);
  person1.sayGender();
  genderTeller();
  genderTeller.call(person1);
  console.log(genderTeller === person1.sayGender);
  console.log(genderTeller === Person.prototype.sayGender);
}

function initStudent() {
  var student1 = new Student('Girl');
  student1.sayHello();
  student1.walk();
  student1.sayGender();
  console.log(student1 instanceof Person);
  console.log(student1 instanceof Student);
}

function init() {
  /*initPerson();*/
  /*initStudent();*/
  var foo = function(){};
  console.log('foo is a Function: ' + (foo instanceof Function));
  console.log('foo.prototype is an Object: ' + (foo.prototype instanceof Object));
}

$(document).ready(init());

