import os
import re
import json

if __name__ == "__main__":

    files = []
    for i in os.walk("./"):
        for f in i[2]:
            if re.match(".+(.html|.pdf)", f):
                fname = i[0] + "/"+f
                if any(k in fname for k in ["checkpoint", "FORM", "CSC309", "index.html"]):
                    continue
                else:
                    files.append(fname)

    tups = []
    for file in files:
        file = file.replace("\\", "/")
        document = file.split("/")
        for i in range(1, len(document)):
            name = document[i]
            path = "/".join(document[:i])
            if i != len(document) - 1:
                type_ = "folder"
            elif ".html" in document[i]:
                type_ = "note"
            elif ".pdf" in document[i]:
                type_ = "book"
            tups.append((name, path, type_))
    folders = []
    for t in set(tups):
        folders.append({
            'name': t[0],
            'path': t[1],
            'type': t[2]
        })

    with open("./table_of_content.json", "w") as f:
        json.dump(folders, f, indent=2)
