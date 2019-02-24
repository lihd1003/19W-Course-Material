const log = console.log
log('Feb 13 - node - 10am');

// require() is a built-in function that 'imports'
// modules into a variable
const util = require('util')

const s = util.format('%s %s', 'csc', '309')
log(s)

const fs = require('fs')
fs.appendFileSync('feb13.txt', 'appending to file\n')

/// Creting our own module (the module design pattern)
// require() returns module.exports of the module
const course = require('./course')

//course.addCourse('csc343')
//course.removeCourse('csc301')
log(course.courseList)

// a 3rd-party module installed by npm:
const chalk = require('chalk')
log(chalk.blue('Feelin blue'))
log(chalk.blue.bgRed.bold('Hello world'))

/// User input ///
// Passing in command line arguments
//log(process.argv)

//const command = process.argv[2]
//const courseName = process.argv[3]
const yargs = require('yargs')
const yargv = yargs.argv
log(yargs.argv)


//if (command === '--add') {
if ('add' in yargv) {
	course.addCourse(yargv.add)
	log(course.courseList)
}

//if (command === '--remove') {
if ('remove' in yargv) {	
	course.removeCourse(yargv.remove)
	log(course.courseList)
}










