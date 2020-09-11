'use strict';
const log = console.log
log('app.js - march 4 - 6pm')

const students = require('./students')

// students.addStudent('Kate', 3, ['csc120'])
// students.addStudent('Paul', 2, ['csc309'])


//log(students.getAllStudents())
// log(students.getStudent(2))

// log(students.removeStudent(2))
// log(students.getAllStudents())

students.addCourse(1, 'csc301')
log(students.getAllStudents())

