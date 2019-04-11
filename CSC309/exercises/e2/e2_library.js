/* E2 Library - JS */

/*-----------------------------------------------------------*/
/* Starter code - DO NOT edit the code below. */
/*-----------------------------------------------------------*/

// global counts
let numberOfBooks = 0; // total number of books
let numberOfPatrons = 0; // total number of patrons

// global arrays
const libraryBooks = [] // Array of books owned by the library (whether they are loaned or not)
const patrons = [] // Array of library patrons.

// Book 'class'
class Book {
	constructor(title, author, genre) {
		this.title = title;
		this.author = author;
		this.genre = genre;
		this.patron = null; // will be the patron objet

		// set book ID
		this.bookId = numberOfBooks;
		numberOfBooks++;
	}

	setLoanTime() {
		// Create a setTimeout that waits 3 seconds before indicating a book is overdue

		const self = this; // keep book in scope of anon function (why? the call-site for 'this' in the anon function is the DOM window)
		setTimeout(function() {
			
			console.log('overdue book!', self.title)
			changeToOverdue(self);

		}, 3000)

	}
}

// Patron constructor
const Patron = function(name) {
	this.name = name;
	this.cardNumber = numberOfPatrons;

	numberOfPatrons++;
}


// Adding these books does not change the DOM - we are simply setting up the 
// book and patron arrays as they appear initially in the DOM.
libraryBooks.push(new Book('Harry Potter', 'J.K. Rowling', 'Fantasy'));
libraryBooks.push(new Book('1984', 'G. Orwell', 'Dystopian Fiction'));
libraryBooks.push(new Book('A Brief History of Time', 'S. Hawking', 'Cosmology'));

patrons.push(new Patron('Jim John'))
patrons.push(new Patron('Kelly Jones'))

// Patron 0 loans book 0
libraryBooks[0].patron = patrons[0]
// Set the overdue timeout
libraryBooks[0].setLoanTime()  // check console to see a log after 3 seconds


/* Select all DOM form elements you'll need. */ 
const bookAddForm = document.querySelector('#bookAddForm');
const bookInfoForm = document.querySelector('#bookInfoForm');
const bookLoanForm = document.querySelector('#bookLoanForm');
const patronAddForm = document.querySelector('#patronAddForm');

/* bookTable element */
const bookTable = document.querySelector('#bookTable')
/* bookInfo element */
const bookInfo = document.querySelector('#bookInfo')
/* Full patrons entries element */
const patronEntries = document.querySelector('#patrons')

/* Event listeners for button submit and button click */

bookAddForm.addEventListener('submit', addNewBookToBookList);
bookLoanForm.addEventListener('submit', loanBookToPatron);
patronAddForm.addEventListener('submit', addNewPatron)
bookInfoForm.addEventListener('submit', getBookInfo);

/* Listen for click patron entries - will have to check if it is a return button in returnBookToLibrary */
patronEntries.addEventListener('click', returnBookToLibrary)

/*-----------------------------------------------------------*/
/* End of starter code - do *not* edit the code above. */
/*-----------------------------------------------------------*/


/** ADD your code to the functions below. DO NOT change the function signatures. **/


/*** Functions that don't edit DOM themselves, but can call DOM functions 
     Use the book and patron arrays appropriately in these functions.
 ***/

// Adds a new book to the global book list and calls addBookToLibraryTable()
function addNewBookToBookList(e) {
	e.preventDefault();

	// Add book book to global array
	const title = document.querySelector("#newBookName").value
	const author = document.querySelector("#newBookAuthor").value
	const genre = document.querySelector("#newBookGenre").value
	const book = new Book(title, author, genre)
	libraryBooks.push(book)
	
	// Call addBookToLibraryTable properly to add book to the DOM
	addBookToLibraryTable(book)
}

// Changes book patron information, and calls 
function loanBookToPatron(e) {
	e.preventDefault();

	// Get correct book and patron
	let bookId = document.querySelector("#loanBookId").value
	let patronId = document.querySelector("#loanCardNum").value
	
	if (parseInt(bookId) != NaN) {
		bookId = parseInt(bookId)
	}
	
	if (parseInt(patronId) != NaN) {
		patronId = parseInt(patronId)
	}

	// Add patron to the book's patron property
	if (libraryBooks[bookId].patron == null) {
		libraryBooks[bookId].patron = patrons[patronId]
	} else {
		return
	}

	// Add book to the patron's book table in the DOM by calling addBookToPatronLoans()
	addBookToPatronLoans(libraryBooks[bookId])

	// Start the book loan timer.
	libraryBooks[bookId].setLoanTime()
	
	
	// for my enhancement of lending history
	const newHistory = new History(bookId, patronId)
	newHistory.setBorrowTime()
	allHistory.push(newHistory)
}

