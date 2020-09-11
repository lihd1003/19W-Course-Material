/* User model */
'use strict';

const mongoose = require('mongoose')
const validator = require('validator')

// making a model a little differently
const UserSchema = new mongoose.Schema({
	email: {
		type: String,
		required: true,
		minlength: 1,
		trim: true,
		unique: true,
		validate: {
			validator: validator.isEmail,
			message: 'Not valid email'
		}
	}, 
	password: {
		type: String,
		required: true,
		minlength: 8
	}
})

const User = mongoose.model('User', UserSchema)

module.exports = { User }








