'use strict';
const log = console.log
log('Mar. 4 - 6pm')
// const requuire = require;

const request = require('request')

const bikes = require('./bike-location-promise')
const address = require('./address-promise')

//bikes.stationInformation(7000)

/*
bikes.stationInformation(7000, (errorMessage, result) => {
	if (errorMessage) {
		log(errorMessage)
	} else {
		// we can now log the result and know that it exists
		log(result)
		address.getAddress(result.lat, result.lon, (errorMessage, addressResult) => {
			if (errorMessage) {
				log(errorMessage)
			} else {
				// we can now log the result and know that it exists
				log(`The address for bike station ${result.name} is ${addressResult.address}`)
			}
		})
	}
})

// Callback hell - not so nice
*/

bikes.stationInformation(7000).then((result) => {
	log(result)
	return address.getAddress(result.lat, result.lon)
}).then((addressResult) => {
	log(`The address for the bike station is ${addressResult.address}`)
}).catch((error) => {
	log(error)
})
















