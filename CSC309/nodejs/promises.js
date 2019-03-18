/* Promises */

'use strict';
const log = console.log

const myPromise = new Promise((resolve, reject) => {
	
	// until a promise is resolved or rejected, it is pending
	// once it is resolved or rejected, it is fulfilled
	// resolve('12345')
	// reject('1234')

	setTimeout(() => {
		resolve({message: 'Promise resolved'})
	}, 1000)
})


// myPromise.then((message) => {
// 	log('Resolve function', message)
// }, (error) => {
// 	log('Reject function', error)
// })

const promiseSquare = (n) => {
	return new Promise((resolve, reject) => {
		setTimeout(() => {
			if (typeof(n) === 'number') {
				resolve(n * n)
			} else {
				reject('n must be a number')
			}
		}, 500)
	})
}

/*
promiseSquare('4').then((result) => { // will give us undefined because it thinks we've settled everything
	log('Squared it', result)
	return promiseSquare(result)
}, (error) => {
	log(error)
}).then((result) => {
	log('Squared it again', result)
	return promiseSquare(result)
}, (error) => {
	log(error)
})
*/

promiseSquare(5).then((result) => {
	log('Squared it', result)
	return promiseSquare(result)
}).then((result) => {
	log('Squared it again', result)
	return promiseSquare(result)
}).catch((error) => {
	log(error)
})







