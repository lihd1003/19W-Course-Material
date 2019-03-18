// Some Functional JS
const log = console.log

students = [
	{name: 'Bob', year: 3},
	{name: 'Kelly', year: 2},
	{name: 'Randy', year: 3},
	{name: 'Jimmy', year: 1},
	{name: 'Betty', year: 4},
	{name: 'Clara', year: 3}
]

// How can we make an array of all 3rd year students?
/*
const thirdYears = []
for (let i = 0; i < students.length; i++) {
	if (students[i].year === 3) {
		thirdYears.push(students[i])
	}
}

log(thirdYears)
*/

// the for loop is a lot of code.
// We can take advantage of JavaScript's first-class functions
// to write this in a much easier way


/// Functional array methods
// filter
// takes a callback function and an argument that is assigned
// to each element of the array
// it adds to the new array any element that passes a test (returns true)

// const isThirdYear = function (student) {
// 	return student.year === 3
// }

const thirdYearsFunctional = students.filter(function (student) {
	return student.year === 3
})

log(thirdYearsFunctional)


////////////////
// How can we make an array of all of the student names?
const studentNames = []
for (let i = 0; i < students.length; i++) {
	studentNames.push(students[i].name)
}

log(studentNames)

const studentNamesFunctional = students.map(function(student) {
	return student.name
})

log(studentNamesFunctional)


// Really cool: Arrow functions
// A different syntax for making function expressions that
// saves time and often makes code easier to read

const square = function(x) {
	return x * x
}

//const squareA = (x) => { return x * x }
//const squareA = (x) => x * x;
const squareA = x => x * x;

log(squareA(5))

const noArgs = () => 6
log(noArgs())

const multipleArgs = (a, b) => a + b;
log(multipleArgs(1, 2))


// Now let's use arrow functions with filter and map
const thirdYearsArrow = students.filter((student) => student.year === 3 )

log(thirdYearsArrow)

// MUCH less code than the for loop and more readable

// map with arrow
const studentNamesArrow = students.map((student) => student.name)
log(studentNamesArrow)

/// Be careful with arrow functions. They *do not* bind 'this'.

const s = {
	name: 'Jimmy',
	sayName: () => log(this.name)
}

//s.sayName() // 'this' is not bound to s when using arrow functions
// functional programming is in conflict with Object oriented approach

////
// More generic funcional method: reduce()
// filter and map are specific functional array methods
// reduce can implement them and do way more

const accounts = [
	{ balance: 5 },
	{ balance: 10 },
	{ balance: -3 }
]

// How do we sum all of the balances?

// reduce: takes a function and optional arguments that
// can be changed and then re-fed into the function
// after every object in the array
const totalBalance = accounts.reduce(function (total, account) {
	log(total)
	return total + account.balance
}, 0)

log(totalBalance)

const totalBalanceArrow = accounts.reduce((total, account) => total + account.balance, 0)

log(totalBalanceArrow)












