# University of Toronto Course Material Repository

Quick View the files <a href="https://lihd1003.github.io/UofT-Course-Material-Repo/">lihd1003.github.io/UofT-Course-Material-Repo/</a>

The repository of slides, notes, and other course materials from U of Toronto. 

## Using Github Pages as a file navigator


You can also make your own notes portal, so that people can easily view their PDFs and notes on various devices. The steps are simple: 
1. Enable Github Pages on your Repo
2. copy `index.html` and `table_of_content` into your repository
3. change information in `table_of_content/host_info.json`
4. Push to Github, and you can browse your notes on `USER_NAME.github.io/REPO_NAME`
5. Each time a change is made in your repo, run `python updateTofC.py` to update the Table of Content and then push your changes 

This project uses a naive implementation of a file navigation system. Due to the static nature of Github Pages, I used a Python script to iterate through all files and save their paths. Due to the naive implementation, please make sure the repository is not too large. 

Since the purpose of the page is to see the notes on the go, I purposely filter out all `.ipynb, .md, .tex` files, since they cannot be rendered directly by the browser. Please make compiled copy (PDF, HTML, or other browser friendly format) of your files for easier browsering.    
