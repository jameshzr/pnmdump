#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


// The struct of the image‘s first four lines
struct image_variable_type
{
    int columns; // the columns of image
    int rows; // the rows of image
    int rotate_columns; // the columns of image after rotate
    int rotate_rows; // the rows of image after rotate
    int expand_columns; // the columns of image after expand
    int expand_rows; // the rows of image after expand
    int contract_columns; // the columns of image after contract
    int contract_rows; // the rows of image after contract
    
    int maxpixel ; // the maximum pixel value
    int defined_format; // the format for the file (P2 or P5) which will be opended;
    
} typedef image; // imgage_pgm_data


void print_badargument ();
void print_usage ();
void hexdump (FILE* inputfile);
void hexdump_inputfile(char* inputfilename);

int file_test (FILE* inputfile, FILE* outputfile,char* inputfilename,char* outputfilename);

image read_write_four_lines(FILE* inputfile,int format,FILE* outputfile,int rotate,int expand,int factor,int contract,int emphasizeblack,int emphasizewhite,int blur,int gaussian_blur);

int readP2_writeP5_remainder(FILE* inputfile,int columns,int rows,int maxpixel,FILE* outputfile);
int readP5_writeP2_remainder(FILE* inputfile,int columns,int rows,FILE* outputfile);

int rotateP2_remainder(FILE* inputfile,int columns,int rows,int rotate_columns,int rotate_rows,int maxpixel,int direction,FILE* outputfile);
int rotateP5_remainder(FILE* inputfile,int columns,int rows,int rotate_columns,int rotate_rows,int direction,FILE* outputfile);

void P2toP5(char* inputfilename,char* outputfilename);
void P5toP2(char* inputfilename,char* outputfilename);
void rotate(char* inputfilename, char* outputfilename);
void rotate90(char* inputfilename, char* outputfilename);

//###############################
//Extension part

int factor_test(char* argument4);
//Expand and contract
int expand_P2_remainder(FILE* inputfile,int columns,int rows,int expand_columns,int expand_rows,int maxpixel,int factor,FILE* outputfile);
int contract_P2_remainder(FILE* inputfile,int columns,int rows,int contract_columns, int contract_rows,int maxpixel,int factor,FILE* outputfile);
int expand_P5_remainder(FILE* inputfile,int columns,int rows,int expand_columns,int expand_rows,int factor,FILE* outputfile);
int contract_P5_remainder(FILE* inputfile,int columns,int rows,int contract_columns, int contract_rows,int factor,FILE* outputfile);
void expand(char* inputfilename,char* outputfilename,char* argument4);
void contract(char* inputfilename,char* outputfilename,char* argument4);

//Emphasize black or white
int emphasize_black_white_P2_remainder(FILE* inputfile,int columns,int rows,int maxpixel,int factor,int emphasizeblack, int emphasizewhite, FILE* outputfile);
int emphasize_black_white_P5_remainder(FILE* inputfile,int columns,int rows,int maxpixel,int factor,int emphasizeblack, int emphasizewhite, FILE* outputfile);
void emphasize_black(char* inputfilename,char* outputfilename,char* argument4);
void emphasize_white(char* inputfilename,char* outputfilename,char* argument4);

//Average blur
int blur_P2_remainder(FILE* inputfile,int columns, int rows, int maxpixel,int factor,FILE* outputfile);
int blur_P5_remainder(FILE* inputfile,int columns, int rows, int maxpixel,int factor,FILE* outputfile);
void blur(char* inputfilename,char* outputfilename,char* argument4);

//Gaussian blur
float parameter_test(char* argument5);
int gaussian_weight(int factor, float parameter,unsigned char data[][factor]);
int gaussian_blur_P2_remainder(FILE* inputfile,int columns, int rows, int maxpixel,int factor,float parameter, FILE* outputfile);
int gaussian_blur_P5_remainder(FILE* inputfile,int columns, int rows, int maxpixel,int factor,float parameter,FILE* outputfile);
void gaussian_blur(char* inputfilename, char* outputfilename, char* argument4, char* argument5);


//########################
//########################
// main function
int main(int argc, char *argv[])
{
    if (argc == 2) // one of the right number of input arguemnt
    {
        if (strcmp("--usage", argv[1]) == 0)  // usage
        {
            print_usage ();
            return 0;
        }
        else if ((strcmp("--version", argv[1]) == 0)) // version
        {
            printf("1.0\n");
            return 0;
        }
        else if (strcmp("--hexdump", argv[1]) == 0) // hexdump
        {
            hexdump (stdin); // read file from standard input
            return 0;
        }
        else // Wrong input argument
        {
            print_badargument ();
            return 1;
        }
    }
    else if (argc == 3) // one of the right number of input argument
    {
        if (strcmp("--hexdump", argv[1]) == 0) // is --hexdump
        {
            hexdump_inputfile(argv[2]);
            return 0;
        }
        else // Wrong input argument
        {
            print_badargument ();
            return 1;
        }
    }
    else if (argc == 4) //image file
    {
        if (strcmp ("--P2toP5", argv[1]) == 0) // convert P2 to P5 (ASCII to Binary)
        {
            P2toP5(argv[2],argv[3]);
            return 0;
        }
        else if (strcmp("--P5toP2",argv[1]) == 0) // convert P5 to P2 (Binary to ASCII)
        {
            P5toP2(argv[2],argv[3]);
            return 0;
        }
        else if (strcmp("--rotate",argv[1]) == 0) // reflect the iamge
        {
            rotate(argv[2],argv[3]);
            return 0;
        }
        else if (strcmp("--rotate90",argv[1]) == 0) // rotate the iamge to right 90 degrees (-90)
        {
            rotate90(argv[2],argv[3]);
            return 0;
        }
        else // wrong input arguemnt
        {
            print_badargument();
            return 1;
        }
    }
    else if (argc == 5) // extension part
    {
        if (strcmp ("--expand", argv[1]) == 0) // expand the image
        {
            expand(argv[2],argv[3],argv[4]);
            return 0;
        }
        else if (strcmp("--contract", argv[1]) == 0) // contract the image
        {
            contract(argv[2],argv[3],argv[4]);
            return 0;
        }
        else if (strcmp("--emphasizeblack", argv[1]) == 0) // emphasize the black colour of the image
        {
            emphasize_black(argv[2],argv[3],argv[4]);
            return 0;
        }
        else if (strcmp("--emphasizewhite",argv[1]) == 0) // emphasize the white colour of the image
        {
            emphasize_white(argv[2],argv[3],argv[4]);
            return 0;
        }
        else if (strcmp("--blur",argv[1]) == 0) // blur the image
        {
            blur(argv[2],argv[3],argv[4]);
            return 0;
        }
        else // wrong input arguemnt
        {
            print_badargument();
            return 1;
        }
    }
    else if (argc == 6) // extension part
    {
        if(strcmp("--gaussianblur",argv[1]) == 0) // gaussian blur the image
        {
            gaussian_blur(argv[2],argv[3],argv[4],argv[5]);
            return 0;
        }
        else // wrong input argument
        {
            print_badargument();
            return 1;
        }
    }
    else if(argc < 2 || argc > 6) // wrong number of input argument
    {
        print_badargument ();
        return 1;
    }
}

// A function to print the badarguments
void print_badargument ()
{
    fprintf(stderr,"pnmdump: bad arguments\n");
    fprintf(stderr,"Usage:\n");
    fprintf(stderr,"./pnmdump.exe --version\n");
    fprintf(stderr,"./pnmdump.exe --usage\n");
    fprintf(stderr,"./pnmdump.exe --hexdump [FILE]\n");
}

