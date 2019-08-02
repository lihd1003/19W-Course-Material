/* Feb. 11 - 10am */
'use strict';
const log = console.log
log('feb 11 - 10am ')

function logId(element) {
	log(element.id)
}

// The DOM isn't loaded yet, so can't find the element

// const myElement = document.querySelector('#myId')
// logId(myElement)

document.addEventListener('DOMContentLoaded', function() {
	const myElement = document.querySelector('#myId')
	logId(myElement)

	// Careful with scope
	// Won't be able to see logId2 outside of this scope
	const logId2 = function(element) {
		log(element.id)
	}

	// To fix this, you can attach the object/function
	// to the global window object
	window.logId3 = function(element) {
		log(element.id)
	}

})









