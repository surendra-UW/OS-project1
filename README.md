

Approach:
wman: 
The idea here is to go through man_pages directory with readdir list out all the sub-directories all further iterate if the
subdirectory is also a directory. But, the caveat here is readdir() doesnt gurantee the in order traversel.
So, went back to the simple way of looping over 1 to 9 man pages and looking into each directory.
Compare the command with file names using compareFiles() custom method which excludes the file extension while comparing.
If a file name match is found open the file using fopen and print out the contents.

wapropos:
Look out of the Name and Description section in the man pages and if find the section start looking into the content
to find the given keyword using strstr(). If a match is found exit the file and print the name line with proper formatting.
When a name section is found save the next line of it into some temp variable so that we can print the text at the end.
If no match is found print the no match response.

wgroff:
TH header is always the first line so get the first line in the document and validate it.
If any of the conditions fail output it.

Look for the SH header and convert the section name to upper case using strUpper() custom function.
validate the sh header as well
For every line scanned replace the /fB, /fI, /fU, /fP, // strings with respective ANSI characters using the 
custom function replaceSubstring() which internally uses strstr and strncpy to replace the search text with replace text
