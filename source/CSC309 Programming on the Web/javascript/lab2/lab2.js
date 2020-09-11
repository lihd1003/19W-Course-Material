// Q1
var b = 1;
function outer() {
	var b = 2
	function inner() {
		b++
		var b = 3
		console.log(b)
	}
	inner()
}
outer();

// output 
// 3


// Q2 Create makeAdder that 
// returns a function that adds to a given number

function makeAdder(n) {
	return function(m) {
		console.log(m + n)
	}
}

const addTwo = makeAdder(2)
addTwo(3)

function makeAdder2(n) {
	function sub(m) {
		console.log(m + n)
	}
	return sub
}

const addThree = makeAdder2(3)
addThree(3);

// Q3

(function(a) {
	return (function(b) {
		console.log(a);
	})(2)
})(1);

// output 1


/*** -----------------Object------------------ ***/
// Q1
const student = {
	name: 'Sally',
	getName: function() {
		return this.name;
	}
};

const g = student.getName;
console.log(g()); // output ""
console.log(student.getName()); // output Sally

// Q2
const num_toppings = function() {
	// console.log("the object is" + this)
	return this.toppings.length
}

const p1 = {
	toppings: ["A", "B", "C"],
	getNumToppings: num_toppings
};

const p2 = {
	toppings: ["D"],
	getNumToppings: p1.getNumToppings
}

console.log(p1.getNumToppings()); // 3
console.log(p2.getNumToppings()); // 1
// console.log(num_toppings()); 
// Error: Cannot read property 'length' of undefined

d = num_toppings.bind(p1)
console.log(d()) // 3

console.log(p1.getNumToppings.call(p2)) // 1


// Q3

const students = {
	student1: {
		name: "James",
		friend: {
			name: "Jimmy"
		}
	}, 
	student2: {
		name: "Jen",
		getName: function() {
			return this.name
		}
	}
}

// get student1's friend's name by using student2's getName
console.log(students.student2.getName.call(students.student1.friend))

// function.call(object, parameters)