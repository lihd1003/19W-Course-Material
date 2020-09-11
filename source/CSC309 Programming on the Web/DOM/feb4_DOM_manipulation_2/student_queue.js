/* Student queue js - 10am*/

'use strict';
const log = console.log;
console.log('Student queue - feb 4');

// Let's get the elements we need first
const form = document.querySelector('#queueForm')
const queue = document.querySelector('#queue')

form.addEventListener('submit', addStudent)

function addStudent(e) {
	e.preventDefault(); // prevent default form action

	log('Adding a student')

	// First, need to get the student's name and course
	// from the text fields

	const studentName = document.querySelector('#sName').value
	const studentCourse = document.querySelector('#sCourse').value

	// Format the text for the list item
	// String formatting through template literals
	const studentText = `${studentName} - ${studentCourse}`

	// Now we can create the DOM element
	// first a list eement to hold the student text

	const listElement = document.createElement('li')

	// add the class to the list element
	listElement.className = 'student'

	// add the text to list element
	listElement.appendChild(document.createTextNode(studentText))

	// create and add a remove button
	const removeButton = document.createElement('button')
	removeButton.className = 'remove'
	removeButton.appendChild(document.createTextNode('remove'))
	listElement.appendChild(removeButton)

	/// Add the list element to 
	//queue.appendChild(listElement)

	// Put in the right place depending on addAfter
	const students = document.querySelectorAll('.student')
	const position = document.querySelector('#sAddAt').value

	log(students)
	log(position)

	try {
		if (position > students.length) {
			queue.appendChild(listElement) // put at the end
		} else {
			students[position - 1].before(listElement)
		}
	} catch (e) {
		log('Invalid position')
	}

}


queue.addEventListener('click', removeStudent)

function removeStudent(e) {

	if (e.target.classList.contains('remove')) {
		log('remove student')
		const studentToRemove = e.target.parentElement
		queue.removeChild(studentToRemove)
	}
}

const kickButton = document.querySelector('#kickButton')
kickButton.addEventListener('click', startTimeLimit)

function startTimeLimit(e) {
	const timeLimit = parseInt(document.querySelector('#timeLimit').value)

	const kickInterval = setInterval(function () {
		log('kicked')
		try {
			queue.removeChild(queue.lastElementChild)
		} catch (e) {
			clearInterval(kickInterval)
			log('done kicking')
		}
		
	}, timeLimit * 1000)

}














