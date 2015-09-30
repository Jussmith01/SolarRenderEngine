/*******************************************************************
************File Designed to Read in XYZ Trajectory File************
********************************************************************
DESIGNED BY: JUSTIN S. SMITH
CONTACT E-MAIL: JSMITH48@CHEM.UFL.EDU

SAMPLE INPUT FILE:

-----------------STARTS BELOW THIS LINE---------------------
Executable:Name_Of_Executable
$c++:
        compiler:
                g++
        flags:
                -std=c++11
        dir:
                utils_cpp/
        files:
                lib_timer_class
                lib_classes
                lib_ipt_file
                lib_opt_class
        dir:
                none
        files:
                main_MD
        DONE
$ENDc++
$cuda:
        compiler:
                nvcc
        flags:
                -arch=compute_30
        dir:
                cuda_main/cuda_classes/
        files:
                cuda_MemHandler_class
        DONE
$ENDcuda
$linkerflags:
        compiler:
                nvcc
        flags:
                -arch=compute_30
        DONE
$ENDlinkerflags
$END OF FILE$
-------------------ENDS ABOVE THIS LINE-----------------------
*********************************************************************/

//**C/C++ included libraries**
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>

//**Included custom headers**

using namespace std;

//***************************************************************
//***********Obtain line number of file line delimiter***********
//***************************************************************
unsigned int get_line_num (string search_string, string *INPUT_DATA, int NUM_LINES)
{
unsigned int LINE_NUM=0,i=0;

while (i < NUM_LINES)
{

	//cout << "First String: \"" << search_string << "\" Second String: \"" << INPUT_DATA[i] << "\" COMPNUM: " << INPUT_DATA[i].compare(search_string) << "\n";
	if (INPUT_DATA[i].compare(search_string) == 0)
	{LINE_NUM = i;break;}
	i++;
}
return LINE_NUM;
}

//Trims the white space from the front and end of a string
string trim (string line)
{
string trimmed_line;
unsigned int first = line.find_first_not_of(" \t\r\n\x0b");
unsigned int last = line.find_last_not_of(" \t\r\n\x0b") + 1;	

if (line.length() != 0)
{
	trimmed_line = line.substr(first,last);
}
return trimmed_line;
}


