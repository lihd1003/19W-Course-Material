/* feb. 6 - 10am*/
'use strict'
const log = console.log;
console.log('feb 6 - 10am')

let right = false;
function moveCircle(e) {
	// const circle = document.querySelector('#circle')
	const circle = e.target;

	if (!right) {
		circle.style.left = '650px';
		right = true	
	} else {
		circle.style.left = '40px';
		right = false	
	}
}

//const circle = document.querySelector('#circle')
//circle.addEventListener('mouseover', moveCircle)


/// Non-blocking code

setTimeout(function () {
	log('2 seconds')
}, 2000)

log('After setTimeout')


setTimeout(function () {
	log('3 seconds')
}, 3000)

log('After setTimeout 2')


