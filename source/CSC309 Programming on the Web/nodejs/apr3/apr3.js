module.exports = {
	square: (n) => n * n,

	asyncSquare: (n, callback) => {
		setTimeout(() => {
			callback(n * n)
		}, 1000)
	},
	
	asyncSquarePromise: (n) => {
		return new Promise((resolve, reject) => {
				setTimeout(() => {
					resolve(n * n)
				}, 1000)
			}
		)}
}