//*******************************************************************
//**********************Read the input file**************************
//*******************************************************************
extern void Compile (char *inputfile)
{

//ipt_data *DATA;
string line;
ifstream iptfile (inputfile);
int i=0,NUM_LINES=0;

//COUNTS NUMBER OF LINES IN THE INPUT FILE FOR DYNAMIC MEMORY ALLOCATION
if (iptfile.is_open())
	{
		while (!iptfile.eof())
		{
			getline(iptfile,line);
			if (line == "$END OF FILE$") {break;}
			NUM_LINES++;
		}
iptfile.close();

} else {cout << "***error -- unable to open compiler data file***\n";exit(1);}

//ALLOCATION OF DATA STRING ARRAY
string *DATA = NULL;
DATA = new string [NUM_LINES];

//ALLOCATION OF PROGRAM FILENAMES STRING ARRAY
string *progfilename = NULL;
progfilename = new string [NUM_LINES];

//READ INPUT LINES INTO STRING ARRAY: DATA
ifstream ipt2file (inputfile);
if (ipt2file.is_open())
{
	while (!ipt2file.eof())
	{
		getline(ipt2file,line);
		if (line != "$END OF FILE$")
		{
			DATA[i] = trim(line);
		} else {break;}
		i++;
	}
ipt2file.close();

} else {cout << "***error -- unable to open compiler data file***\n";exit(1);}
//PARSE DATA FUNCTIONS FOLLOW

//Required Declarations
unsigned int LINE_BEG, LINE_END,char_search=0,filecount = 0,finchecker = 0,k = 0;
string test_string_beg,test_string_end,command,error_string;
int err_chk = 0;

//Delimiters
string flags = "flags:";
string postflags = "postflags:";
string dir = "dir:";
string files = "files:";
string done = "DONE";
string compiler = "compiler:";
string none = "none";

//Working strings
string workingDir;
string workingFlags;
string workingpostFlags;
string workingCompiler;

//Required Strings
string space = " ";
string back = "../";
string linkoptdir;
string linkiptdir;
string c = "-c";
string o = "-o";
string optDir = "compiler_output/";
string mkdir = "mkdir";
string rmdir = "rm -R";
string executable;

//Extensions
string opt = ".o";
string cpp = ".cpp";
string cu = ".cu";

cout << "Begin Reading: " << inputfile << "...\n";

int delimpos = DATA[0].find_first_of(":") + 1;
executable = DATA[0].substr(delimpos);
cout << "Saving executable as: " << executable.c_str() << "\n";

command = mkdir + space + optDir;
system(command.c_str());

i = 0;
while (i <= 2)
{
	switch (i)
	{
		case 0: {test_string_beg = "$c++:";test_string_end = "$ENDc++";break;}
		case 1: {test_string_beg = "$cuda:";test_string_end = "$ENDcuda";break;}
		case 2: {test_string_beg = "$linkerflags:";test_string_end = "$ENDlinkerflags";break;}
	}

	LINE_BEG = get_line_num(test_string_beg,DATA,NUM_LINES);
	LINE_END = get_line_num(test_string_end,DATA,NUM_LINES);


	switch (i)
	{
		case 0:
		{
			for (int j = LINE_BEG + 1; j < LINE_END; ++j)
			{
				if (DATA[j].compare(compiler) == 0)
				{
					workingCompiler = DATA[j + 1];
                                        if (none.compare(trim(workingCompiler)) == 0) {break;}
					cout << "Using Compiler: " << workingCompiler << " ...\n";
				}
				if (DATA[j].compare(flags) == 0)
				{ 
					workingFlags = DATA[j + 1];
				} 
				else if (dir.compare(trim(DATA[j])) == 0)
				{
					if (none.compare(trim(DATA[j + 1])) == 0)
					{
						workingDir = "";
					} else {
						workingDir = DATA[j + 1];
					}
				} 
				else if (files.compare(trim(DATA[j])) == 0)
				{
					k = 1;
					while (finchecker  == 0)
					{
						if (DATA[j + k].compare(done) == 0 || dir.compare(trim(DATA[j + k])) == 0)
						{
							break;
						}
						progfilename[filecount] = DATA[j + k];
						command  = workingCompiler + space + workingFlags + space + c + space + trim(workingDir) + DATA[j + k] + cpp + space + o + space + optDir + DATA[j + k] + opt; 
						
						cout << "Executing Command: " << command.c_str() << " ...\n";	
						//cout << "Compiling...\n";
						system(command.c_str());
						++k;
						++filecount;
					}
				}
                        }
                	break;
		}
                case 1:
                {
                	for (int j = LINE_BEG + 1; j < LINE_END; ++j)
                        {
                        	if (DATA[j].compare(compiler) == 0)
                                {
                                	workingCompiler = DATA[j + 1];
					if (none.compare(trim(workingCompiler)) == 0) {break;}
                                        cout << "Using Compiler: " << workingCompiler << " ...\n";
                                }
                                if (DATA[j].compare(flags) == 0)
                                {
                                	workingFlags = DATA[j + 1];
                                }
                                else if (dir.compare(trim(DATA[j])) == 0)
                                {
                                	if (none.compare(trim(DATA[j + 1])) == 0)
                                        {
                                        	workingDir = "";
                                        } else {
                                        	workingDir = DATA[j + 1];
                                        }
                                } 
                                else if (DATA[j].compare(files) == 0)
                                {
                                	k = 1;
                                	while (finchecker  == 0)
                                        {
                                        	if (DATA[j + k].compare(done) == 0 || dir.compare(trim(DATA[j + k])) == 0)
                                                {
                                                	break;
                                                }
                                                progfilename[filecount] = DATA[j + k];
                                                command  = workingCompiler + space + workingFlags + space + c + space + trim(workingDir) + trim(DATA[j + k]) + cu + space + o + space + optDir + DATA[j + k] + opt; 

                                                cout << "Executing Command: " << command.c_str() << " ...\n"; 
                                                //cout << "Compiling...\n";
                                                system(command.c_str());
                                                ++k;
                                                ++filecount;
                                        }
                                }
                        }

			break;
		}
                case 2:
                {
			for (int j = LINE_BEG + 1; j < LINE_END; ++j)
                	{
                        	if (DATA[j].compare(compiler) == 0)
                                {
                                	workingCompiler = DATA[j + 1];
                                }
                                if (DATA[j].compare(flags) == 0)
                                {
                                	workingFlags = DATA[j + 1];
                                }
                                if (DATA[j].compare(postflags) == 0)
                                {
                                        workingpostFlags = DATA[j + 1];
                                }
			}
			command = workingCompiler + space + workingFlags + space;
			for (int i = 0; i < filecount; ++i)
			{
				command = command + optDir + progfilename[i] + opt + space;
                        }
			command  = command + o + space + back + executable + space + workingpostFlags;
			cout << "Executing Linking Command: " << command.c_str() << " ...\n";
	               	system(command.c_str());
                       	//cout << "Linking Complete.\n";
			break;
                }
	}
        i++;
}
	
command = rmdir + space + optDir;
system(command.c_str());

        //***********CLEAR FUNCTION MEMORY ALLOCATIONS***********

	//cout << "Deleting Temporary Data...\n";
        delete [] DATA;
        DATA = NULL;

        delete [] progfilename;
        progfilename = NULL;
	
	//cout << "Temporary Data Deleted.\n";

	if (err_chk != 0)
	{
		cout << "Errors Detected: " << err_chk << "\n";
		cout << "Compilation Failed.\n";
		exit(1);
	}
}

//*******************************************************************
//**********************Main Program Launcher************************
//*******************************************************************
int main (int argc, char *argv[])
{
        char *inputfile = argv[1];

	cout << "Compiling Started...\n";

        //DECLARE INPUT ARRAYS - 
        Compile(inputfile);

	cout << "Compiling Complete.\n";

        return 0;
}

