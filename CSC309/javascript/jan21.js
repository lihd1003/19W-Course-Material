console.log('jan 21');

var a = 3;
console.log(a);

// this causes an error
// console.log(b);

// this will print undefined
console.log(c);
var c = 12;

// this is fine
console.log(c);

// function
function f1() {
	var a = 4;
	console.log(a);
}

f1();
// difference between stack and global
console.log(a);

function f2(){
	console.log(a)
}

// this will print the value of global a normally
f2();

function f3() {
	var r = 4;
	console.log(r);
}

// this is cause error
// f3();
// console.log('outside f3', r)

function f4() {
	k = 'k';
}

// this is cause error
f4();
console.log('outside f4', k);

function f5() {
	console.log(k)
}

f5();

// for loop with var
function forFunc() {
	console.log(i)
	for (var i = 0; i < 5; i++) {
		console.log(i)
	}
	console.log(i)
}
forFunc()
// this will cause error, since the var's function scope
// console.log(i)

function forFuncWithLet() {
	for (let j = 0; j < 5; j++) {
		console.log(j);
	}
	// this will cause erro
	// console.log(j);
}
forFuncWithLet()