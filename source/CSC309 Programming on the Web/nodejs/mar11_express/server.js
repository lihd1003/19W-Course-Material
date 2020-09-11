/* server.js - mar 11 - 6pm*/
'use strict';
const log = console.log

const express = require('express')

const app = express();

// Setting up a static directory using express middleware
app.use(express.static(__dirname + '/pub'))



// Let's make some express 'routes'
// Express has something called a Router, which takes
// specific HTTP requests and handles them
// based on the HTTP method and the URL

// Let's make a route for an HTTP GET request to the
// 'root' of our app (i.e. the top level doman '/')
app.get('/', (req, res) => {
	// Sending a string 
	//res.send('This should be the root route!')

	// sending html
	res.send('<h1>This should be the root route!</h1>')	
})

app.get('/contact', (req, res) => {
	res.send('<h2>This is the Contact page</h2>')	
})

// error codes
app.get('/problem', (req, res) => {
	// You can indicate a status code to send back
	// by default it is 200
	// it's up to you if you want to send anything else
	res.status(500).send("There was a problem on the server")

	// can send status codes with no standard
	//res.status(867).send("There was a problem on the server")	
})

// some JSON
app.get('/someJSON', (req, res) => {
	res.send({
		name: 'John',
		year: 3,
		courses: ['csc309','csc301']
	})
})

const port = process.env.PORT || 3000

app.listen(port, () => {
	log(`Listening on port ${port}...`)
}) // common localhost development port 3000
// we've bound that port on localhost to go to our 
// express server
// Must restart web server when you make changes to
// your route handlers


