
console.log('Course selection module')

// module.exports.courseList = ['csc309', 'csc301']

module.exports = {
	courseList: ['csc309', 'csc301'],

	addCourse: function(course) {
		this.courseList.push(course)
	},

	removeCourse: function(course) {
		const i = this.courseList.indexOf(course)
		this.courseList.splice(i, 1)
	}
}