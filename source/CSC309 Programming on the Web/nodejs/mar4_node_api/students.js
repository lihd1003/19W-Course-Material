'use strict';
const log = console.log
log('students.js - march 4 - 6pm')

const fs = require('fs')

const addStudent = function (name, year, courses) {
	
	const students = getAllStudents()

	const newId = (students.length === 0) ? 1 : students[students.length - 1].id + 1

	const student = {
		id: newId,
		name,
		year,
		courses
	}

	students.push(student)
	saveStudentsToJSONFile(students)
}

const saveStudentsToJSONFile = (students) => {
	fs.writeFileSync('students.json', JSON.stringify(students))
}

const getAllStudents = () => {
	try {
		const studentsFromFile = fs.readFileSync('students.json')
		return JSON.parse(studentsFromFile)
	} catch (e) {
		return []
	}
}

const getStudent = (id) => {
	log('get student by id')
	const students = getAllStudents()
	const studentWithId = students.filter((student) => student.id === id)
	return studentWithId[0]
}

const removeStudent = (id) => {
	log('this will remove a student')
	const students = getAllStudents()
	const studentsToKeep = students.filter((student) => student.id !== id)
	saveStudentsToJSONFile(studentsToKeep)

	return students.length !== studentsToKeep.length

}

const addCourse = (id, course) => {
	const students = getAllStudents()
	const studentWithId = students.filter((student) => student.id === id)
	studentWithId[0].courses.push(course)
	// we can edit the objects in the array directly
	saveStudentsToJSONFile(students)
}

module.exports = {
	addStudent,
	getAllStudents,
	getStudent,
	removeStudent,
	addCourse
}	