// A function to print the usage
void print_usage ()
{
    printf("Usage:\n");
    printf("./pnmdump.exe --version\n");
    printf("./pnmdump.exe --usage\n");
    printf("./pnmdump.exe --hexdump [FILE]\n");
    printf("./pnmdump.exe --P2toP5 INFILE OUTFILE\n");
    printf("./pnmdump.exe --P5toP2 INFILE OUTFILE\n");
    printf("./pnmdump.exe --rotate INFILE OUTFILE\n");
    printf("./pnmdump.exe --rotate90 INFILE OUTFILE\n");
    printf("./pnmdump.exe --expand INFILE OUTFILE factor(a positive integer)\n");
    printf("./pnmdump.exe --contract INFILE OUTFILE factor(a positive integer)\n");
    printf("./pnmdump.exe --exphasizeblack INFILE OUTFILE factor(a positive integer)\n");
    printf("./pnmdump.exe --exphasizewhite INFILE OUTFILE factor(a positive integer)\n");
    printf("./pnmdump.exe --blur INFILE OUTFILE factor(a positive integer)\n");
    printf("./pnmdump.exe --gaussianblur INFILE OUTFILE factor(a positive odd integer) parameter(a positive float)\n");
}

// hexdump function, read fomr the input file or stdin and print in the terminal
void hexdump (FILE* inputfile)
{
    unsigned char chunk[8];  // A size of 8 array to store the characters read by the 'fraed' function
    int bytes_read; // Return value of the fread function
    int previous_bytes_read = 0; // Used for the last line since it might not fill all the elements of chunk[8].
    int i = 0; // used for forever loop
    for (;;)
    {
        bytes_read = fread(chunk,1,8,inputfile);
        if (bytes_read == 0) // There is nothing to read
        {
            printf("%07x\n",(8*(i-1)+previous_bytes_read)); // The total bytes
            break;
        }
        else
        {
            printf("%07x  ",8*i); // The line number
            for (int char_pos = 0; char_pos < bytes_read; char_pos++)
            {
                if (chunk[char_pos] > 126 || chunk[char_pos] < 32) // indicate to the non-printable characters
                {
                    if (char_pos + 1==bytes_read) // Last character in this line, new line is needed
                    {
                        printf("%02X .\n",chunk[char_pos]);
                    }
                    if (char_pos + 1<bytes_read) // In the middle of the line
                    {
                        printf("%02X .  ",chunk[char_pos]);
                    }
                }
                if (chunk[char_pos] >= 32 && chunk[char_pos] <= 126) // indicate to the printable characters
                {
                    if (char_pos + 1==bytes_read) // Last character in this line, new line is needed
                    {
                        printf("%02X %c\n",chunk[char_pos],chunk[char_pos]);
                        
                    }
                    if (char_pos + 1 <bytes_read) // In the middle of the line
                    {
                        printf("%02X %c  ",chunk[char_pos],chunk[char_pos]);
                    }
                }
            }
            previous_bytes_read = bytes_read;
        }
        i++;
    }
}


// A conbined function for hexdump to minimise the length of the mian function
void hexdump_inputfile(char* inputfilename)
{
    FILE* inputfile = fopen(inputfilename, "rb");//open and read binary
    if (inputfile == NULL) // inputfile does not exist
    {
        fprintf(stderr, "No such file: \"%s\"\n",inputfilename);
        exit (1);
    }
    else
    {
        hexdump (inputfile); // call the hexump function
    }
}

// A function to test the existence of the input and outfut file
int file_test (FILE* inputfile, FILE* outputfile,char* inputfilename,char* outputfilename)
{
    if (inputfile == NULL) // Inpuftile does not exist
    {
        fprintf(stderr,"No such file: \"%s\"\n",inputfilename);
        exit (1);
    }
    if (outputfile == NULL) // Outoutfile does not exist
    {
        fprintf(stderr,"No such file: \"%s\"\n",outputfilename);
        fclose (inputfile);
        exit (1);
    }
    return 0;
}

// A strct function to read and write first four lines, the return value is the strut
image read_write_four_lines(FILE* inputfile,int format,FILE* outputfile,int rotate,int expand,int factor,int contract,int emphasizeblack,int emphasizewhite,int blur, int gaussian_blur)
{
    image image_variable; // initialize the strct
    char line[250]; // store the characters that read fomr the four lines, the reason of using size of 250 is the maximum characters in the second line is 250, and all the other line's characters are smaller than this number.
    int saved_number[1]; // sort out integer from the sscanf function, for P2 or P5
    char saved_char[250]; // sort out characters form the four lines
    for (int i=1;i<5;i++) // from line 1 to line 4
    {
        if (i == 1) // line 1
        {
            fgets(line,250,inputfile); // Since the maximum characters in the four lines is the second line which is the 250 characters
            if (!sscanf(line,"P%d",&saved_number[0]))
            {
                fprintf(stderr,"Input file is not in the correct format\n");
                exit (1);
            }
            if (rotate == 0 && expand == 0 && contract ==0 && emphasizeblack == 0 && emphasizewhite == 0 && blur == 0 && gaussian_blur == 0) // convert mode
            {
                if (format != saved_number[0]) // Wrong format, compared to the number we saved from the first line
                {
                    if (format == 2)
                    {
                       fprintf(stderr,"Input is not in P2 format\n");
                    }
                    if (format == 5)
                    {
                        fprintf(stderr,"Input is not in P5 format\n");
                    }
                    exit (1);
                }
                if (format == 2)
                {
                    line[1] = 53; //5 + 48 = 53, write P5
                }
                if (format == 5)
                {
                    line[1] = 50; //2 + 48 = 50, write P2
                }
            }
            if (rotate == 1 || expand == 1 || contract == 1 || emphasizeblack == 1 || emphasizewhite == 1 || blur == 1 || gaussian_blur == 1) // rotate, expand, contract , emphasizeblack, emphasizewhite mode
            {
                // keep th same format number
                if (line[1] == 50) // 2+48
                {
                    format = 2; //P2 file
                }
                else if (line[1] == 53) // 5+48
                {
                    format = 5; // P5 file
                }
            }
        }
        if (i == 2) // line 2
        {
            fgets(line,250,inputfile);
            if (!sscanf(line,"# %s",saved_char))
            {
                fprintf(stderr,"Input file is not in the correct format\n");
                exit (1);
            }
        }
        if (i == 3) // line 3
        {
            if (!fscanf(inputfile,"%d",&image_variable.columns)) // sort out the columns
            {
                {
                    fprintf(stderr,"Corrupted input file\n");
                    exit (1);
                }
            }
            if (!fscanf(inputfile,"%d",&image_variable.rows)) // sort out the rows
            {
                {
                    fprintf(stderr,"Corrupted input file\n");
                    exit (1);
                }
            }
            fgetc(inputfile); // after the fscanf, there is a newline character needs to be read.
            
            if (rotate == 0 && expand == 0 && contract == 0 && emphasizeblack == 0 && emphasizeblack == 0 && emphasizewhite == 0 && blur == 0 && gaussian_blur == 0) // convert mode
            {
                fprintf(outputfile,"%d %d\n",image_variable.columns,image_variable.rows); // directly write the columns and rows
            }
            if (rotate == 1) // rotate mode
            {
                image_variable.rotate_columns = image_variable.rows;
                image_variable.rotate_rows = image_variable.columns;
                fprintf(outputfile,"%d %d\n",image_variable.rotate_columns,image_variable.rotate_rows); // switch the rows and columns
            }
            if (expand == 1 ) // expand mode
            {
                image_variable.expand_columns = image_variable.columns*factor;
                image_variable.expand_rows = image_variable.rows*factor;
                fprintf(outputfile,"%d %d\n",image_variable.expand_columns,image_variable.expand_rows); // write the expand rows and columns
            }
            if (contract == 1 ) // contract mode
            {
                image_variable.contract_columns = image_variable.columns/factor;
                image_variable.contract_rows = image_variable.rows/factor;
                fprintf(outputfile,"%d %d\n",image_variable.contract_columns,image_variable.contract_rows); // write the contract rows and columns
            }
            if (emphasizeblack == 1) // emphasizeblack mode
            {
                fprintf(outputfile,"%d %d\n",image_variable.columns,image_variable.rows); // directly write the columns and rows
            }
            if (emphasizewhite == 1) // emphasizewhite mode
            {
                fprintf(outputfile,"%d %d\n",image_variable.columns,image_variable.rows); // directly write the columns and rows
            }
            if (blur == 1) // blur mode
            {
                fprintf(outputfile,"%d %d\n",image_variable.columns,image_variable.rows); // directly write the columns and rows
            }
            if (gaussian_blur == 1) // gaussian_blur  mode
            {
                fprintf(outputfile,"%d %d\n",image_variable.columns,image_variable.rows); // directly write the columns and rows
            }
        }
        if (i == 4) // line 4
        {
            fgets(line,250,inputfile);
            if (!sscanf(line,"%d",&image_variable.maxpixel)) // sort out the maximum pixel
            {
                fprintf(stderr,"Input file is not in the correct format\n");
                exit (1);
            }
        }
        if (i != 3 ) // since in line 3 the read function used is fsancf, so in line 3, we use fprintf to write the rows and columns
        {
           fputs(line,outputfile);
        }
    }
    if ((rotate == 1 || expand == 1 || contract == 1 || emphasizeblack == 1|| emphasizewhite == 1 || blur == 1 || gaussian_blur == 1)&& format == 2) // for not convert mode, the format P2 is defined
    {
        image_variable.defined_format =2;
        return image_variable; // return the struct
    }
    if ((rotate == 1 || expand == 1 || contract == 1 || emphasizeblack == 1 || emphasizewhite == 1 || blur == 1 || gaussian_blur == 1 ) && format == 5) // for not convert mode, the format P5 is defined
    {
        image_variable.defined_format =5; // for rotate mode
        return image_variable; // return the struct
    }
    else
    {
        return image_variable; // return the struct
    }
}

