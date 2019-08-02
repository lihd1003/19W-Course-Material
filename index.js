let path;
$.ajax({
  type: 'GET',
  url: './table_of_content.json',
  dataType: 'json',
  success: function(data) {
    path = data
  },
  async: false
});

console.log(data)
