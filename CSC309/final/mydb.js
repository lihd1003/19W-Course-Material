

module.exports = (function() {
  let database = {
    'CSC309': {
      id: 'CSC309',
      what: 'Prog',
      who: 'Gonz'
    }
  }

  const insertOne = (obj) => {
    database[obj.id] = obj;
  }

  const updateOne = (id, obj) => {
    Object.assign(database[id], obj);
  }

  const deleteOne = (id) => {
    delete database[id];
  }

  const find = (id) => {
    return database[id];
  }


  return {
    insertOne: insertOne,
    updateOne: updateOne,
    deleteOne: deleteOne,
    find: find,
    database: database
  }
})();