// A function read P2 file's remainder and write as P5 file format
int readP2_writeP5_remainder(FILE* inputfile,int columns,int rows,int maxpixel,FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    int number_buffer; // Use this to test if each element is greater than the maxpixel
    // read
    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<columns;j++)
        {
            if (fscanf(inputfile,"%d",&number_buffer) != 1  || number_buffer > maxpixel) // read each element in the remainder, and each element should be integer and smaller than the maximum pixel
            {
                fprintf(stderr,"Corrupted input file\n");
                exit(1);
            }
            else
            {
                remainder[i][j] = (unsigned char) number_buffer; // convert number_buffer back to unsigned char
                // The reason of using unsigned char array is the bytes of the unsigned char is 1, and the bytes of the integer is 4, as a result, the size of the array will be smaller if we use the unsigned char array and when we read the big size of image it will be faster. In addition, we know that the maxpixel is 255, which is equal to the max value of the unsigned char(255).
            }
        }

    }
    // write
    fwrite(remainder,columns*rows,sizeof(unsigned char),outputfile); // Writhe the remainder in binary form in the outputfile
    return 0;
}


// A function read P5 file's remainder and write as P2 file format
int readP5_writeP2_remainder(FILE* inputfile,int columns,int rows,FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    // read
    if (rows*columns != fread(remainder,1,rows*columns,inputfile) ) // Read the whole file, and the total bytes in the remainder should be equal to the rows*columns
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    if (fgetc(inputfile) != EOF) // used to check the corrupted file, fread has already read everthing, fgectc should get EOF
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    
    /* Since the fread function read the specific size which is rows*columns, so if the total bytes is less than this, it can be detected by the fitst if statement. However, if the total bytes is larger than this value, it can not be detected, and we know that the fread function has already read the whole remainder, so if fgetc != EOF, which means that there are extra bytes.
     */
    // write
    else
    {
        for (int i=0;i<rows;i++)
        {
            for (int j=0;j<columns;j++)
            {
                fprintf(outputfile,"%hhu",remainder[i][j]); // write the ASCII form remainder in the output file.
                if (j!=columns-1)
                {
                    fputc(32,outputfile); // space
                }
            }
            fputc(10,outputfile); // new line
        }
        return 0;
    }
}


// A function reflect or rotate to right 90 degrees of the P2 file
int rotateP2_remainder(FILE* inputfile,int columns,int rows,int rotate_columns,int rotate_rows,int maxpixel,int direction,FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    unsigned char rotate_remainder[rotate_rows][rotate_columns]; // 2-d array to store the reflected or rotated remainder data
    int number_buffer = 0; // Use this to test if each element is greater than the maxpixel
    // read
    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<columns;j++)
        {
            if (fscanf(inputfile,"%d", &number_buffer) !=1 || number_buffer > maxpixel) // read each element in the remainder, and each element should be integer and smaller than the maximum pixel
            {
                fprintf(stderr,"Corrupted input file\n");
                exit(1);
            }
            remainder[i][j] = (unsigned char) number_buffer; // convert number_buffer back to unsigned char
            // The reason of using unsigned char array is the bytes of the unsigned char is 1, and the bytes of the integer is 4, as a result, the size of the array will be smaller if we use the unsigned char array and when we read the big size of image it will be faster. In addition, we know that the maxpixel is 255, which is equal to the max value of the unsigned char(255).
        }
    }
    // rotate
    for (int i=0;i<rotate_rows;i++)
    {
        for (int j=0;j<rotate_columns;j++)
        {
            if (direction == -90) // refltct;
            {
                rotate_remainder[i][j] = remainder[j][i]; // write the data from left top to left bottom
            }
            if (direction == 90) // rotate to right;
            {
                rotate_remainder[i][j] = remainder[rotate_columns-1-j][i]; // write the data from left bottom to left top
            }
            fprintf(outputfile,"%hhu",rotate_remainder[i][j]); // write the ASCII form remainder in the output file.
            if (j != rotate_columns-1)
            {
                fputc(32,outputfile); // space
            }
        }
        fputc(10,outputfile); // new line
        
    }
    return 0;
}


// A function reflect or rotate to right 90 degrees of the P5 file
int rotateP5_remainder(FILE* inputfile,int columns,int rows,int rotate_columns,int rotate_rows,int direction,FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    unsigned char rotate_remainder[rotate_rows][rotate_columns]; // 2-d array to store the reflected or rotated remainder data
    
    // read
    if (rows*columns != fread(remainder,1,rows*columns,inputfile) ) // Read the total file, and the total bytes in the remainder should be equal to the rows*columns
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    if (fgetc(inputfile) != EOF) // used to check the corrupted file, fread has already read everthing, fgect should get EOF
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    
    /* Since the fread function read the specific size which is rows*columns, so if the total bytes is less than this, it can be detected by the fitst if statement. However, if the total bytes is larger than this value, it can not be detected, and we know that the fread function has already read the whole remainder, so if fgetc != EOF, which means that there are extra bytes.
     */
    // rotate
    else
    {
        for (int i=0;i<rotate_rows;i++)
        {
            for (int j=0;j<rotate_columns;j++)
            {
                if (direction == -90) // refltect
                {
                    rotate_remainder[i][j] = remainder[j][i]; // write the data from left top to left bottom
                }
                if (direction == 90 ) // rotate to right
                {
                    rotate_remainder[i][j] = remainder[rotate_columns-1-j][i]; // write the data from left bottom to left top
                }
            }
        }
        //write
        fwrite(rotate_remainder,rotate_columns*rotate_rows,sizeof(unsigned char),outputfile);// Writhe the remainder in binary form in the output file
    }
    return 0;
}


