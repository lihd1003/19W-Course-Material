'use strict'
const log = console.log

const { MongoClient, ObjectID } = require('mongodb')

// Deleting Student documents

MongoClient.connect('mongodb://localhost:27017/StudentAPI', (error, client) => {
	if (error) {
		log("Can't connect to mongo server");
	} else {
		console.log('Connected to mongo server')
	}

	const db = client.db('StudentAPI')

	// deleteMany: deletes all documents with certain condition
	db.collection('Students').deleteMany({year: 2}).then((result) => {
		log(result)
	})

	// deleteOne: deletes first document with certain condition
	db.collection('Students').deleteOne({year: 2}).then((result) => {
		log(result)
	})

	// findOneAndDelete: deletes and returns the deleted object
	db.collection('Students').findOneAndDelete({year: 3}).then((result) => {
		log(result)
	})

	// Close the connection
	client.close();
})







