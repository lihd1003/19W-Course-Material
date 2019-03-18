'use strict';
const log = console.log

const request = require('request')

const stationInformation = (stationId, callback) => {

	request({
		url: 'https://tor.publicbikesystem.net/ube/gbfs/v1/en/station_information',
		json: true
	}, (error, response, body) => { // callback fires once response comes back

		//log(response.statusCode)

		// Two errors:
		//  one that occurs because we couldn't connect to a server
		//  one that occurs when we can connect, but can't get the resource we want (like 404 status code)

		if (error) {
			callback("Can't connect to server")
		} else if (response.statusCode !== 200) {
			callback('Issue with getting resource')
		} else {
			const stations = body.data.stations

			const stationFilter = stations.filter(x => x.station_id === stationId.toString())

			if (stationFilter.length !== 0 ) {
				callback(undefined, {
					name: stationFilter[0].name,
					lat: stationFilter[0].lat,
					lon: stationFilter[0].lon
				})

				// log(stationFilter[0].name)
			 //    log(stationFilter[0].lat)
			 //    log(stationFilter[0].lon)
			}
			

		}

	})

}



module.exports = { stationInformation }