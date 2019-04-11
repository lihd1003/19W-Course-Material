const mongoose = require('mongoose');

const ReservationSchema = new mongoose.Schema({
    time: String,
    people: Number
});

// Reservations will be embedded in the Restaurant model
const RestaurantSchema = new mongoose.Schema({
    name: String,
    description: String,
    reservations: [ReservationSchema]
});

const Restaurant = mongoose.model('Restaurant', RestaurantSchema);

module.exports = { Restaurant };
