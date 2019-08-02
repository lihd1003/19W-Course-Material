'use strict'
let paths = []

$.ajax({
  dataType: "json",
  url: "./table_of_content.json",
  success: (data) => {
    paths = data
  },
  async: false
});

let current = "./"

const option = {
  item: '<li class="mdl-list__item" >' +
    '<a data-path class="path mdl-list__item-primary-content mdl-color-text--blue-grey-800" style="cursor: pointer" onclick="update(this)">' +
    '<i class="material-icons mdl-list__item-icon type"></i>' +
    `<span class="name"></span>` +
    '</a></li>',
  valueNames: ['type', 'name', {
    name: 'path',
    attr: 'data-path'
  }]
}

const list = new List('list', option, paths)

$("#path").text(current)

list.filter((i) => {
  return i.values().path === "."
})
list.sort('name', { order: "asc" });


const update = (t) => {
  const target = t.children[1].innerHTML
  if (t.firstChild.innerHTML === "folder") {
    current = t.getAttribute("data-path") + "/" + target
    $("#path").text(current)
    list.search()
    $("#saerch").val("")
    list.filter()
    list.filter((i) => {
      return i.values().path === current
    })
    list.sort('name', { order: "asc" });
  } else {
    window.open(current + "/" + target, '_blank');
  }
}

const back = () => {
  if (current === ".") {
    return
  }
  else {
    const backed = current.split("/")
    backed.pop()
    current = backed.join("/")
    $("#path").text(current + "/")
    list.search()
    $("#saerch").val("")
    list.filter()
    list.filter((i) => {
      return i.values().path === current
    })
    list.sort('name', { order: "asc" });
  }

}
