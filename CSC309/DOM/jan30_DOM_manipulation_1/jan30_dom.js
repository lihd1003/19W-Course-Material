/* jan 30 - dom - 10am */
'use strict';
const log = console.log
log('jan 30 dom - 10am')

// We can select the elements using their id's and 
// bring them into javasript
const headerText = document.getElementById('headerText')
log(headerText)
log(headerText.innerText)

headerText.innerText = 'Geddit'
headerText.innerHTML = '<h2>I am an h2</h2>'
log(headerText.innerHTML)

// Can change the css style
headerText.style.color = 'orangered'
headerText.style.marginLeft = '300px'

const sidebar = document.getElementById('sidebar')
log(sidebar)

// get all the posts
const postCollection = document.getElementsByClassName('post')
log(postCollection)

postCollection[1].style.backgroundColor = 'seagreen'

// How do we change all of them to seagreen?
for (let i = 0; i < postCollection.length; i++) {
	postCollection[i].style.backgroundColor = 'seagreen'
}

const listElements = document.getElementsByTagName('li');
log(listElements)

// Another way to select (thanks ES6) - querySelector

const postsArea1 = document.querySelector('#posts')
log(postsArea1)
const postsArea2 = document.getElementById('posts')
log(postsArea2)

/// triple equal sign - checks if references are equal
// always use this one
log(postsArea1 === postsArea2)

// querySelectorAll for multiple elements
const allPosts = document.querySelectorAll('.post')
log(allPosts)

////////////
// DOM hierarchy
const postsArea = document.querySelector('#posts')

log(postsArea.parentElement)
log(postsArea.parentElement.parentElement)

log(postsArea.children)
log(postsArea.firstElementChild)
log(postsArea.lastElementChild)
log(postsArea.nextElementSibling)
log(postsArea.previousElementSibling)

/// Creating elements

// Let's create a new post
const newPost = document.createElement('div')

// add the post class
newPost.className = 'post'
log(newPost)

/// Make the post's header
// first, the text node
const newPostTitle = document.createTextNode('Javascriptttttt')
const newPostTitleContainer = document.createElement('h3')
newPostTitleContainer.appendChild(newPostTitle)

log(newPostTitleContainer)

// text for the post content
const newPostContent = document.createTextNode("I can't believe you can modify the DOM!")
newPost.appendChild(newPostTitleContainer)
newPost.appendChild(newPostContent)

log(newPost)

postsArea.appendChild(newPost)
















