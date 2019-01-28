"use strict";
const log = console.log;
log('jan 28');


/**Prototypes**/


function sayName() {
	log("my name is " + this.firstName);
}

// error
// sayName();

const person = {
	sayName: sayName
}

// undefined
person.sayName();

const student = {
	firstName: "A"
}

// error
// student.sayName()

Object.setPrototypeOf(student, person)

// return the firstName
student.sayName();

const teacher = {
	firstName: "T"
}

Object.setPrototypeOf(teacher, person)
teacher.sayName()

student.age = 26;

const partTimeStudent = {
	numCourses: 2
}

Object.setPrototypeOf(partTimeStudent, student)
partTimeStudent.sayName();

partTimeStudent.sayName = function () {
	log("part time " + this.firstName)
}

partTimeStudent.sayName()


/**Object creatiion using functions**/


// a constructor function
function Student(firstName, lastName) {
	this.firstName = firstName;
	this.lastName = lastName;
}

Student.prototype.sayLastName = function() {
	log("last name: " + this.lastName);
}

const student2 = new Student("Jimmy", "Parker");
student2.sayLastName();

// using Object.create(o)
const student3 = Object.create(student);
student3.sayName();

// ES6, class keyword, 
// actually just a syntac sugar, 
// does the exactly same thing above
class Person {
	constructor(firstName) {
		this.firstName = firstName
	}
}

class Instructor extends Person{
	constructor(fName, course) {
		super(fName)
		this.course = course;
	}
	
	whatsMyCourse() {
		return this.course
	}
}

const jen = new Instructor("Jen", "CSC108");
log(jen.whatsMyCourse())

