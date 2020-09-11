/* server.js - mar13 - 10am - mongoose*/
'use strict'
const log = console.log

const express = require('express')
const port = process.env.PORT || 3000
const bodyParser = require('body-parser') // middleware for parsing HTTP body
const { ObjectID } = require('mongodb')

const { mongoose } = require('./db/mongoose')

// import the models
const { Student } = require('./models/student')
const { User } = require('./models/user')


const app = express();
// body-parser middleware
app.use(bodyParser.json())

// Set up a POST route to *create* a student
app.post('/students', (req, res) => {
	// log(req.body)

	// Create a new student
	const student = new Student({
		name: req.body.name,
		year: req.body.year
	})

	// Save student to the database
	student.save().then((result) => {
		res.send(result)
	}, (error) => {
		res.status(400).send(error) // 400 for bad request
	})

})

app.get('/students/:id', (req, res) => {
	// log(req.params.id)
	const id = req.params.id

	// Good practise: validate id
	if (!ObjectID.isValid(id)) {
		res.status(404).send()
	}

	// Otherwise, findById
	Student.findById(id).then((student) => {
		if (!student) {
			res.status(404).send()
		} else {
			/// sometimes wrap returned object in another object
			//res.send({student})   
			res.send(student)
		}
	}).catch((error) => {
		res.status(500).send()
	})

})

app.delete('/students/:id', (req, res) => {
	const id = req.params.id

	if (!ObjectID.isValid(id)) {
		res.status(404).send()
	}

	Student.findByIdAndRemove(id).then((student) => {
		if (!student) {
			res.status(404).send()
		} else {   
			res.send(student)
		}
	}).catch((error) => {
		res.status(500).send()
	})

})

// PATCH for changing properties of elements
// PUT is more for replacing entire elements
app.patch('/students/:id', (req, res) => {
	const id = req.params.id

	const { name, year } = req.body
	const body = { name, year }

	if (!ObjectID.isValid(id)) {
		res.status(404).send()
	}

	// patch it
	Student.findByIdAndUpdate(id, {$set: body}, {new: true}).then((student) => {
		if (!student) {
			res.status(404).send()
		} else {   
			res.send(student)
		}
	}).catch((error) => {
		res.status(400).send()
	})

})


/// User routes
// Set up a POST route to *create* a student
app.post('/users', (req, res) => {
	// log(req.body)

	// Create a new user
	const user = new User({
		name: req.body.email,
		year: req.body.password
	})

	user.save().then((user) => {
		res.send(user)
	}, (error) => {
		res.status(400).send(error) // 400 for bad request
	})

})



app.listen(port, () => {
	log(`Listening on port ${port}...`)
}) 







