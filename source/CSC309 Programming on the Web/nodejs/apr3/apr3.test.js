
apr3 = require('./apr3')
expect = require('expect')

describe('Square', () => {
	it('should square a number', () => {
		const result = apr3.square(4)
		expect(result).toBe(16)

		expect(result).not.toBe(163)
		expect(typeof result).toBe('number');

		expect({name: "Jimmy"}).toEqual({name: "Jimmy"})
		expect({name: "Jimmy"}).toHaveProperty('name')

		expect([1,2,3]).toContain(2)
		expect([1,2,3]).not.toContain(22)

	});

	it('should async square a number', (done) => {
		apr3.asyncSquare(4, (result) => {
			expect(result).toBe(16)
			done(); // has to wait for done 
		})

	});

	it('should async square a number w/ a promise', (done) => {

		apr3.asyncSquarePromise(4).then((result) => {
			expect(result).toBe(16)
			done(); // has to wait for done 
		})

	});
})

