"use strict";

const log = console.log;
log('jan 23 10am');
/*
function foo(){
	let a = 2;
    function inner(){
    	document.write(a);
    }
    a = 5;
    return inner;
}
let bar = foo();
document.write(bar());
*/

/*
function createCounter() {
	let count = 0;
	return function() {
		count += 1;
		return count;
	}
}

const myCounter = createCounter();
myCounter();
*/
/*
for (var i = 1; i <= 5; i++) {
	setTimeout(function () {
		log(i) // always output 6
	}, i * 1000)
}*/

// wrap it in an IFFE
/*
for (var i = 1; i <= 5; i++) {
	(function() {
		var j = i;
		setTimeout(function () {
			log(j) // always output 6
		}, i * 1000)
	})();
}*/

// better to use let to create a new scopr within each iteration
/*
for (let i = 1; i <= 5; i++) {
	setTimeout(function () {
		log(i) // always output 6
	}, i * 1000)
}*/

const student = {
	name: 'J', year: 2, sayName: function() { log(this.name) }
}

student.sayName()
let mySayName = student.sayName;
// This will cause an error 
// mySayName()

// bind 'this' context
const boundSayName = mySayName.bind(student);
boundSayName();

const whatYearAmI = function() {
	log(this.year)
}

const s2 = {
	name: 'S',
	year: 3,
	myYear: whatYearAmI
}

s2.myYear()

const s3 = {
	name: 'Jenny',
	year: 7,
	myYear: s2.myYear
}

s3.myYear()
s3.myYear.bind(s2)()
s3.myYear.bind(s2);