# PDFBookmark
An open source tool to add bookmarks to pdfs.

<h2>What is PDFBookmark</h2>
Using outline/bookmarks is the conventional way of navigating a pdf document. Otherwise the navigation becomes too cumbersome.
No simple open source ways exist currently for adding bookmarks/ contents to a pdf document.

<strong>PDFBookmark</strong> is a tool to allow adding bookmarks/outline to a pdf file that lacks it with minimum hassle.

<h3>The Problem</h3>
Almost all options directly or indirectly rely on Adobe Acrobat for doing this. Even the open source software suits like LibreOffice, OpenOffice etc don’t have a solution for this.

A person with cognitive disabilities may prefer a hierarchical outline that provides an overview of the document rather than reading and traversing through many pages. 

<h3>How to install</h3>
The c code is self sufficient to create a PDFBookmark executable.

Simply execute:

```
gcc -o PDFBookmark.exe pdfbm.c
```

(you must have gcc installed and added to path)

<h3>Usage</h3>

PDFBookmark needs the following two files in the same directory it resides in to work. These are explained in the following lines:
  1. The pdf file (aka the input file) in which you wish to add bookmarks
  2. A file named 'bookmarks.file' which has bookmark information (one per line) [look at example bookmarks.file file in repo]

<h2>For windows:</h2>

Simply place both the files together and run PDFBookmark:
  1. Enter the input filename
  2. Enter the output filename
  3. Select the option 'Begin adding bookmarks'

<h4>How to write a bookmarks.file</h4>
  Write  <strong>page number</strong>  followed by  <strong>title of the bookmark</strong> , separated by a <strong>space</strong>.
  
  1. One bookmark per line only.
  2. Use tabs (only) to create child bookmarks/ tree structure. <strong>No 4 spaces</strong> 
  3. Example: 20 Page Twenty
 
 To set a bookmark to page 20 titled as 'Page Twenty'.
 
**NOTE:** The page number must not be out of bounds of total available pages in the pdf file (especially while adding offset to the bookmarks).

<h3>In action</h3>
This turns into:

![1](https://user-images.githubusercontent.com/77376446/124363790-fd505600-dc5a-11eb-8a27-ed2345c8753e.png)

Into this:
![2](https://user-images.githubusercontent.com/77376446/124363815-3be61080-dc5b-11eb-9729-95fdcd58eb50.gif)


Linux source code coming soon.

Written with ❤️ in C.
