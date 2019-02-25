/**============== Q2 ================ **/

/**
let a = 4;
let b = 1;
**/

/**
(function() {
	if (a > 2) {
		let b = 3;
		a = 0;
	}
	console.log(a + b)
})();

// 1
**/

/**
const c = 0;
for (let i = 0; i < a; i++) {
	if (i > 2) {
		b = 0;
	}
	c += b;
	console.log(c);
}
// Error Assignment to constant variable
**/

/**
function foo() {
	function bar() {
		return function(){
			console.log(a);
		}
	}
	a = 3;
	return bar();
}

const baz = foo();
baz();
**/

/**
function f1() {
	const n = b;
	return function(p) {
		return n + p;
	}
}
const f2 = f1();
console.log(f2(a));
**/


/**============== Q3 ================ **/


/**
function Account() {
	this.balance = 0;
}
Account.prototype = {
	buyWithCard: function(n) {
		if (this.balance >= n) {
			this.balance -= n;
			console.log("all good");
		} else {
			console.log("not enough money")
		}
	},
	
	showBalance: function() {
		console.log(this.balance)
	},
	
	addToBalance: function(n) {
		this.balance += n
	}
}

const myAcc = new Account();
myAcc.showBalance()
myAcc.addToBalance(30)
myAcc.showBalance()
myAcc.buyWithCard(20)
myAcc.buyWithCard(50)
const yourAcc = new Account();
myAcc.buyWithCard.bind(yourAcc)(5);
**/


/**============== Q4 ================ **/


/**
function f0() {
	console.log(0);
}

function f5(){
	console.log(5);
}

setTimeout(f5, 5000);
setTimeout(f0, 0);
console.log("bar");
// setTimeout will let the function go to the callback queue
// wait for web apis to notify that is finished 
// then go back to call stack
**/


/**============== Q5 ================ **/

const tform = document.querySelector("#tweetForm");
const timeline = document.querySelector("#timeline");
tform.addEventListener("click", addTweet);

function addTweet(e) {
	if (e.target.classList.contains("tweetButton")) {
		const textInput = document.querySelector("#tweetInput")
		if (textInput.value.length != 5) {
			const tweet = makeTweet(textInput.value);
			timeline.appendChild(tweet);
		} else {
			const error = makeErrorMessage();
			tform.appendChild(error);
		}
	}
}

function makeTweet(tweetText) {
	const tweet = document.createElement("div")
	tweet.className = "tweet"
	const img = document.createElement("img")
	img.setAttribute("src", "https://cdn0.iconfinder.com/data/icons/tiny-icons-1/100/tiny-15-64.png")
	tweet.appendChild(img)
	
	const textNode = document.createTextNode(tweetText)
	tweet.appendChild(textNode)
	return tweet
}

function makeErrorMessage() {
	const error = document.createElement("div")
	error.id = "error_message"
	const span = document.createElement("span")
	const textNode = document.createTextNode("Max")
	span.appendChild(textNode)
	error.appendChild(span)
	return error
}