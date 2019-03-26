/* CSC309 - user and resource authentication */

'use strict';
const log = console.log;

const express = require('express')
const port = process.env.PORT || 3000
const bodyParser = require('body-parser') // middleware for parsing HTTP body from client
const session = require('express-session')
const hbs = require('hbs')

const { ObjectID } = require('mongodb')

// Import our mongoose connection
const { mongoose } = require('./db/mongoose');

// Import the models
const { Student } = require('./models/student')
const { User } = require('./models/user')


// express
const app = express();
// body-parser middleware setup.  Will parse the JSON and convert to object
app.use(bodyParser.json());
// parse incoming parameters to req.body
app.use(bodyParser.urlencoded({ extended:true }))

// set the view library
app.set('view engine', 'hbs')

// static js directory
app.use("/js", express.static(__dirname + '/public/js'))

// Add express sesssion middleware
app.use(session({
	secret: 'oursecret',
	resave: false,
	saveUninitialized: false,
	cookie: {
		expires: 600000,
		httpOnly: true
	}
}))

// Add middleware to check for logged-in users
const sessionChecker = (req, res, next) => {
	if (req.session.user) {
		res.redirect('dashboard')
	} else {
		next();
	}
}

// route for root; redirect to login
app.get('/', sessionChecker, (req, res) => {
	res.redirect('login')
})

// route for login
app.route('/login')
	.get(sessionChecker, (req, res) => {
		res.sendFile(__dirname + '/public/login.html')
	})

app.get('/dashboard', (req, res) => {
	// check if we have active session cookie
	if (req.session.user) {
		//res.sendFile(__dirname + '/public/dashboard.html')
		res.render('dashboard.hbs', {
			email: req.session.email
		})
	} else {
		res.redirect('/login')
	}
})

// User login and logout routes

app.post('/users/login', (req, res) => {
	const email = req.body.email
	const password = req.body.password

	User.findByEmailPassword(email, password).then((user) => {
		if(!user) {
			res.redirect('/login')
		} else {
			// Add the user to the session cookie that we will
			// send to the client
			req.session.user = user._id;
			req.session.email = user.email
			res.redirect('/dashboard')
		}
	}).catch((error) => {
		res.status(400).redirect('/login')
	})
})

app.get('/users/logout', (req, res) => {
	req.session.destroy((error) => {
		if (error) {
			res.status(500).send(error)
		} else {
			res.redirect('/')
		}
	})
})


// Middleware for authentication for resources
const authenticate = (req, res, next) => {
	if (req.session.user) {
		User.findById(req.session.user).then((user) => {
			if (!user) {
				return Promise.reject()
			} else {
				req.user = user
				next()
			}
		}).catch((error) => {
			res.redirect('/login')
		})
	} else {
		res.redirect('/login')
	}
}

/// Student routes go below

// Set up a POST route to create a student
app.post('/students', authenticate, (req, res) => {
	log(req.body)

	// Create a new student
	const student = new Student({
		name: req.body.name,
		year: req.body.year,
		creator: req.user._id // from the authenticate middleware
	})

	// save student to database
	student.save().then((result) => {
		// Save and send object that was saved
		res.send(result)
	}, (error) => {
		res.status(400).send(error) // 400 for bad request
	})

})

// GET all students
app.get('/students', authenticate, (req, res) => {
	Student.find({
		creator: req.user._id // from authenticate middleware
	}).then((students) => {
		res.send({ students }) // put in object in case we want to add other properties
	}, (error) => {
		res.status(500).send(error)
	})
})

// GET student by id
app.get('/students/:id', (req, res) => {
	const id = req.params.id // the id is in the req.params object

	// Good practise is to validate the id
	if (!ObjectID.isValid(id)) {
		return res.status(404).send()
	}

	// Otheriwse, findById
	Student.findById(id).then((student) => {
		if (!student) {
			res.status(404).send()
		} else {
			res.send({ student })
		}
		
	}).catch((error) => {
		res.status(500).send(error)
	})
})

app.delete('/students/:id', (req, res) => {
	const id = req.params.id

	// Good practise is to validate the id
	if (!ObjectID.isValid(id)) {
		return res.status(404).send()
	}

	// Otheriwse, findByIdAndRemove
	Student.findByIdAndRemove(id).then((student) => {
		if (!student) {
			res.status(404).send()
		} else {
			res.send({ student })
		}
	}).catch((error) => {
		res.status(500).send(error)
	})
})

app.patch('/students/:id', (req, res) => {
	const id = req.params.id

	// Get the new name and year from the request body
	const { name, year } = req.body
	const properties = { name, year }

	// Good practise is to validate the id
	if (!ObjectID.isValid(id)) {
		return res.status(404).send()
	}	

	// Update it
	// $new: true gives back the new document
	Student.findByIdAndUpdate(id, {$set: properties}, {new: true}).then((student) => {
		if (!student) {
			res.status(404).send()
		} else {
			res.send({ student })
		}
	}).catch((error) => {
		res.status(400).send(error)
	})

})


/** User routes **/
app.post('/users', (req, res) => {

	// Create a new user
	const user = new User({
		email: req.body.email,
		password: req.body.password
	})

	// save user to database
	user.save().then((result) => {
		res.send(user)
	}, (error) => {
		res.status(400).send(error) // 400 for bad request
	})

})

app.listen(port, () => {
	log(`Listening on port ${port}...`)
});


