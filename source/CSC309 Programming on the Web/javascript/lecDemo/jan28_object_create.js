/* Jan 28 js */ 
"use strict";
const log = console.log;
log('jan 28');

function sayName() {
	log("My name is " + this.firstName);
}

// sayName(); //undefined 

const person = {
	sayName: sayName
}

person.sayName();
// My name is undefined

const student = {
	firstName: 'James'
}

//student.sayName()
// Error: not a function

Object.setPrototypeOf(student, person)
student.sayName();
// My name is James

const teacher = {
	firstName: 'Paul'
}

Object.setPrototypeOf(teacher, person)
teacher.sayName();
// My name is Paul

// Chaining prototypes
const partTimeStudent = {
	numCourses: 2
}

Object.setPrototypeOf(partTimeStudent, student)
partTimeStudent.sayName()
// My name is James

person.sayName = function () {
	log("MY NAME IS " + this.firstName);
}

partTimeStudent.sayName()
// MY NAME IS James (since the prototype changed)



//////



// a constructor function
function Student(firstName, lastName) {
	this.firstName = firstName;
	this.lastName = lastName;
	this.say = function() {
		log('Say ' + this.firstName)
	}
}

Student.prototype.sayLastName = function() {
	log('My last name is ' + this.lastName);
}

const student2 = new Student('Jimmy', 'Parker')
student2.sayLastName()
// My last name is Parker
student2.say()
// Say Jimmy

// Object.create
const student3 = Object.create(student)
student3.sayName()
student3.firstName = "Jimmy"
student.sayName()
// James
student3.sayName()
// Jimmy

// class keyword

class Instructor {
	constructor(firstName, course) {
		this.firstName = firstName
		this.course = course
	}

	whatsMyCourse() {
		return this.course
	}
} 

// function Instructor(firstName, course) {
// 		this.firstName = firstName
// 		this.course = course
// }

// Instructor.prototype.whatsMyCourse = function() {
// 		return this.course
// }

const jen = new Instructor('Jen', 'CSC108')
log(jen.whatsMyCourse())

class Person {
	constructor(firstName) {
		this.firstName = firstName
	}
}


/// More Object-oriented syntactic sugar below.
// Under the hood, this is still prototypal delegation
class Instructor2 extends Person {
	constructor(firstName, course) {
		super(firstName)
		this.firstName = firstName
		this.course = course
	}

	whatsMyCourse() {
		return this.course
	}
}

const jen2 = new Instructor2('Jen2', 'CSC108')
 

































