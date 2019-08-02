'use strict'
const log = console.log

const {MongoClient, ObjectID} = require('mongodb')

// Fetching Student documents

MongoClient.connect('mongodb://localhost:27017/StudentAPI', (error, client) => {
	if (error) {
		log("Can't connect to mongo server");
	} else {
		console.log('Connected to mongo server')
	}

	const db = client.db('StudentAPI')

	db.collection('Students').findOneAndUpdate({
		_id: new ObjectID('5bf0f87c47dc1d4dc4c3f04d')
	}, {
		// update operators: $set and $inc
		$set: { 
			name: 'Kelly',
		}, 
		$inc: {
			year: 1
		}
	}, {
		returnOriginal: false // gives us back updated arguemnt
	}).then((result) => {
		log(result)
	});

	// Close the connection
	client.close();
})







