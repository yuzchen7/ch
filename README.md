# File Checker
To check two text file to see if both are having to some contents

## Installation
Clone or download this project or just the bin directory, and set the bin directory in system environment variable.

## Usage
- ### command:
    ``` bash
        # compare two file if the contents are the same or not
        ch file1.txt file2.txt

        # help command (man)
        ch -m

        # display the version of Checker
        ch -v
    ```
    - #### NOTE: if enter file is more then 2, it will compare the last two enter file
    - #### NOTE: A command operation can have at most one operation parameter at a time. if more then 1 operation parameter, the first parameter is considered as the operation parameter of the current command

- ### result with compare two file:
    - #### without any arguments, display:
        ``` bash
            check: no file arguments
        ```
    - #### file are not found or can not able to open, display:
        ``` bash
            # Error on the first file
            input file1 error : File not Found error

            # Error on the second file
            input file2 error : File not Found error
        ```
    - #### two file not the same, display:
        ``` bash
            # "line #" represent the lines with different content
            result is -> false : line #

            # one of the file has more contents the other file, "line #" represent starts with this line, and there is more contents to follow
            result is -> false : line # and keep on
        ```
        - #### Checker will terminates immediately as soon as it finds a different line and output the result.
    
    - #### two file are the same, display:
        ``` bash
            result is -> true : all line Match
        ```

## DEPENDENCY
### wingetopt:
- #### author: alex85k
- #### link of dependency : https://github.com/alex85k/wingetopt
- Window version of  getopt(...)

## Version
- #### v 1.3.0 Beta
- #### Yuz.chen (Yuzhuang Chen)
