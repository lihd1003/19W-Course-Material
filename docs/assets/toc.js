const toc = $("#toc");
const headers = $("#content h1, #content h2");
headers.each((idx, e) => {
    console.log(idx)
    e = $(e);
    e.attr("id", "header-" + String(idx));
    const navlink = $('<a class="text-capitalize list-group-item list-group-item-action" style="padding:0.75em"></a>');
    if (e.is("h1")) {
        navlink.addClass("font-weight-bold");
        navlink.text(e.text());
    } 
    else {
        navlink.html('<i class="fas fa-chevron-right"></i> ' + e.text());
    }
    navlink.attr("href", "#" + e.attr("id"));
    
    toc.append(navlink);
});