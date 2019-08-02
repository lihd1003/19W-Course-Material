
$.ajax({
  type: 'GET',
  url: './table_of_content.json',
  dataType: 'json',
  success: function(data) {
    console.log(data)
  },
  async: false
});
