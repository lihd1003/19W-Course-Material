# Notebook
https://lihd1003.github.io/notebook

This is the note repo for the some courses I've taken @UofT and some additional things.

## Why using github pages as a notebook?
As a computer science student, my requirements for a notebook include
 1. code snipplets and demo output.
 2. LaTeX support for typing multi-line math equations. 
 3. Quick editing, no extra hassle for formatting.
 4. Viewing on mobile devices with small screen friendly formatting.
 
For most notebook apps, 1. and 2. are barely satisfied, while they are quite important to me. Jupyter notebook is definitely ideal for computer scientists, while there's no jupyter package on mobile (maybe possible, but requires some hacks). 

Then, I realized that you can view jupyter notebooks in Github repos and viewable on mobile. Unfortunately, it's slow and incapable of showing long notebooks. However, as Github renders Jupyter notebook into HTML, why cannot I do that? Jupyter is browser based and have great support for HTML conversion. The only things left are a good templating tools and a web hosting services, which are Jekyll and Github Pages. 

## How is the notebook constructed
The repo has two parts, the `notebook` dir contains all the source jupyter notebooks for editing, and the `doc` dir contains the static websites and Jekyll templating files for viewing. The two parts are connected by the `ipynb2html.py` script, which converts jupyter notebooks to HTML and copy/move necessary assets. 