// A combined function of P2toP5 to minimise the length of the mian function
void P2toP5(char* inputfilename,char* outputfilename)
{
    int format = 2; // initialize P2
    int rotate = 0; // initialize the rotate off
    int expand = 0; // initialize the expand off
    int factor = 0; // unused
    int contract = 0; // initialize the contract off
    int emphasizeblack = 0; // initialize the emphasize black off
    int emphasizewhite = 0; // initialize the emphasize white off
    int blur = 0;// initialize the blur off
    int gaussian_blur =0;// initialize the gaussian_blur off
    FILE* inputfile = fopen(inputfilename, "rb");//open; read ASCII in binary
    FILE* outputfile = fopen(outputfilename,"wb"); // write binary
    file_test (inputfile,outputfile,inputfilename,outputfilename); // test whether the inputfile and outputfile exist
    image image_variable = read_write_four_lines(inputfile,format,outputfile,rotate,expand,factor,contract,emphasizeblack,emphasizewhite,blur,gaussian_blur);  // read and write first four lines
    readP2_writeP5_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.maxpixel,outputfile); // read and write the remainder
    fclose(inputfile); // close the input file
    fclose(outputfile); // close the output file
}

// A combined function of P5toP2 to minimise the length of the mian function
void P5toP2(char* inputfilename,char* outputfilename)
{
    int format = 5; // initialize P5
    int rotate = 0; // initialize the rotate off
    int expand = 0; // initialize the expand off
    int factor = 0; // unused
    int contract = 0; // initialize the contract off
    int emphasizeblack = 0; // initialize the emphasize black off
    int emphasizewhite = 0; // initialize the emphasize white off
    int blur = 0;// initialize the blur off
    int gaussian_blur =0;// initialize the gaussian_blur off
    FILE* inputfile = fopen(inputfilename, "rb");//open; read binary
    FILE* outputfile = fopen(outputfilename,"w"); // write ASCII
    file_test (inputfile,outputfile,inputfilename,outputfilename); // test whether the inputfile and outputfile exist
    image image_variable = read_write_four_lines(inputfile,format,outputfile,rotate,expand,factor,contract,emphasizeblack,emphasizewhite,blur,gaussian_blur);  // read and write first four lines
    readP5_writeP2_remainder(inputfile,image_variable.columns,image_variable.rows,outputfile); // read and write the remainder
    fclose(inputfile); // close the input file
    fclose(outputfile); // close the output file
}

// A combined function of rotate to minimise the length of the mian function
void rotate(char* inputfilename, char* outputfilename)
{
    int format = 0; // initialize P2 or P5
    int rotate = 1; // rotate mode activate
    int expand = 0; // initialize the expand off
    int direction = -90; // reflect the image
    int factor = 0; // unused
    int contract = 0; // initialize the contract off
    int emphasizeblack = 0; // initialize the emphasize black off
    int emphasizewhite = 0; // initialize the emphasize white off
    int blur = 0;// initialize the blur off
    int gaussian_blur =0;// initialize the gaussian_blur off
    FILE* inputfile = fopen(inputfilename, "rb");//open; read binary
    FILE* outputfile = fopen(outputfilename,"w");
    file_test (inputfile,outputfile,inputfilename,outputfilename); // test whether the inputfile and outputfile exist
    image image_variable = read_write_four_lines(inputfile,format,outputfile,rotate,expand,factor,contract,emphasizeblack,emphasizewhite,blur,gaussian_blur);  // read and write first four lines
    if (image_variable.defined_format == 2) // rotate P2 file
    {
        rotateP2_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.rotate_columns,image_variable.rotate_rows,image_variable.maxpixel,direction,outputfile);
    }
    else // rotate P5 file
    {
        rotateP5_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.rotate_columns,image_variable.rotate_rows,direction,outputfile);
    }
    fclose(inputfile); // close the input file
    fclose(outputfile); // close the output file
}

// A combined function of rotate90 to minimise the length of the mian function
void rotate90(char* inputfilename, char* outputfilename)
{
    int format = 0; // initialize P2 or P5
    int rotate = 1; // rotate mode activate
    int expand = 0; // initialize the expand off
    int direction = 90; // rotate to right 90 degrees (90)
    int factor = 0; // unused
    int contract = 0; // initialize the contract off
    int emphasizeblack = 0; // initialize the emphasize black off
    int emphasizewhite = 0; // initialize the emphasize white off
    int blur = 0;// initialize the blur off
    int gaussian_blur =0;// initialize the gaussian_blur off
    FILE* inputfile = fopen(inputfilename, "rb");//open; read binary
    FILE* outputfile = fopen(outputfilename,"w"); // write ASCII
    file_test (inputfile,outputfile,inputfilename,outputfilename); // test whether the inputfile and outputfile exist
    image image_variable = read_write_four_lines(inputfile,format,outputfile,rotate,expand,factor,contract,emphasizeblack,emphasizewhite,blur,gaussian_blur);  // read and write first four lines
    if (image_variable.defined_format == 2) // rotate90 P2 file
    {
        rotateP2_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.rotate_columns,image_variable.rotate_rows,image_variable.maxpixel,direction,outputfile);
    }
    if (image_variable.defined_format == 5) // rotate90 P5 file
    {
        rotateP5_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.rotate_columns,image_variable.rotate_rows,direction,outputfile);
    }
    fclose(inputfile); // close the input file
    fclose(outputfile); // close the output file
}

//#########################################################
// Extension part
// obtain the factor from the stdin
int factor_test(char* argument4)
{
    int factor;
    if (sscanf(argument4,"%d",&factor) !=1 || factor <= 0)
    {
        fprintf(stderr,"please enter an positive interger as a factor\n");
        exit (1);
    }
    return factor;
}

// Expand the P2 file
int expand_P2_remainder(FILE* inputfile,int columns,int rows,int expand_columns,int expand_rows,int maxpixel,int factor,FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    unsigned char expand_remainder[expand_rows][expand_columns]; // 2-d array to store the remainder data of expand iamge
    int number_buffer; // Use this to test if each element is greater than the maxpixel
    
    // read
    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<columns;j++)
        {
            if (fscanf(inputfile,"%d", &number_buffer) !=1 || number_buffer > maxpixel) // read each element in the remainder, and each element should be integer and smaller than the maximum pixel
            {
                fprintf(stderr,"Corrupted input file\n");
                exit(1);
            }
            remainder[i][j] = (unsigned char) number_buffer; // convert number_buffer back to unsigned char
            // The reason of using unsigned char array is the bytes of the unsigned char is 1, and the bytes of the integer is 4, as a result, the size of the array will be smaller if we use the unsigned char array and when we read the big size of image it will be faster. In addition, we know that the maxpixel is 255, which is equal to the max value of the unsigned char(255).
        }
    }
    
    //expand
    for (int i=0;i<expand_rows;i=i+factor) // use factor as an increase unit (jump over each factor length in remainder)
    {
        for (int j=0;j<expand_columns;j=j+factor)
        {
            for (int k=0;k<factor;k++) // inside of each factor square
            {
                for (int l=0;l<factor;l++)
                {
                    expand_remainder[i+k][j+l] =remainder[i/factor][j/factor]; // all the other value in this square will be equal to the left top element
                }
            }
        }
    }
    
    // write
    for (int i=0;i<expand_rows;i++)
    {
        for (int j=0;j<expand_columns;j++)
        {
            
            fprintf(outputfile,"%hhu",expand_remainder[i][j]); // write the ASCII form remainder in the output file.
            if (j != expand_columns-1)
            {
                fputc(32,outputfile); // space
            }
        }
        fputc(10,outputfile); // new line
    }
    return 0;
}

