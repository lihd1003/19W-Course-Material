/* Lab 3 JS */
'use strict';

// We are a Toyota factory
const toyota = {
  brand: 'Toyota',
  color: 'red',
  type: 'sedan',
  seats: 5,
};

// We will now make a car object that will act as a prototype

const car = {
	type: 'car', // a default car type
    describe: function() {  
     console.log(`This is a ${this.color} ${this.brand} ${this.type} that has ${this.seats} seats.`);
  } 
}

Object.setPrototypeOf(toyota, car)
toyota.describe()

console.log(toyota.__proto__ === car) // true

// 'constructor'
function Toyota(color, type, seats) {
	this.brand = 'Toyota'
	this.color = color
	this.type = type
	this.seats = seats
}

// We are setting the prototype of the constructor function
Toyota.prototype = car 

const myToyota = new Toyota('black', 'SUV', 7)
myToyota.describe()

const mySecondToyota = new Toyota('lime green', 'convertible', 2)
mySecondToyota.describe()

// Add to the prototype a function for car
car.seatsFive = function() {
	return this.seats >= 5
}

console.log(myToyota.seatsFive())
console.log(mySecondToyota.seatsFive())

myToyota.__proto__.describe()
Toyota.prototype.describe()
Toyota.prototype.describe.bind(mySecondToyota)()

// Attach a delegate prototype using Object.create
const myThirdToyota = Object.create(car) // add the car prototype to the object
Toyota.bind(myThirdToyota)('red', 'minivan', 7) 
myThirdToyota.describe()

class CarClass {}
console.log(typeof(CarClass)) // it's a function - the constructor
console.dir(CarClass)

class Auto {
	constructor() {
		this.type = 'car'
	}

	describe() {	
	   console.log(`This is a ${this.color} ${this.brand} ${this.type} that has ${this.seats} seats.`);
	}
}

console.dir(Auto)
 
class Tesla extends Auto { 
	constructor(color, type, seats) {
		super()
		this.brand = 'Tesla'
		this.color = color
		this.type = type
		this.seats = seats
	}
}

console.dir(Tesla) // observe the prototype
const myTesla = new Tesla('grey', 'crossover', 5)
myTesla.describe()


