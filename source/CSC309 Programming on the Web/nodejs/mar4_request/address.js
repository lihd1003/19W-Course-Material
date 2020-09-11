'use strict';
const log = console.log

const request = require('request')

const getAddress = (lat, lon, callback) => {
	request({
		url: `https://maps.googleapis.com/maps/api/geocode/json?latlng=${lat},${lon}&key=AIzaSyBmlBarJN3RhKqwNnH4uVwcfydzNwUW-OQ`,
		json: true
	}, (error, response, body) => {
		if (error) {
				callback("Can't connect to server")
			} else if (response.statusCode !== 200) {
				callback('issue with getting resource')
			} else {
				callback(undefined, {
					address: body.results[0].formatted_address
				})
			}
		})
}

module.exports = { getAddress }