// contract the p2 file
int contract_P2_remainder(FILE* inputfile,int columns,int rows,int contract_columns, int contract_rows,int maxpixel,int factor,FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    unsigned char contract_remainder[contract_rows][contract_columns]; // 2-d array to store the remainder data of contract image
    int number_buffer; // Use this to test if each element is greater than the maxpixel
    
    // read
    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<columns;j++)
        {
            if (fscanf(inputfile,"%d", &number_buffer) !=1 || number_buffer > maxpixel) // read each element in the remainder, and each element should be integer and smaller than the maximum pixel
            {
                fprintf(stderr,"Corrupted input file\n");
                exit(1);
            }
            remainder[i][j] = (unsigned char) number_buffer; // convert number_buffer back to unsigned char
            // The reason of using unsigned char array is the bytes of the unsigned char is 1, and the bytes of the integer is 4, as a result, the size of the array will be smaller if we use the unsigned char array and when we read the big size of image it will be faster. In addition, we know that the maxpixel is 255, which is equal to the max value of the unsigned char(255).
        }
    }
    
    //contract
    int total = 0;
    int average = 0;
    for (int i=0;i<contract_rows;i++) //In the contract case, we do not need to jump over the factor, since the original remainder array is the one which needs to be jumped.
    {
        for (int j=0;j<contract_columns;j++)
        {
            for (int k=0;k<factor;k++) // Treating this as a reverse case of the expand, jump over these factor*factor square in the orininal remainder.
            {
                for (int l=0;l<factor;l++)
                {
                    total = total + remainder[(i*factor)+k][(j*factor)+l]; // factor will become a coefficient and by adding k and l, the total pixcel value of each square can be calculated.
                }
            }
            average = (total) /(factor*factor); // find the average of the square
            contract_remainder[i][j] = average; // assign the average to the left top (shrinked) position of each square to the new array.
            total = 0;
            average = 0;
        }
    }
    
    // write
    for (int i=0;i<contract_rows;i++)
    {
        for (int j=0;j<contract_columns;j++)
        {
            fprintf(outputfile,"%hhu",contract_remainder[i][j]); // write the ASCII form remainder in the output file.
            if (j != contract_columns-1)
            {
                fputc(32,outputfile); // space
            }
        }
        fputc(10,outputfile); // new line
    }
    return 0;
}

// expand the p5 file
int expand_P5_remainder(FILE* inputfile,int columns,int rows,int expand_columns,int expand_rows,int factor,FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    unsigned char expand_remainder[expand_rows][expand_columns]; // 2-d array to store the remainder data of expand image
    
    // read
    if (rows*columns != fread(remainder,1,rows*columns,inputfile) ) // Read the total file, and the total bytes in the remainder should be equal to the rows*columns
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    if (fgetc(inputfile) != EOF) // used to check the corrupted file, fread has already read everthing, fgect should get EOF
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    
    /* Since the fread function read the specific size which is rows*columns, so if the total bytes is less than this, it can be detected by the fitst if statement. However, if the total bytes is larger than this value, it can not be detected, and we know that the fread function has already read the whole remainder, so if fgetc != EOF, which means that there are extra bytes.
     */
    
    //expand
    for (int i=0;i<expand_rows;i=i+factor) // use factor as an increase unit (jump over each factor length)
    {
        for (int j=0;j<expand_columns;j=j+factor)
        {
            for (int k=0;k<factor;k++) // inside of each factor square
            {
                for (int l=0;l<factor;l++)
                {
                    expand_remainder[i+k][j+l] =remainder[i/factor][j/factor]; // all the other value in this square will be equal to the left top element
                }
            }
        }
    }
    // write
    fwrite(expand_remainder,expand_columns*expand_rows,sizeof(unsigned char),outputfile);// Writhe the remainder in binary form in the outputfile

    return 0;
}

int contract_P5_remainder(FILE* inputfile,int columns,int rows,int contract_columns, int contract_rows,int factor,FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    unsigned char contract_remainder[contract_rows][contract_columns]; // 2-d array to store the remainder data of contract image
    
    // read
    if (rows*columns != fread(remainder,1,rows*columns,inputfile) ) // Read the total file, and the total bytes in the remainder should be equal to the rows*columns
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    if (fgetc(inputfile) != EOF) // used to check the corrupted file, fread has already read everthing, fgect should get EOF
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    
    /* Since the fread function read the specific size which is rows*columns, so if the total bytes is less than this, it can be detected by the fitst if statement. However, if the total bytes is larger than this value, it can not be detected, and we know that the fread function has already read the whole remainder, so if fgetc != EOF, which means that there are extra bytes.
     */
    
    //contract
    int total = 0;
    int average = 0;
    for (int i=0;i<contract_rows;i++) //In the contract case, we do not need to jump over the factor, since the original remainder array is the one which needs to be jumped.
    {
        for (int j=0;j<contract_columns;j++)
        {
            for (int k=0;k<factor;k++) // Treating this as a reverse case of the expand, jump over these factor*factor square in the orininal remainder.
            {
                for (int l=0;l<factor;l++)
                {
                    total = total + remainder[(i*factor)+k][(j*factor)+l]; // factor will become a coefficient and by adding k and l, the total pixcel value of each square can be calculated.
                }
            }
            average = (total) /(factor*factor); // find the average of the square
            contract_remainder[i][j] = average; // assign the average to the left top (shrinked) position of each square to the new array.
            total = 0;
            average = 0;
        }
    }
    
    // write
    fwrite(contract_remainder,contract_columns*contract_rows,sizeof(unsigned char),outputfile);// Writhe the remainder in binary form in the output file
    return 0;
}

// A combined function for expand to minimise the length of the mian function
void expand(char* inputfilename,char* outputfilename,char* argument4)
{
    int format = 0; // initialize P2 or P5
    int rotate = 0; // initialize the rotate off
    int expand = 1; // initialize the rotate on
    int contract = 0; // initialize the contract off
    int emphasizeblack = 0; // initialize the emphasize black off
    int emphasizewhite = 0; // initialize the emphasize white off
    int blur = 0;// initialize the blur off
    int gaussian_blur =0;// initialize the gaussian_blur off
    FILE* inputfile = fopen(inputfilename, "rb");//open; read binary
    FILE* outputfile = fopen(outputfilename,"w");
    int factor = factor_test(argument4); // obtain the factor value from the stdin
    file_test (inputfile,outputfile,inputfilename,outputfilename); // test whether the inputfile and outputfile exist
    image image_variable = read_write_four_lines(inputfile,format,outputfile,rotate,expand,factor,contract,emphasizeblack,emphasizewhite,blur,gaussian_blur); // read and write first four lines
    if (image_variable.defined_format == 2) // expand P2 file
    {
        expand_P2_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.expand_columns,image_variable.expand_rows,image_variable.maxpixel,factor,outputfile);
    }
    if (image_variable.defined_format == 5) // expand P5 file
    {
        expand_P5_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.expand_columns,image_variable.expand_rows,factor,outputfile);
    }
    fclose(inputfile); // close the input file
    fclose(outputfile); // close the out put file
}

// A combined function for contract to minimise the length of the mian function
void contract(char* inputfilename, char* outputfilename, char* argument4)
{
    int format = 0; // initialize P2 or P5
    int rotate = 0; // initialize the rotate off
    int expand = 0; // initialize the rotate off
    int contract = 1; // initialize the contract on
    int emphasizeblack = 0; // initialize the emphasize black off
    int emphasizewhite = 0; // initialize the emphasize white off
    int blur = 0;// initialize the blur off
    int gaussian_blur =0;// initialize the gaussian_blur off
    FILE* inputfile = fopen(inputfilename, "rb");//open; read binary
    FILE* outputfile = fopen(outputfilename,"w");
    int factor = factor_test(argument4); // obtain the factor value from the stdin
    file_test (inputfile,outputfile,inputfilename,outputfilename); // test whether the inputfile and outputfile exist
    image image_variable = read_write_four_lines(inputfile,format,outputfile,rotate,expand,factor,contract,emphasizeblack,emphasizewhite,blur,gaussian_blur); // read and write first four lines
    if (image_variable.defined_format == 2) // contract P2 file
    {
        contract_P2_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.contract_columns, image_variable.contract_rows,image_variable.maxpixel,factor,outputfile);
    }
    if (image_variable.defined_format == 5) // contract P5 file
    {
        contract_P5_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.contract_columns,image_variable.contract_rows,factor,outputfile);
    }
    fclose(inputfile); // close the input file
    fclose(outputfile); // close the output file
}

