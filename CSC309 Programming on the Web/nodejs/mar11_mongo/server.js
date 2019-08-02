/* server.js - mar11 - 6pm - mongoose*/
'use strict'
const log = console.log

const express = require('express')
const port = process.env.PORT || 3000
const bodyParser = require('body-parser')
const { ObjectID } = require('mongodb')

const { mongoose } = require('./db/mongoose')

// import the model
const { Student } = require('./models/student')

// express
const app = express();
// body-parser middleware - will parse the JSON and convert to object
app.use(bodyParser.json())

// setup a POST route to create a student
app.post('/students', (req, res) => {
	log(req.body)

	const student = new Student({
		name: req.body.name,
		year: req.body.year
	})

	// save student to the database
	student.save().then((result) => {
		// send as a response to the client
		// the object that was saved
		res.send(result)
	}, (error) => {
		res.status(400).send(error) // 400 for bad request
	})


})



app.listen(port, () => {
	log(`Listening on port ${port}...`)
}) 