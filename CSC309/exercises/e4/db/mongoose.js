'use strict';

const mongoose = require('mongoose');

mongoose.connect(process.env.MONGODB_URI || 'mongodb://localhost:27017/RestaurantAPI', { useNewUrlParser: true, useCreateIndex: true});

module.exports = {
	mongoose
}