// emphasize the P2 file
int emphasize_black_white_P2_remainder(FILE* inputfile,int columns,int rows,int maxpixel,int factor,int emphasizeblack, int emphasizewhite, FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    int critical_value = (maxpixel/factor)+1; // depend on the factor
    int number_buffer; // Use this to test if each element is greater than the maxpixels
    // read
    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<columns;j++)
        {
            if (fscanf(inputfile,"%d", &number_buffer) !=1 || number_buffer > maxpixel) // read each element in the remainder, and each element should be integer and smaller than the maximum pixel
            {
                fprintf(stderr,"Corrupted input file\n");
                exit(1);
            }
            remainder[i][j] = (unsigned char) number_buffer; // convert number_buffer back to unsigned char
            // The reason of using unsigned char array is the bytes of the unsigned char is 1, and the bytes of the integer is 4, as a result, the size of the array will be smaller if we use the unsigned char array and when we read the big size of image it will be faster. In addition, we know that the maxpixel is 255, which is equal to the max value of the unsigned char(255).
        }
    }
    
    //black
    if (emphasizeblack == 1)
    {
        for (int i=0;i<rows;i++)
        {
            for (int j=0;j<columns;j++)
            {
                if (remainder[i][j] >= critical_value)
                {
                    remainder[i][j] = 0; // the value that greater than the critical value will be changed to black
                }
                else
                {
                    remainder[i][j] = maxpixel; // the value that smaller than the critical value will be change to white
                }
            }
        }
    }
    
    //white
    if (emphasizewhite == 1)
    {
        for (int i=0;i<rows;i++)
        {
            for (int j=0;j<columns;j++)
            {
                if (remainder[i][j] >= critical_value)
                {
                    remainder[i][j] = maxpixel; // the value that greater than the critical value will be changed to white
                }
                else
                {
                    remainder[i][j] = 0; // // the value that smaller than the critical value will be changed to black
                }
            }
        }
    }
    
    // write
    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<columns;j++)
        {
            fprintf(outputfile,"%hhu",remainder[i][j]); // write the ASCII form remainder in the output file.
            if (j != columns-1)
            {
                fputc(32,outputfile); // space
            }
        }
        fputc(10,outputfile); // new line
    }
    return 0;
}

// emphasize the p5 file
int emphasize_black_white_P5_remainder(FILE* inputfile,int columns,int rows,int maxpixel,int factor,int emphasizeblack, int emphasizewhite, FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    int critical_value = (maxpixel/factor)+1; // depend on the factor
    // read
    if (rows*columns != fread(remainder,1,rows*columns,inputfile) ) // Read the total file, and the total bytes in the remainder should be equal to the rows*columns
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    if (fgetc(inputfile) != EOF) // used to check the corrupted file, fread has already read everthing, fgect should get EOF
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    
    /* Since the fread function read the specific size which is rows*columns, so if the total bytes is less than this, it can be detected by the fitst if statement. However, if the total bytes is larger than this value, it can not be detected, and we know that the fread function has already read the whole remainder, so if fgetc != EOF, which means that there are extra bytes.
     */
    
    else
    {
        //black
        if (emphasizeblack == 1)
        {
            for (int i=0;i<rows;i++)
            {
                for (int j=0;j<columns;j++)
                {
                    if (remainder[i][j] >= critical_value)
                    {
                        remainder[i][j] = 0; // the value that greater than the critical value will be changed to black
                    }
                    else
                    {
                        remainder[i][j] = maxpixel; // the value that smaller than the critical value will be changed to white
                    }
                }
            }
        }
        
        //white
        if (emphasizewhite == 1)
        {
            for (int i=0;i<rows;i++)
            {
                for (int j=0;j<columns;j++)
                {
                    if (remainder[i][j] >= critical_value)
                    {
                        remainder[i][j] = maxpixel; // the value that greater than the critical value will be changed to white
                    }
                    else
                    {
                        remainder[i][j] = 0; // the value that smaller than the critical value will be changed to black
                    }
                }
            }
        }
    
        // write
        fwrite(remainder,rows*columns,sizeof(unsigned char),outputfile);// Writhe the remainder in binary form in the outputfile
    }
    return 0;
}

// A combined function for emphasize black to minimise the length of the mian function
void emphasize_black(char* inputfilename, char* outputfilename, char* argument4)
{
    int format = 0; // initialize P2 or P5
    int rotate = 0; // initialize the rotate off
    int expand = 0; // initialize the rotate off
    int contract = 0; // initialize the contract off
    int emphasizeblack = 1; // initialize the emphasize black on
    int emphasizewhite = 0; // initialize the emphasize white off
    int blur = 0;// initialize the blur off
    int gaussian_blur =0;// initialize the gaussian_blur off
    
    FILE* inputfile = fopen(inputfilename, "rb");//open; read binary
    FILE* outputfile = fopen(outputfilename,"w");

    int factor = factor_test(argument4); // obtain the factor
    if (factor <=1) // the factor should be greater than 1
    {
        fprintf(stderr,"please enter an integer which is greater than 1.\n");
        exit(1);
    }
    file_test (inputfile,outputfile,inputfilename,outputfilename); // test whether the inputfile and outputfile exist
    image image_variable = read_write_four_lines(inputfile,format,outputfile,rotate,expand,factor,contract,emphasizeblack,emphasizewhite,blur,gaussian_blur); // read and write first four lines
    if (image_variable.defined_format == 2) // emphasize_black P2 file
    {
        emphasize_black_white_P2_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.maxpixel,factor,emphasizeblack,emphasizewhite, outputfile);
    }
    if (image_variable.defined_format == 5) // emphasize_black P5 file
    {
        emphasize_black_white_P5_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.maxpixel,factor,emphasizeblack,emphasizewhite, outputfile);
    }
    fclose(inputfile); // close the input file
    fclose(outputfile); // close the output file
}

// A combined function for emphasize white to minimise the length of the mian function
void emphasize_white(char* inputfilename, char* outputfilename, char* argument4)
{
    int format = 0; // initialize P2 or P5
    int rotate = 0; // initialize the rotate off
    int expand = 0; // initialize the rotate off
    int contract = 0; // initialize the contract off
    int emphasizeblack = 0; // initialize the emphasize black off
    int emphasizewhite = 1; // initialize the emphasize white on
    int blur = 0;// initialize the blur off
    int gaussian_blur =0;// initialize the gaussian_blur off
    FILE* inputfile = fopen(inputfilename, "rb");//open; read binary
    FILE* outputfile = fopen(outputfilename,"w");
    int factor = factor_test(argument4); // obtain the factor
    if (factor <=1) // the factor should be greater thant 1
    {
        fprintf(stderr,"please enter an integer which is greater than 1.\n");
        exit(1);
    }
    file_test (inputfile,outputfile,inputfilename,outputfilename); // test whether the inputfile and outputfile are exist
    image image_variable = read_write_four_lines(inputfile,format,outputfile,rotate,expand,factor,contract,emphasizeblack,emphasizewhite,blur,gaussian_blur); // read and write first four lines
    if (image_variable.defined_format == 2) // emphasize_white P2 file
    {
        emphasize_black_white_P2_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.maxpixel,factor,emphasizeblack,emphasizewhite, outputfile);
    }
    if (image_variable.defined_format == 5) // emphasize_white P5 file
    {
        emphasize_black_white_P5_remainder(inputfile,image_variable.columns,image_variable.rows,image_variable.maxpixel,factor,emphasizeblack,emphasizewhite, outputfile);
    }
    fclose(inputfile); // close the input file
    fclose(outputfile); // close the output file
}