// Changes book patron information and calls returnBookToLibraryTable()
function returnBookToLibrary(e){
	e.preventDefault();
	// check if return button was clicked, otherwise do nothing.
	if (e.target.classList.contains('return')) {
		const bookId = parseInt(e.target.parentElement.parentElement.cells[0].innerHTML)
		const bookToReturn = libraryBooks[bookId]
		
		// Call removeBookFromPatronTable()
		removeBookFromPatronTable(bookToReturn)

		// for my enhancement of lending history
		updateReturnHistory(bookToReturn.bookId, bookToReturn.patron.cardNumber)
		
		// Change the book object to have a patron of 'null'
		bookToReturn.patron = null
		
	}
}

// Creates and adds a new patron
function addNewPatron(e) {
	e.preventDefault();

	// Add a new patron to global array
	const name = document.querySelector("#newPatronName").value
	const patron = new Patron(name)
	patrons.push(patron)
	
	// Call addNewPatronEntry() to add patron to the DOM
	addNewPatronEntry(patron)
}

// Gets book info and then displays
function getBookInfo(e) {
	e.preventDefault();

	// Get correct book
	const book = libraryBooks[parseInt(document.querySelector("#bookInfoId").value)]
	// Call displayBookInfo()	
	displayBookInfo(book)
}


/*-----------------------------------------------------------*/
/*** DOM functions below - use these to create and edit DOM objects ***/

// Adds a book to the library table.
function addBookToLibraryTable(book) {
	// Add code here
	const row = bookTable.insertRow(-1)
	const bookIDCell = row.insertCell(0)
	const titleCell = row.insertCell(1)
	const patronCell = row.insertCell(2)
	
	bookIDCell.innerHTML = book.bookId
	
	const strong = document.createElement('strong')
	strong.innerHTML = book.title
	titleCell.appendChild(strong)
}


// Displays deatiled info on the book in the Book Info Section
function displayBookInfo(book) {
	infomation = [book.bookID, book.title, book.author, book.genre]
	// Add code here
	for (let i = 0; i < 4; i++) {
		bookInfo.children[i].children[0].innerHTML = infomation[i]
	}
	if (book.patron == null) {
		bookInfo.children[4].children[0].innerHTML = "N/A"
	} else {
		bookInfo.children[4].children[0].innerHTML = book.patron.name
	}
	
}

// Adds a book to a patron's book list with a status of 'Within due date'. 
// (don't forget to add a 'return' button).
function addBookToPatronLoans(book) {
	// Add code here
	bookTable.rows[book.bookId + 1].cells[2].innerHTML = book.patron.cardNumber
	const patronTable = document.querySelectorAll(".patronLoansTable")[book.patron.cardNumber]
	const row = patronTable.insertRow(-1)
	const bookIDCell = row.insertCell(0)
	const titleCell = row.insertCell(1)
	const statusCell = row.insertCell(2)
	const returnCell = row.insertCell(3)
	
	bookIDCell.innerHTML = book.bookId
	
	const strong = document.createElement('strong')
	strong.innerHTML = book.title
	titleCell.appendChild(strong)
	
	const span = document.createElement('span')
	span.className = "green"
	span.innerHTML = "Within due date"
	statusCell.appendChild(span)
	
	const returnButton = document.createElement('button')
	returnButton.className = "return"
	returnButton.innerHTML = "return"
	returnCell.appendChild(returnButton)
	
}

// Adds a new patron with no books in their table to the DOM, including name, card number,
// and blank book list (with only the <th> headers: BookID, Title, Status).
function addNewPatronEntry(patron) {
	// Add code here
	const patronDiv = document.createElement('div')
	patronDiv.className = "patron"
	
	const p1 = document.createElement('p')
	const span1 = document.createElement('span')
	span1.className = "bold"
	span1.innerHTML = patron.name
	p1.innerHTML = "Name: "
	p1.appendChild(span1)
	patronDiv.appendChild(p1)
	
	const p2 = document.createElement('p')
	const span2 = document.createElement('span')
	span2.className = "bold"
	span2.innerHTML = patron.cardNumber
	p2.innerHTML = "Card Number: "
	p2.appendChild(span2)
	patronDiv.appendChild(p2)
	
	const h4 = document.createElement('h4')
	h4.innerHTML = "Books on loan:"
	patronDiv.appendChild(h4)
	
	const table = document.createElement('table')
	table.className = "patronLoansTable"
	const row = table.insertRow(0)
	const values = ["BookID", "Title", "Status", "Return"]
	for (let i = 0; i < 4; i++) {
		const th = document.createElement('th')
		th.innerHTML = values[i]
		row.appendChild(th)
	}
	patronDiv.appendChild(table)
	
	patronEntries.appendChild(patronDiv)
}


