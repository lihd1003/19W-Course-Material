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

	/// A 'select all' query to get all the documents
	// toArray(): promise based function that gives the documents
	db.collection('Students').find().toArray().then((documents) => {
		log(documents)
	}, (error) => {
		log("Can't fetch Students", error)
	})

	// To select based on a condition, add object argument to find()
	db.collection('Students').find({year: 4}).toArray().then((documents) => {
		log(documents)
	}, (error) => {
		log("Can't fetch Students", error)
	})

	// To select based on object id, we have to call the ObjectID function on
	// the id string
	db.collection('Students').find({_id: new ObjectID('5befd1075a0bf001c3d78c9b')}).toArray().then((documents) => {
		log(documents)
	}, (error) => {
		log("Can't fetch Students", error)
	})


	// Aggregation function: count
	db.collection('Students').find().count().then((number) => {
		log(number)
	}, (error) => {
		log("Can't fetch Students", error)
	})


	// Close the connection
	client.close();
})







