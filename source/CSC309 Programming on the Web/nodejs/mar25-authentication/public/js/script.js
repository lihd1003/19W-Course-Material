/* AJAX fetch() calls */
const log = console.log

log('Our script')
function addStudent() {
    const url = '/students';
    // The data we are going to send in our request
    let data = {
        name: document.querySelector('#name').value,
        year: document.querySelector('#year').value
    }
    // Create our request constructor with all the parameters we need
    const request = new Request(url, {
        method: 'post', 
        body: JSON.stringify(data),
        headers: {
            'Accept': 'application/json, text/plain, */*',
            'Content-Type': 'application/json'
        },
    });
    fetch(request)
    .then(function(res) {
        // Handle response we get from the API
        // Usually check the error codes to see what happened
        const message = document.querySelector('#message')
        if (res.status === 200) {
            console.log('Added student')
            message.innerText = 'Success: Added a student.'
            message.setAttribute("style", "color: green")
           
        } else {
            message.innerText = 'Could not add student'
            message.setAttribute("style", "color: red")
     
        }
        console.log(res)
        
    }).catch((error) => {
        console.log(error)
    })
}

function getStudents() {
    const url = '/students';

    fetch(url)
    .then((res) => { 
        if (res.status === 200) {
           return res.json() 
       } else {
            alert('Could not get students')
       }                
    })
    .then((json) => {
        studentsList = document.querySelector('#studentsList')
        console.log(json)
        json.students.map((s) => {
            li = document.createElement('li')
            li.innerHTML = `Name: <strong>${s.name}</strong>, Year: <strong>${s.year}</strong>`
            studentsList.appendChild(li)
            console.log(s)
        })
    }).catch((error) => {
        console.log(error)
    })
}