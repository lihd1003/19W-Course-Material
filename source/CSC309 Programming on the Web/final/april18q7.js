const db = require('./mydb')
console.log(db)

db.insertOne({id: 'csc209', what: "some other", who: "Reid"});
console.log(db)

db.updateOne('CSC309', {who: 'Reid'});
console.log(db)

db.deleteOne('CSC309');
console.log(db)

db.find('CSC309');
console.log(db)