// average blur
// For P2 file
int blur_P2_remainder(FILE* inputfile,int columns, int rows, int maxpixel,int factor,FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    int number_buffer; // Use this to test if each element is greater than the maxpixels
    // read
    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<columns;j++)
        {
            if (fscanf(inputfile,"%d", &number_buffer) !=1 || number_buffer > maxpixel) // read each element in the remainder, and each element should be integer and smaller than the maximum pixel
            {
                fprintf(stderr,"Corrupted input file\n");
                exit(1);
            }
            remainder[i][j] = (unsigned char) number_buffer; // convert number_buffer back to unsigned char
            // The reason of using unsigned char array is the bytes of the unsigned char is 1, and the bytes of the integer is 4, as a result, the size of the array will be smaller if we use the unsigned char array and when we read the big size of image it will be faster. In addition, we know that the maxpixel is 255, which is equal to the max value of the unsigned char(255).
        }
    }
    
    int total = 0;
    int average = 0;
    int blur_columns = 0;
    int blur_rows = 0;
    
    if (columns%factor == 0)
    {
        blur_columns = columns;
    }
    else
    {
        blur_columns = columns-factor; 
    }
    if (rows%factor == 0)
    {
        blur_rows = rows;
    }
    else
    {
        blur_rows = rows - factor;
    }
    
    // There are tow reason for re-assiagn the columns and rows vlaue: 1. By subtracting the factor from columns and rows, which will be used in the for loop, this could aviod the "Segmentation fault: 11". Since in this case, all the element will be inside of the array. 2. if the file size is 512*512, and the blur factor is 512, it will just return the smae image, since the new blur_volumns or blur_rows will be equal to 0. So we can not simply just subtract the factor from the columns and rows.
    
    for (int i=0;i<blur_rows;i=i+factor) // jump over each blur square
    {
        for (int j=0;j<blur_columns;j=j+factor)
        {
            for (int k=0;k<factor;k++) // inside of each blur square
            {
                for (int l=0;l<factor;l++)
                {
                    total = total +remainder[i+k][j+l]; // find the total value of the blur sqaure
                }
            }
            average = (total) /(factor*factor); // calculate the average value of the blur square
            
            for (int k=0;k<factor;k++)
            {
                for (int l=0;l<factor;l++)
                {
                    remainder[i+k][j+l] = average; // assign this average value back to each element in this blur square
                }
            }
            total = 0;
            average = 0;
        }
    }
    
    // write
    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<columns;j++)
        {
            fprintf(outputfile,"%d",remainder[i][j]); // write the ASCII form remainder in the output file.
            if (j != columns-1)
            {
                fputc(32,outputfile); // space
            }
        }
        fputc(10,outputfile); // new line
    }
    return 0;
}

// For P5 file
int blur_P5_remainder(FILE* inputfile,int columns, int rows, int maxpixel,int factor,FILE* outputfile)
{
     unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    
    // read
    if (rows*columns != fread(remainder,1,rows*columns,inputfile) ) // Read the total file, and the total bytes in the remainder should be equal to the rows*columns
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    if (fgetc(inputfile) != EOF) // used to check the corrupted file, fread has already read everthing, fgect should get EOF
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    /* Since the fread function read the specific size which is rows*columns, so if the total bytes is less than this, it can be detected by the fitst if statement. However, if the total bytes is larger than this value, it can not be detected, and we know that the fread function has already read the whole remainder, so if fgetc != EOF, which means that there are extra bytes.
     */
    
    else
    {

        int total = 0;
        int average = 0;
        int blur_columns = 0;
        int blur_rows = 0;
        
        if (columns%factor == 0)
        {
            blur_columns = columns;
        }
        else
        {
            blur_columns = columns-factor;
        }
        if (rows%factor == 0)
        {
            blur_rows = rows;
        }
        else
        {
            blur_rows = rows - factor;
        }
        
        // There are tow reason for re-assiagn the columns and rows vlaue: 1. By subtracting the factor from columns and rows, which will be used in the for loop, this could aviod the "Segmentation fault: 11". Since in this case, all the element will be inside of the array. 2. if the file size is 512*512, and the blur factor is 512, it will just return the smae image, since the new blur_volumns or blur_rows will be equal to 0. So we can not simply just subtract the factor from the columns and rows.
        
        for (int i=0;i<blur_rows;i=i+factor) // jump over each blur square
        {
            for (int j=0;j<blur_columns;j=j+factor)
            {
                for (int k=0;k<factor;k++) // inside of each blur square
                {
                    for (int l=0;l<factor;l++)
                    {
                        total = total +remainder[i+k][j+l]; // find the total value of the blur sqaure
                    }
                }
                average = (total) /(factor*factor); // calculate the average value of the blur square
                
                for (int k=0;k<factor;k++)
                {
                    for (int l=0;l<factor;l++)
                    {
                        remainder[i+k][j+l] = average; // assign this average value back to each element in this blur square
                    }
                }
                total = 0;
                average = 0;
            }
        }
        
        // write
        fwrite(remainder,rows*columns,sizeof(unsigned char),outputfile);// Writhe the remainder in binary form in the outputfile
    }
    return 0;
}

// A combined function for blur to minimise the length of the mian function
void blur(char* inputfilename, char* outputfilename, char* argument4)
{
    int format = 0; // initialize P2 or P5
    int rotate = 0; // initialize the rotate off
    int expand = 0; // initialize the rotate off
    int contract = 0; // initialize the contract off
    int emphasizeblack = 0; // initialize the emphasize black off
    int emphasizewhite = 0; // initialize the emphasize white off
    int blur = 1;// initialize the blur on
    int gaussian_blur =0;// initialize the gaussian_blur off
    FILE* inputfile = fopen(inputfilename, "rb");//open; read binary
    FILE* outputfile = fopen(outputfilename,"w");
    int factor = factor_test(argument4); // obtain the factor value
    file_test (inputfile,outputfile,inputfilename,outputfilename); // test whether the inputfile and outputfile are exist
    image image_variable = read_write_four_lines(inputfile,format,outputfile,rotate,expand,factor,contract,emphasizeblack,emphasizewhite,blur,gaussian_blur); // read and write first four lines
    /*
    if (factor > image_variable.columns || factor> image_variable.rows)
    {
        fprintf(stderr,"The factor should be smaller than rows and columns.\n");
        exit(1);
    }
     */
    if (image_variable.defined_format == 2) // blur P2 file
    {
        blur_P2_remainder(inputfile,image_variable.columns,image_variable.rows, image_variable.maxpixel,factor,outputfile);
    }
    if (image_variable.defined_format == 5) // blur P5 file
    {
        blur_P5_remainder(inputfile,image_variable.columns,image_variable.rows, image_variable.maxpixel,factor,outputfile);
    }
    fclose(inputfile); // close the input file
    fclose(outputfile); // close the output file
}

// gaussian blur
// obtain the parameter fomr stdin which will be used in normal distribution
float parameter_test(char* argument5) // since we want an float, so we use a float function to retunr a float.
{
    float parameter;
    if (sscanf(argument5,"%f",&parameter) != 1 || parameter < 0)
    {
        fprintf(stderr,"please enter an positive number as a parameter\n");
        exit (1);
    }
    return parameter;
}

/*
 The general idea of gaussian blur is obtaining an better resolution of the image compared to the average blur. In the average blur, we just take the average value of each blur square. However, in the Gaussian blur, we want each element's value is close to the elements surrounding it, and the relationship between these elements follows the normal distribution. For example, if the factor which is obtained from the stdin is 3, we will have a 3*3 square, and we will find another value for the centre element in this square by using the normal distribution. If elements in this square are near to the center, the weight of them will be higher than the elements which are further to the center. The reason is simple, the near the distance, the higher the influence.  As a result, if we assign the coordinate of the centre as (0,0), the corrdinates of the left top to left right will be (-1,1),  (0,1), and ( 1,1) etc. So by using the 2-d normal distribution equation, we can calculate the weight of each coordinate. However, if we just use this value, our Gaussian blur square is not balanced, since the total weight of this square is not 1. So we need to add the total weight of each coordinate and then each coordinate’s weight should be divided by this total weight. In this case, it can be tested that the total weight of our new gaussian blur square is 1, which is balanced.

 After this, each coordinate’s weight needs to multiply to its initial pixel value, and then by adding these value together, this total sum value is the new value for the centre point in the Gaussian blur square [1] [2].
 */

