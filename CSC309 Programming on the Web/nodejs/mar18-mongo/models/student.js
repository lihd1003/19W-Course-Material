
const mongoose = require('mongoose')

const Student = mongoose.model('Student', {
	name: {
		type: String,
		required: true,
		minlegth: 1
	},
	year: {
		type: Number,
		required: true,
		default: 1
	}
})

module.exports = { Student }