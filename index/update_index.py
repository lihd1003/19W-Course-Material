from bs4 import BeautifulSoup
import os, time
import argparse


LINK_HTML = '<li class="breadcrumb-item"><a href="{link}">{name}</a></li>'

ENTRY_HTML = """
<div class="col-lg-4 ol-md-6 mb-2">
          <a href="{link}" class="text-dark">
          <div class="boxed p-2 scaling">
            <div class="row align-items-center justify-content-between">
              <div class="col-10">
                <div class="media align-items-center">
                  <h4><i class="icon-{icon} mr-2 type"></i></h4>
                  <div class="media-body">
                    <h6 class="mb-0 dentry-name name">{name}</h6>
                    <span class="text-muted mtime">{mtime}</span>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </a>
        </div>
"""

EXCLUDE = ['ipynb', 'assets', 'index', '.tpl', '.git', 'README.md']



def format_entry(i):
    for exc in EXCLUDE:
        if exc in i:
            return None
        
    _, fext = os.path.splitext(i)
    icon = "code"
    type_ = "Regular File"

    mtime = os.path.getmtime(i)
    mtime = time.strftime("%X %x", time.localtime(mtime))
    link = "./" + os.path.basename(i)

    if os.path.isdir(i):
        icon = "folder"
        type_ = "Folder"
        link += "/index.html"
    elif fext.lower() == ".html":
        icon = "book"
        type_ = "Notebook"
    elif fext.lower() == ".pdf":
        icon = "file-text"
        type_ = "PDF File"

    return ENTRY_HTML.format(link=link, icon=icon, 
                             name=os.path.basename(i), mtime=mtime)

def create_index(dir_path, template):
    
    
    html = open(template).read()
    soup = BeautifulSoup(html, "lxml")
    path = soup.find(id="path")
    dentry = soup.find(id="dentry")
    abspath = os.path.abspath(dir_path)
    absp = []
    abspath, curr = os.path.split(abspath)
    while curr != "":
        absp.append(curr)
        abspath, curr = os.path.split(abspath)
    absp.reverse()
    absindex = absp.index('UofT-Course-Material-Repo')
    
    if absindex != -1 and len(absp[absindex:]) >= 2:
        end = absindex + 2
        while end <= len(absp):
            link = "/" + "/".join(absp[absindex: end]) + "/index.html"
            name = absp[end-1]
            insert_string = LINK_HTML.format(link=link, name=name)
            insert_soup = BeautifulSoup(insert_string, 'html.parser')
            path.append(insert_soup)
            end += 1

    files = os.listdir(dir_path)
    files.sort()
    for i in files:
        insert_string = format_entry(dir_path + i)
        if insert_string:
            insert_soup = BeautifulSoup(insert_string, 'html.parser')
            dentry.append(insert_soup)
            
    return soup


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-a", "--all", help="Update index for all directory in the repo", action="store_true")
    parser.add_argument("-d", "--dir", help="Update index for the specified directory")
    args = parser.parse_args()
    
    if args.dir and os.path.isdir(args.dir):
        index = create_index(args.dir + "/", "./template.html")
        with open(args.dir + "/index.html", "w") as f:
            f.write(str(index))
    
    if args.all:
        folder_set = set([i[0] for i in os.walk("../")])
        all_folder = []
        for i in folder_set:
            inc = True
            for exc in EXCLUDE:
                if exc in i:
                    inc = False
                    break
            if inc:
                all_folder.append(i+ "/")


        for i in all_folder:
            index = create_index(i, "./template.html")
            with open(i + "/index.html", "w") as f:
                f.write(str(index))