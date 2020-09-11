'use strict';
const log = console.log

log('Feb 25 - 6pm')

const student = {
	name: 'Bob',
	year: 3,
	courses: ['csc301', 'csc309', 'csc343', 'phy207']
}

const studentString = JSON.stringify(student)
log(studentString)

const fs = require('fs');
// writing a json string to a file
fs.writeFileSync('student.json', studentString)
/////
// read from a json string
const studentJSONString = fs.readFileSync('student.json')

const studentParsed = JSON.parse(studentJSONString)

log(studentParsed)

///////////
const students = [
	student,
	{name: 'Kelly', year: 2,
	courses: ['csc207', 'csc209', 'csc301', 'csc309'] }
]

const studentsString = JSON.stringify(students)
log(studentsString)
fs.writeFileSync('students.json', studentsString)
/////
// read from a json string
const studentsJSONString = fs.readFileSync('students.json')

const studentsParsed = JSON.parse(studentsJSONString)

log(studentsParsed)
log(studentsParsed[1].name)


studentsParsed.map((student) => log(student.name))

// How do we find the total enrollment for every course?
const courseEnrollment = studentsParsed.reduce((enrollment, student) => {
	student.courses.map((course) => {
		if (course in enrollment) {
			enrollment[course] = enrollment[course] + 1
		} else {
			enrollment[course] = 1
		}
	})
	return enrollment
}, {})

log(courseEnrollment)





















