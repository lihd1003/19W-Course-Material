let path = []

$.ajax({
  dataType: "json",
  url: "./table_of_content.json",
  success: (data) => {
    path = JSON.parse(data)
  },
  async: false
});

let current = ["."]

const option = {
  item: '<li class="mdl-list__item" >' +
    '<a data-parent class="parent mdl-list__item-primary-content mdl-color-text--blue-grey-800" style="cursor: pointer" onclick="update(this)">' +
    '<i class="material-icons mdl-list__item-icon type"></i>' +
    `<span class="name"></span>` +
    '</a></li>',
  valueNames: ['type', 'name', {
    name: 'parent',
    attr: 'data-parent'
  }]
}

const list = new List('list', option, path)

$("#path").text(current.join("/") + "/")

list.filter((i) => {
  return i.values().parent === "."
})
list.sort('name', { order: "asc" });


const update = (t) => {
  const target = t.children[1].innerHTML
  if (t.firstChild.innerHTML === "folder") {
    current.push(target)
    $("#path").text(current.join("/") + "/")
    list.filter()
    list.filter((i) => {
      return i.values().parent === target
    })
    list.sort('name', { order: "asc" });
  } else {
    window.open(current.join("/") + "/" + target, '_blank');
  }
}

const back = () => {
  if (current.length == 1) {
    return
  }
  current.pop()
  list.filter()
  list.filter((i) => {
    return i.values().parent === current[current.length - 1]
  })
  list.sort('name', { order: "asc" });
  $("#path").text(current.join("/") + "/")
}
