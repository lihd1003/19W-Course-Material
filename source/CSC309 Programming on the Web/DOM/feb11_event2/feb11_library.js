"use strict";
// const log = console.log
log('Feb 11 - library - 10am')

// Let's make a small Jquery clone
// one way is to make a function that returns an object
function $$$(selector) {

	const _self = {}
	_self.selector = selector
	_self.element = document.querySelector(selector)

	_self.text = function() {
		return _self.element.innerText
	}

	_self.addClass = function(className) {
		if (!_self.element.classList.contains(className)) {
			_self.element.classList.add(className)
		}
	}

	_self.attr = function(name, value) {
		if (!value) {
			return _self.element.getAttribute(name)
		} else {
			 _self.element.setAttribute(name, value)
		}
	}

	return _self

}
//////////
// Different way of creating library: creating an Object and adding
// to its prototype

// A Circle Generator Library

function CircleGenerator() {
	this.circles = []
	// this..
	// this..
}

CircleGenerator.prototype = {
	// make a circle
	makeCircle: function() {
		const circle = document.createElement('div')
		circle.style = 'width: 60px; height: 60px; border-radius: 50%; margin: 10px; background-color: Aqua;'
		const body = document.querySelector('body')
		body.append(circle)
		this.circles.push(circle)
	},

	changeCirclesColor: function() {
		for (let i = 0; i < this.circles.length; i++) {
			this.circles[i].style.backgroundColor = 'darkmagenta'
		}
	}
}

const cg = new CircleGenerator()











