/* Reservations.js */
'use strict';

const log = console.log
const fs = require('fs');
const datetime = require('date-and-time')

const startSystem = () => {

	let status = {};

	try {
		status = getSystemStatus();
	} catch(e) {
		status = {
			numRestaurants: 0,
			totalReservations: 0,
			currentBusiestRestaurantName: null,
			systemStartTime: new Date(),
		}

		fs.writeFileSync('status.json', JSON.stringify(status))
	}

	return status;
}

/*********/


// You may edit getSystemStatus below.  You will need to call updateSystemStatus() here, which will write to the json file
const getSystemStatus = () => {
	updateSystemStatus();
	const status = fs.readFileSync('status.json')
	return JSON.parse(status)
}

/* Helper functions to save JSON */
const updateSystemStatus = () => {

	/* Add your code below */
	const status = {
		numRestaurants: getAllRestaurants().length,
		totalReservations: getAllReservations().length,
		currentBusiestRestaurantName: "",
		systemStartTime: new Date()
	}

	const res = getAllRestaurants();
	let max_res = -1;
	res.forEach((e) => {
		if (e.numReservations > max_res) {
			status.currentBusiestRestaurantName = e.name;
			max_res = e.numReservations;
		}
	});

	try {
		const old_status = JSON.parse(fs.readFileSync('status.json'));
		if (old_status.systemStartTime){
			status.systemStartTime = old_status.systemStartTime;
		}
	} catch(e) {
		return ;
	}

	fs.writeFileSync('status.json', JSON.stringify(status))
}

const saveRestaurantsToJSONFile = (restaurants) => {
	/* Add your code below */
	fs.writeFileSync('restaurants.json', JSON.stringify(restaurants));
};

const saveReservationsToJSONFile = (reservations) => {
	/* Add your code below */
	fs.writeFileSync('reservations.json', JSON.stringify(reservations))
};

/*********/

// Should return an array of length 0 or 1.
const addRestaurant = (name, description) => {
	// Check for duplicate names
	if (getRestaurantByName(name) != null){
		return [];
	}

	// if no duplicate names:
	const restaurants = getAllRestaurants();

	const restaurant = {name, description, numReservations: 0}

	restaurants.push(restaurant);
	saveRestaurantsToJSONFile(restaurants)

	return [restaurant];
}

// should return the added reservation object
const addReservation = (restaurant, time, people) => {

	/* Add your code below */
	const reservations = getAllReservations();
	const time_parse = new Date(time);

	const restaurants = getAllRestaurants();
	const find_res = restaurants.find((res) => res.name === restaurant);
	find_res.numReservations += 1;
	saveRestaurantsToJSONFile(restaurants);

	const reservation = {restaurant, time: time_parse, people};
	reservations.push(reservation);
	saveReservationsToJSONFile(reservations);


	return reservation;
}


/// Getters - use functional array methods when possible! ///

// Should return an array - check to make sure restaurants.json exists
const getAllRestaurants = () => {
	/* Add your code below */
	try {
		return JSON.parse(fs.readFileSync('restaurants.json'));
	} catch(e) {
		return [];
	}
};

// Should return the restaurant object if found, or an empty object if the restaurant is not found.
const getRestaurantByName = (name) => {
	const restaurants = getAllRestaurants();
	return restaurants.find((res) => res.name === name);
};

// Should return an array - check to make sure reservations.json exists
const getAllReservations = () => {
  /* Add your code below */
	try {
		return JSON.parse(fs.readFileSync('reservations.json'));
	} catch(e) {
		return [];
	}
};

// Should return an array
const getAllReservationsForRestaurant = (name) => {
	/* Add your code below */
 const reservations = getAllReservations();
 const chosen = reservations.filter((r) => r.restaurant === name);
 return chosen.sort(function(a, b){
	 return Date.parse(a.time) - Date.parse(b.time);
 });
};


// Should return an array
const getReservationsForHour = (time) => {
	/* Add your code below */
	const reservations = getAllReservations();
	const time_range = new Date(time);
	const time_range2 = datetime.addHours(time_range, 1);

	return reservations.filter(function(r){
		let time_reserv = new Date(r.time);
		return time_reserv >= time_range && time_reserv <= time_range2;
	});
}

// should return a reservation object
const checkOffEarliestReservation = (restaurantName) => {
	const reservations = getAllReservations();
	if (getAllReservationsForRestaurant(restaurantName).length > 0){
		const want = getAllReservationsForRestaurant(restaurantName)[0];
		reservations.splice(reservations.findIndex((x) => {
			return x.time == want.time;
		}), 1);
		// reservations.splice(reservations.indexOf(want), 1);
		saveReservationsToJSONFile(reservations);

		const restaurants = getAllRestaurants();
		const find_res = restaurants.find((res) => res.name === restaurantName);
		find_res.numReservations -= 1;
		saveRestaurantsToJSONFile(restaurants);

		return want;
	}
 	return null;
}


const addDelayToReservations = (restaurant, minutes) => {
	// Hint: try to use a functional array method
	const reservations = getAllReservations();
	const want = reservations.filter((e) => e.restaurant === restaurant)
	want.forEach((e) => {
		e.time = new Date(e.time);
		e.time = datetime.addMinutes(e.time, minutes);
	})
	saveReservationsToJSONFile(reservations);
	return getAllReservationsForRestaurant(restaurant);

}

startSystem(); // start the system to create status.json (should not be called in app.js)

// May not need all of these in app.js..but they're here.
module.exports = {
	addRestaurant,
	getSystemStatus,
	getRestaurantByName,
	getAllRestaurants,
	getAllReservations,
	getAllReservationsForRestaurant,
	addReservation,
	checkOffEarliestReservation,
	getReservationsForHour,
	addDelayToReservations
}