// Removes book from patron's book table and remove patron card number from library book table
function removeBookFromPatronTable(book) {
	// Add code here
	const patronTable = document.querySelectorAll(".patronLoansTable")[book.patron.cardNumber]
	for (let i = 0; i < patronTable.rows.length; i++) {
		if (patronTable.rows[i].cells[0].innerHTML == book.bookId) {
			patronTable.deleteRow(i)
			break
		}
	}
	bookTable.rows[book.bookId + 1].cells[2].innerHTML = ""
}

// Set status to red 'Overdue' in the book's patron's book table.
function changeToOverdue(book) {
	// Add code here
	const patronTable = document.querySelectorAll(".patronLoansTable")[book.patron.cardNumber]
	for (let i = 0; i < patronTable.rows.length; i++) {
		let row = patronTable.rows[i]
		if (row.cells[0].innerHTML == book.bookId) {
			row.cells[2].children[0].innerHTML = "Overdue"
			row.cells[2].children[0].className = "red"
			break
		}
	}
}



/** =============== enhancement1 Review =============**/

// add a new class Review
const Review = function(book, user, review) {
	this.book = book
	this.user = user
	this.review = review	
}

// add the init one 
const reviews = []
reviews.push(new Review("Harry Potter", 
"This is a great book and I like it", 
"Jim John"))
const reviewForm = document.querySelector("#reviewForm")
reviewForm.addEventListener('submit', addNewReview)

// add to list
function addNewReview(e) {
	e.preventDefault()
	const book = document.querySelector("#reviewBookTitle").value
	const user = document.querySelector("#reviewUser").value
	const review = document.querySelector("#reviewText").value
	const newReview = new Review(book, user, review)
	reviews.push(newReview)
	
	updateReview(newReview)
}

// update HTML DOM
function updateReview(review) {
	const reviewsDiv = document.querySelector("#reviewsDiv")
	const div = document.createElement("div")
	div.className = "review"
	
	const title = document.createElement("p")
	title.innerHTML = "Book title: "
	const strong = document.createElement("strong")
	strong.innerHTML = review.book
	title.appendChild(strong)
	div.appendChild(title)
	
	const reviewText = document.createElement("p")
	reviewText.className = "reviewP"
	reviewText.innerHTML = review.review
	div.appendChild(reviewText)
	
	const people = document.createElement("p")
	people.innerHTML = "by: " + review.user
	div.appendChild(people)
	reviewsDiv.appendChild(div)
}

// implement choosing reviews
let selectedReview = []
const reviewsDiv = document.querySelector("#reviewsDiv")
reviewsDiv.addEventListener('click', checkBox)

function checkBox(e) {
	if (e.target.classList.contains('review')) {
		const div = e.target
		const index = Array.from(div.parentElement.children).indexOf(div)
		if (! selectedReview.includes(index)) {
			div.style.background = "red"
			selectedReview.push(index)
		} else {
			div.style.background = ""
			for (let i = 0; i < selectedReview.length; i++) {
				if (selectedReview[i] == index) {
					selectedReview.splice(i, 1)
					break
				}
			}
		}
	}
}

// implement delete chosen reviews for the list and DOM
document.querySelector("#deleteReview").addEventListener('click', function () {
	let length = selectedReview.length
	selectedReview = selectedReview.sort()
	const reviewsDiv = document.querySelector("#reviewsDiv")
	for (let i = length-1; i >= 0; i--){
		reviews.splice(selectedReview[i], 1)
		reviewsDiv.removeChild(reviewsDiv.children[selectedReview[i]])
		selectedReview.pop()
	}
})

/** ============================ enhancement2 search =======================**/

