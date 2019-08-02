/* Jan 23 js - 10am */
"use strict";
const log = console.log;
log('jan 23');

function createCounter() {
	let count = 0;
	return function () {
		count += 1;
		return count;
	}
}

const myCounter = createCounter();
// myCounter();

// another way to define createCounter:
const createCounter2 = function () {
	let count = 0;
	return function () {
		count += 1;
		return count;
	}
}


// setTimeout closure
// setTimeout(f, interval)

// let's start with var
//for (var i = 1; i <= 5; i++) {
//	setTimeout(function () {
// 		log(i);
// 	}, i * 1000);
//}

// for (var i = 1; i <= 5; i++) {
// 	(function () {
// 		var j = i; // j is function scoped in the anon function
//		setTimeout(function () {
// 			log(j);
// 		}, i * 1000);
// 	})();
// }


// let creates a variable with
// a new (block) scope within each iteration
// for (let i = 1; i <= 5; i++) {
// 	setTimeout(function () {
// 		log(i);
// 	}, i * 1000);
//}



//// Objects

const student = {
	name: 'Jimmy',
	year: 2,
	sayName: function() {
		log('My name is ' + this.name + '.')
		// Q: what is the context of this?
		// A: we don't know, until we call it
	}
}

student.sayName();
let mySayName = student.sayName;
// mySayName(); // undefined

// we can get this to work without having
// to explicitly call student.sayName()
// Binding
mySayName = student.sayName;
const boundSayName = mySayName.bind(student)
boundSayName();

const whatYearAmI = function() {
	log(this.year)
}

const student2 = {
	name: 'Saul',
	year: 3,
	myYear: whatYearAmI,
	nested: {
		name: 'Jane',
		year: 7,
		//myYear: whatYearAmI
	}
}

student2.myYear();

const student3 = {
	name: 'Jane',
	year: 7,
	myYear: student2.myYear
}

student3.myYear()
student3.myYear.bind(student2)();
student3.myYear.call(student2)

//student2.nested.myYear();