// Calculate the weight of each element in the square
int gaussian_weight(int factor, float parameter, unsigned char data[][factor])
{
    // The reason of using float instead of double is although double is more precise, the byte of the float is 4 which is half of the double, since the calculation in this step will be huge, so by using float could left more space in the memory and lead to speed up this process
    int total_value = 0; // The fianl value of the center point
    float total_weight = 0; // By achieving the balanced Gaussian blur sqaure.
    float weight_array [factor][factor]; // Gaussian blur square
    float two_Pi = 2*(M_PI);
    int radius = (factor/2);
    
    for (int i=0;i<factor;i++)
    {
        for (int j=0;j<factor;j++)
        {
            weight_array[i][j] = ((1/(two_Pi*parameter*parameter))*exp(-((j-radius)*(j-radius)+(radius-i)*(radius-i))/(2*parameter*parameter))); // 2-d normal distribution
            total_weight = total_weight + weight_array[i][j]; // calculate the total weight of the square
        }
    }
    for (int i=0;i<factor;i++)
    {
        for (int j=0;j<factor;j++)
        {
            weight_array[i][j] = (weight_array[i][j])/total_weight; // After dividing the total weight of the square, the new total weight of this square wiil be equal to 1 which is balanced
            total_value = total_value + weight_array[i][j] * data[i][j]; // Since it is balanced now, the new value for each element in the square is its weight multiply the original pixcel value.
        }
    }
    
    return total_value; // Adding these value together, the total value is the new vlaue of the center point which is returned.
}

// For P2 file
int gaussian_blur_P2_remainder(FILE* inputfile,int columns, int rows, int maxpixel,int factor,float parameter,FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    unsigned char new_remainder[rows][columns]; // 2-d array to store the remaidner for gaussian blur image
    unsigned char data[factor][factor]; // 2-d array to pass the gaussian blur square to "gaussian_weight" function
    
    int number_buffer; // Use this to test if each element is greater than the maxpixels
    // read
    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<columns;j++)
        {
            if (fscanf(inputfile,"%d", &number_buffer) !=1 || number_buffer > maxpixel) // read each element in the remainder, and each element should be integer and smaller than the maximum pixel
            {
                fprintf(stderr,"Corrupted input file\n");
                exit(1);
            }
            remainder[i][j] = (unsigned char) number_buffer; // convert number_buffer back to unsigned char
            // The reason of using unsigned char array is the bytes of the unsigned char is 1, and the bytes of the integer is 4, as a result, the size of the array will be smaller if we use the unsigned char array and when we read the big size of image it will be faster. In addition, we know that the maxpixel is 255, which is equal to the max value of the unsigned char(255).
        }
    }
    
    // gaussian blur
    int radius = factor/2;
    
    for (int i=radius;i<rows-factor;i++) // Directly start from the first left top center point.
    {
        for (int j=radius;j<columns-factor;j++)
        {
            for (int k=0;k<factor;k++)
            {
                for (int l=0;l<factor;l++)
                {
                    data[k][l] = remainder[i+k][j+l]; // fill the gaussian blur square
                }
            }
            new_remainder[i][j] = gaussian_weight(factor,parameter,data); // assign the returned total value of this center point.
        }
    }
    
    // write
    for (int i=0;i<rows;i++)
    {
        for (int j=0;j<columns;j++)
        {
            fprintf(outputfile,"%d",new_remainder[i][j]); // write the ASCII form remainder in the output file.
            if (j != columns-1)
            {
                fputc(32,outputfile); // space
            }
        }
        fputc(10,outputfile); // new line
    }
    return 0;
}



// For P5 file
int gaussian_blur_P5_remainder(FILE* inputfile,int columns, int rows, int maxpixel,int factor,float parameter,FILE* outputfile)
{
    unsigned char remainder[rows][columns]; // 2-d array to store the remainder data
    unsigned char new_remainder[rows][columns]; // 2-d array to store the remaidner for gaussian blur image
    unsigned char data[factor][factor]; // 2-d array to pass the gaussian blur square to "gaussian_weight" function
    
    // read
    if (rows*columns != fread(remainder,1,rows*columns,inputfile) ) // Read the total file, and the total bytes in the remainder should be equal to the rows*columns
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    if (fgetc(inputfile) != EOF) // used to check the corrupted file, fread has already read everthing, fgect should get EOF
    {
        fprintf(stderr,"Corrupted input file\n");
        exit(1);
    }
    /* Since the fread function read the specific size which is rows*columns, so if the total bytes is less than this, it can be detected by the fitst if statement. However, if the total bytes is larger than this value, it can not be detected, and we know that the fread function has already read the whole remainder, so if fgetc != EOF, which means that there are extra bytes.
     */
 
    else
    {

        // gaussian blur
        int radius = factor/2;
        
        for (int i=radius;i<rows-factor;i++) // Directly start from the first left top center point.
        {
            for (int j=radius;j<columns-factor;j++)
            {
                for (int k=0;k<factor;k++)
                {
                    for (int l=0;l<factor;l++)
                    {
                        data[k][l] = remainder[i+k][j+l]; // fill the gaussian blur square
                    }
                }
                new_remainder[i][j] = gaussian_weight(factor,parameter,data); // assign the returned total value of this center point.
            }
        }
        // write
        fwrite(new_remainder,rows*columns,sizeof(unsigned char),outputfile);// Writhe the remainder in binary form in the outputfile
        

    }
    return 0;
}


// A combined function for gaussian blur to minimise the length of the mian function
void gaussian_blur(char* inputfilename, char* outputfilename, char* argument4, char* argument5)
{
    int format = 0; // initialize P2 or P5
    int rotate = 0; // initialize the rotate off
    int expand = 0; // initialize the rotate off
    int contract = 0; // initialize the contract off
    int emphasizeblack = 0; // initialize the emphasize black off
    int emphasizewhite = 0; // initialize the emphasize white off
    int blur = 0;// initialize the blur off
    int gaussian_blur = 1; // initialize the gaussian blur on
    FILE* inputfile = fopen(inputfilename, "rb");//open; read binary
    FILE* outputfile = fopen(outputfilename,"w");
    int factor = factor_test(argument4); // obtain the factor from stdin
    if (factor %2 == 0 ) // the length of the gaussian blur square must be an odd number, due to the center point.
    {
        fprintf(stderr,"Please enter an odd integer.\n");
        exit (1);
    }
    float parameter = parameter_test(argument5); //obtain the parameter from stdin, this parameter is a constant which is used in the normal distribution
    file_test (inputfile,outputfile,inputfilename,outputfilename); // test whether the inputfile and outputfile exist
    
    image image_variable = read_write_four_lines(inputfile,format,outputfile,rotate,expand,factor,contract,emphasizeblack,emphasizewhite,blur,gaussian_blur); // read and write first four lines
    
    if (image_variable.defined_format == 2) // gaussian_blur P2 file
    {
        gaussian_blur_P2_remainder(inputfile,image_variable.columns, image_variable.rows, image_variable.maxpixel,factor,parameter,outputfile);
    }
    if (image_variable.defined_format == 5) // gaussian_blur P5 file
    {
        gaussian_blur_P5_remainder(inputfile,image_variable.columns, image_variable.rows, image_variable.maxpixel,factor,parameter,outputfile);
    }
    fclose(inputfile); // close the input file
    fclose(outputfile); // close the output file

}
/*
Reference:

[1] Collins,R. Lecture 4: Smoothing. Available at: http://www.cse.psu.edu/~rtc12/CSE486/lecture04.pdf [Accessed Mar 20, 2018].

[2] Salazar,R. (2017). HOW TO PROGRAM A GAUSSIAN BLUR WITHOUT USING 3RD PARTY LIBRARIES. S0hungry. Available at:http://s0hungry.com/2017/08/29/how-to-program-a-gaussian-blur-without-using-3rd-party-libraries/ [Accessed Mar 21, 2018].
*/