// bind onclick actions
document.querySelector("#searchTitleB").addEventListener('click', search)
document.querySelector("#searchGenreB").addEventListener('click', searchGenre)
document.querySelector("#clear").addEventListener('click', function (){
	let searchBooks = []
	for (let i = 0; i < libraryBooks.length; i++) {
		searchBooks.push(i)
	}
	displaySearch(searchBooks, "All Books")
})

// search
function search(){
	const name = document.querySelector("#searchTitle").value
	let searchBooks = []
	for (let i = 0; i < libraryBooks.length; i++) {
		if (libraryBooks[i].title.toLowerCase().includes(name.toLowerCase()) || String(i) == name) {
			searchBooks.push(i)
		}
	}
	
	displaySearch(searchBooks, name)
}

function searchGenre() {
	const g = document.querySelector("#searchGenre").value
	let searchBooks = []
	for (let i = 0; i < libraryBooks.length; i++) {
		if (libraryBooks[i].genre.toLowerCase().includes(g.toLowerCase())) {
			searchBooks.push(i)
		}
	}
	displaySearch(searchBooks, "genre: "+g)
}

// display search results
function displaySearch(books, name) {
	document.querySelector("#searchText").innerHTML = name
	let l = bookTable.rows.length
	for (let i = 1; i < l; i++) {
		bookTable.deleteRow(1)
	}
	for (let i = 0; i < books.length; i++) {
		const row = bookTable.insertRow(-1)
		const bookIDCell = row.insertCell(0)
		const titleCell = row.insertCell(1)
		const patronCell = row.insertCell(2)
	
		bookIDCell.innerHTML = libraryBooks[books[i]].bookId
	
		const strong = document.createElement('strong')
		strong.innerHTML = libraryBooks[books[i]].title
		titleCell.appendChild(strong)
		if (libraryBooks[books[i]].patron != null) {
			patronCell.innerHTML = libraryBooks[books[i]].patron.cardNumber
		}
	}
}

/** ========================== enhancement3 history =======================**/

// history 'class'
class History {
	constructor(id, patronID) {
		this.bookID = id;
		this.patron = patronID;
		this.borrowtime = null;
		this.returntime = null;
	}
	
	setReturnTime() {
		this.returntime = new Date();
	}
	
	setBorrowTime() {
		this.borrowtime = new Date();
	}
}

// add init history 
allHistory = []
const initHis = new History(0,0)
initHis.setBorrowTime()
allHistory.push(initHis)

function updateReturnHistory(id, patronID) {
	for (let i = 0; i < allHistory.length; i++) {
		if (allHistory[i].bookID == id && allHistory[i].patron == patronID && allHistory.returntime == null) {
			allHistory[i].setReturnTime()
			break
		}
	}
}

// search 
document.querySelector("#historySubmit").addEventListener("click", getHistory)
function getHistory() {
	const id = document.querySelector("#historyTitle").value
	const start = document.querySelector("#historyStart").value
	const end = document.querySelector("#historyEnd").value
	const selectedHistory = allHistory.slice(0)
	let startTime = 0
	if (start != "") {
		startTime = (new Date(start)).getTime()
	} 
	let endTime = (new Date()).getTime()
	if (end != "") {
		endTime = (new Date(end)).getTime()
	}
	let index = 0
	while (index < selectedHistory.length) {
		if ((selectedHistory[index].bookID != id && id != "") || 
		selectedHistory[index].borrowtime.getTime() < startTime ||
		selectedHistory[index].borrowtime.getTime() > endTime) {
			selectedHistory.splice(index, 1)
		} else {
			index++
		}
	}
	
	displayHistory(selectedHistory)
}

// display search results
function displayHistory(selec) {
	const table = document.querySelector("#historyTable")
	let l = table.rows.length
	for (let i = 1; i < l; i++) {
		table.deleteRow(1)
	}
	for (let i = 0; i < selec.length; i++){
		const row = table.insertRow(-1)
		const id = row.insertCell(0)
		const title = row.insertCell(1)
		const patronID = row.insertCell(2)
		const borrowtime = row.insertCell(3)
		const returntime = row.insertCell(4)
		id.innerHTML = selec[i].bookID
		title.innerHTML = libraryBooks[selec[i].bookID].title
		patronID.innerHTML = selec[i].patron
		borrowtime.innerHTML = selec[i].borrowtime.toLocaleString("en")
		if (selec[i].returntime != null){
			returntime.innerHTML = selec[i].returntime.toLocaleString("en")
			returntime.className = "green"
		} else {
			returntime.innerHTML = "Not yet returned"
			returntime.className = "red bold"
		}
	}
}