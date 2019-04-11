/* E3 app.js */
'use strict';

const log = console.log
const yargs = require('yargs').option('addRest', {
    type: 'array' // Allows you to have an array of arguments for particular command
  }).option('addResv', {
    type: 'array'
  }).option('addDelay', {
    type: 'array'
  })

const reservations = require('./reservations');

// datetime available if needed
const datetime = require('date-and-time')

const yargs_argv = yargs.argv
//log(yargs_argv) // uncomment to see what is in the argument array

if ('addRest' in yargs_argv) {
	const args = yargs_argv['addRest']
	const rest = reservations.addRestaurant(args[0], args[1]);
	if (rest.length > 0) {
		/* complete */
    log("Added restaurant "+rest[0].name+".");
	} else {
		/* complete */
    log("Duplicate restaurant not added.");
	}
}

if ('addResv' in yargs_argv) {
	const args = yargs_argv['addResv']
	const resv = reservations.addReservation(args[0], args[1], args[2]);
	const time_parse = new Date(resv.time);
  const reformat = datetime.format(time_parse, "MMMM D YYYY at h:m A")
  log("Added reservation at "+resv.restaurant+" on "+reformat+" for "+resv.people)

}

if ('allRest' in yargs_argv) {
	const restaurants = reservations.getAllRestaurants(); // get the array

	// Produce output below
  restaurants.forEach((e) => {
    log(e.name+": "+e.description+" - "+e.numReservations+" active reservations");
  });
}

if ('restInfo' in yargs_argv) {
	const restaurants = reservations.getRestaurantByName(yargs_argv['restInfo']);

	// Produce output below
  log(restaurants.name+": "+restaurants.description+" - "+restaurants.numReservations+" active reservations");
}

if ('allResv' in yargs_argv) {
	const restaurantName = yargs_argv['allResv']
	const reservationsForRestaurant = reservations.getAllReservationsForRestaurant(restaurantName); // get the arary

	// Produce output below
  log("Reservations for " + restaurantName);
  reservationsForRestaurant.forEach((e) => {
    const time_parse = new Date(e.time);
    const time_string = datetime.format(time_parse, "MMM D YYYY, h:mm A");
    log("- "+time_string+", table for "+e.people)
  });
}

if ('hourResv' in yargs_argv) {
	const time = yargs_argv['hourResv']
	const reservationsForRestaurant = reservations.getReservationsForHour(time); // get the arary

	// Produce output below
  log("Reservations in the next hour:");
  reservationsForRestaurant.forEach((e) => {
    const time_parse = new Date(e.time);
    const time_string = datetime.format(time_parse, "MMM D YYYY, h:mm A");
    log("- "+e.restaurant+": "+time_string+", table for "+e.people);
  })
}

if ('checkOff' in yargs_argv) {
	const restaurantName = yargs_argv['checkOff']
	const earliestReservation = reservations.checkOffEarliestReservation(restaurantName);
	// Produce output below
  const time_parse = new Date(earliestReservation.time);
  const time_string = datetime.format(time_parse, "MMM D YYYY, h:mm A");
  log("Checked off reservation on "+time_string+", table for "+earliestReservation.people);
}

if ('addDelay' in yargs_argv) {
	const args = yargs_argv['addDelay']
	const resv = reservations.addDelayToReservations(args[0], args[1]);

	// Produce output below
  log("Reservations for " + args[0]);
  resv.forEach((e) => {
    const time_parse = new Date(e.time);
    const time_string = datetime.format(time_parse, "MMM D YYYY, h:mm A");
    log("- "+time_string+", table for "+e.people)
  });
}

if ('status' in yargs_argv) {
	const status = reservations.getSystemStatus()

	// Produce output below
  log("Number of restaurants: " + status.numRestaurants);
  log("Number of total reservations: " + status.totalReservations);
  log("Busiest restaurant: " + status.currentBusiestRestaurantName);
  const time_parse = new Date(status.systemStartTime);
  const time_string = datetime.format(time_parse, "MMM D YYYY, h:mm A");
  log("System started at: " + time_string);
}
