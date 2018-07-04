# WATlang Compiler

An assignment which is a part of course [01076262 COMPILER CONSTRUCTION](http://www.reg.kmitl.ac.th/teachtable_v20/subject_detail.php?subject_id=01076262) in semester 2, academic year 2017 under the [Department of Computer Engineering](http://www.ce.kmitl.ac.th), Faculty of Engineering at [KMITL](http://www.kmitl.ac.th)

## Running

1. Install dependencies
    - Ubuntu
    ```bash
    apt-get install build-essential flex bison
    ```

1. Edit test program in `test.in` if you wish (document is WIP). There are some correct code example in PASS directory and incorrect code in ERROR directory

1. Run command
    ```bash
    make
    ```

1. To clean up file after run, do run command
    ```bash
    make clean
    ```

## Known issue

There is a problem where linking error with ld version 2.30 [under investigating]