Main C assignment - pnmdump.exe
==========================

Author
------

Ziran He


Date
----

23 Mar 2018


Description
-----------

This C program includes three parts and an extension part. 

The main function of the first part is to read from the input file and print out the hexadecimal representation and the printable text representation of each byte. The non-printable bytes are replaced by '.'.

In the second part, firstly, the program can convert the P2 pgm file to P5 pgm file. Secondly, the program can convert P5 pgm file to P2 pgm file. Thirdly, the program can reflect either P2 or P5 pgm file. Note that if the image is larger than the 512*512, the reflection might fail. 

In the third part, the program can rotate either P2 or P5 pgm file to right by 90 degrees. Note that if the image is larger than the 512*512, the rotation might fail. 

In the extension part, there are 3 new functions. 

- Firstly, the program can expand or contract P2 or P5 pgm file by an input positive integer factor. Note that depending on the size of the image, if the size of the image is very big, for example, 512*512, in this case, if the expand factor is a very large number the expanding might fail ('Segmentation fault 11').

- Secondly, the program could emphasize the black colour or white colour of the P2 or P5 pgm file by an input positive integer factor. 

- Thirdly, the program can blur or gaussian blur a P2 or P5 pgm file by an input positive odd integer factor. The blur function makes the image become more and more unclear by increase the factor. In contrast, the gaussian blur is more smooth, and the difference between gaussian blur and blur is the outline of the image is still clear by increasing the factor and parameter. In other words, gaussian blur is similar to an short-sighted view of the image. Note that for blur, if the factor is larger than the columns or rows, the blur will fail (return the same image). Since if the factor is greater than the columns or rows, it will directly return the original image to avoid the 'Segmentation fault 11'. Note that for gaussian blur, except the factor which is an odd positive integer, a positive float is also required as an input argument (sixth argument). 


Compile the code
----------------

Run "make" in the pnmdumpFinal directory to compile the code.

Run "make test" int the pnmdumpFinal directory to run the test file.


The way to use the code
-----------------------

Run
    
    $ ./pnmdump.exe

 to run the code.

Run 
    
    $ ./pnmdump.exe --usage

 to see all the commands.

Run 
    
    $ ./pnmdump.exe --version
 
 to obtain the version.

Run 
    
    $ ./pnmdump.exe --hexdump [FILE]

 to read and print the file.

Run 

    $ ./pnmdump.exe --P2toP5 INFILE OUTFILE

 to convert P2 file to P5 file.

Run 

    $ ./pnmdump.exe --P5toP2 INFILE OUTFILE

 to convert P5 file to P2 file.

Run 

    $ ./pnmdump.exe --rotate INFILE OUTFILE

 to reflect P2 or P5 file.

Run 

    $ ./pnmdump.exe --rotate90 INFILE OUTFILE
 
 to rotate P2 or P5 file to right 90 degrees.

Run 

    $ ./pnmdump.exe --expand INFILE OUTFILE factor(a positive integer)
 
 to expand the P2 or P5 file by the corresponding input factor.

Run 
 
    $ ./pnmdump.exe --contract INFILE OUTFILE factor(a positive integer)

 to contract the P2 or P5 file by the corresponding input factor.

Run 

    $ ./pnmdump.exe --emphasizeblack INFILE OUTFILE factor(an integer > 1)

 to emphasize the black colour of P2 or P5 file by the corresponding input factor.

Run 
    
    $ ./pnmdump.exe --emphasizewhite INFILE OUTFILE factor(an integer > 1) 
 
 to emphasize the white colour of P2 or P5 file by the corresponding input factor.
Run 

    $ ./pnmdump.exe --blur INFILE OUTFILE factor(a positive integer) 
 
 to blur the P2 or P5 file by the corresponding input factor.

Run 

    $ ./pnmdump.exe --gaussianblur INFILE OUTFILE factor(a positive odd integer) parameter(a positive float) 
 
 to make the gaussian blur of the P2 or P5 file by the corresponding input factor and parameter. The suggested factor and parameter are 5 and 2. Users could increase these numbers gradually to observe the difference output image.

Note: The INFILE and OUTFILE are the name of the input file and the output file respectively. In addition, the factor is a positive integer or a positive odd integer (in blur and gaussian blur). Moreover, the parameter is a positive float.


Reference
---------

[1] Collins,R. Lecture 4: Smoothing. Available at: http://www.cse.psu.edu/~rtc12/CSE486/lecture04.pdf [Accessed Mar 20, 2018].

[2] Salazar,R. (2017). HOW TO PROGRAM A GAUSSIAN BLUR WITHOUT USING 3RD PARTY LIBRARIES. S0hungry. Available at: http://s0hungry.com/2017/08/29/how-to-program-a-gaussian-blur-without-using-3rd-party-libraries/ [Accessed Mar 21, 2018].


Changelog
---------
22 Feb 2018 - Fix printable characters and non-printable characters
23 Feb 2018 - Fix pass a pointer argument to a function (FILE* inputfile)
05 Mar 2018 - Use fscanf to read the P2 file, Use fread to read the P5 file
09 Mar 2018 - Replace all the global variables
10 Mar 2018 - Use struct and struct function to return first four lines
10 Mar 2018 - Fix hexdump(stdin)
11 Mar 2018 - Include <stdlib.h> to use the exit(1) to terminate the program
12 Mar 2018 - Minimise the length of main function.
13 Mar 2018 - Fix the contract
16 Mar 2018 - Fix passing array into a gaussian blur function as an argument
19 Mar 2018 - Change unsigned int into unsigned char
19 Mar 2018 - Fix one problem with the return value of the fscanf
21 Mar 2018 - Fix the unbalanced gaussian blur square problem
22 Mar 2018 - add more specific comments
