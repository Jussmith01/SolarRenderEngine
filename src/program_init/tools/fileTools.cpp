#define GLEW_STATIC

//#include "../mainHeaders.h"
//#include "objectController.h"
#include "tools.h"

using namespace std;

//------------------------------------------
//  	     Read External Files
//------------------------------------------
bool readExtern(vector<string> &data,int &lines,string dir,string filename)
{
         lines = 0; bool errorchk;
         string line;
         stringstream fd;
         fd << dir << filename;
         ifstream file (fd.str().c_str());
         if (file.is_open())
         {
                 while ( getline (file,line) )
                         {
				if (line.find("#") != 0)
				{
					data.push_back(line);
					++lines;
				}
			 }
                 file.close();
		errorchk = true;
         } else {errorchk=false;cout << "Extern Loader Error: Unable to open file: " << fd.str().c_str() << "\n";}
	return errorchk;
};

//------------------------------------------
//   Search for a given Delim and value
//------------------------------------------
string searchDelim(vector<string> &data,int linestot,int linestart, int &lineend, string searchstring)
{
	string delim = "=";
	string rtnstr;
	for (int i = linestart; i < linestot; ++i)
	{
		int pos = 1;
		pos = data[i].find("=");
		if (data[i].substr(0,pos).compare(searchstring) == 0)
		{
			rtnstr = data[i].substr(pos+1);
			//cout << "rtnstr: " << rtnstr << endl;
			lineend = i;
			break;
		}
	}
	return rtnstr;
};

//------------------------------------------
//   		  Find Line
//------------------------------------------
int searchLine(vector<string> &data,int linestot,int linestart, string searchline)
{
	int rtnpos = 0;
        for (int i = linestart; i < linestot; ++i)
        {
                if (data[i].compare(searchline) == 0)
                	{rtnpos = i;break;}
        }
        return rtnpos;
};

//------------------------------------------
//            Float to String
//------------------------------------------
string nts(float N)
{
	stringstream ss;
	ss << N;
	return ss.str();
};

//------------------------------------------
//            Int to String
//------------------------------------------
string its(int N)
{
        stringstream ss;
        ss << N;
        return ss.str();
};

//------------------------------------------
//      Parse A String by an = sign
//------------------------------------------
bool parseLine(string command,string &func,string &param)
{
        bool errchk = true;
        size_t pos = command.find(":");
        //cout << "POS: " << pos << endl;
        if (pos != string::npos)
        {
                func = command.substr(0,pos);
                param = command.substr(pos+1);
                errchk = false;
        } else {
		func = command;
		param = "NONE";
		errchk = false;
	}
        return errchk;
};

//------------------------------------------
//         Get Letter from Int
//------------------------------------------
string GetLetter(int key,bool shift,bool caps)
{
	string lett;

	if (!shift) //If true, shift off
	{

	switch(key)
	{
		//Various Keys
		case 32: {lett = " ";break;}//space
		case 39: {lett = "'";break;}
		case 44: {lett = ",";break;}
		case 45: {lett = "-";break;}
		case 46: {lett = ".";break;}
		case 47: {lett = "/";break;}

		case 59: {lett = ";";break;}
		case 61: {lett = "=";break;}

		case 91: {lett = "[";break;}
		case 92: {lett = "\\";break;}
		case 93: {lett = "]";break;}
		case 96: {lett = "`";break;}

		//Numbers
		case 48: {lett = "0";break;}
		case 49: {lett = "1";break;}
		case 50: {lett = "2";break;}
		case 51: {lett = "3";break;}
		case 52: {lett = "4";break;}
		case 53: {lett = "5";break;}
		case 54: {lett = "6";break;}
		case 55: {lett = "7";break;}
		case 56: {lett = "8";break;}
		case 57: {lett = "9";break;}
	}
	} else { //Shift on

        switch(key)
        {
                //Various Keys
                case 32: {lett = " ";break;}//space
                case 39: {lett = "\"";break;}
                case 44: {lett = "<";break;}
                case 45: {lett = "_";break;}
                case 46: {lett = ">";break;}
                case 47: {lett = "?";break;}

                case 59: {lett = ":";break;}
                case 61: {lett = "+";break;}

                case 91: {lett = "{";break;}
                case 92: {lett = "|";break;}
                case 93: {lett = "}";break;}
                case 96: {lett = "~";break;}

                //Numbers
                case 48: {lett = ")";break;}
                case 49: {lett = "!";break;}
                case 50: {lett = "@";break;}
                case 51: {lett = "#";break;}
                case 52: {lett = "$";break;}
                case 53: {lett = "%";break;}
                case 54: {lett = "^";break;}
                case 55: {lett = "&";break;}
                case 56: {lett = "*";break;}
                case 57: {lett = "(";break;}
        }
	}

	switch(key)
	{
                //Letters
                case 65: {if ((shift || caps) && !(shift && caps)){lett = "A";}else{lett="a";};break;}
                case 66: {if ((shift || caps) && !(shift && caps)){lett = "B";}else{lett="b";};break;}
                case 67: {if ((shift || caps) && !(shift && caps)){lett = "C";}else{lett="c";};break;}
                case 68: {if ((shift || caps) && !(shift && caps)){lett = "D";}else{lett="d";};break;}
                case 69: {if ((shift || caps) && !(shift && caps)){lett = "E";}else{lett="e";};break;}
                case 70: {if ((shift || caps) && !(shift && caps)){lett = "F";}else{lett="f";};break;}
                case 71: {if ((shift || caps) && !(shift && caps)){lett = "G";}else{lett="g";};break;}
                case 72: {if ((shift || caps) && !(shift && caps)){lett = "H";}else{lett="h";};break;}
                case 73: {if ((shift || caps) && !(shift && caps)){lett = "I";}else{lett="i";};break;}
                case 74: {if ((shift || caps) && !(shift && caps)){lett = "J";}else{lett="j";};break;}
                case 75: {if ((shift || caps) && !(shift && caps)){lett = "K";}else{lett="k";};break;}
                case 76: {if ((shift || caps) && !(shift && caps)){lett = "L";}else{lett="l";};break;}
                case 77: {if ((shift || caps) && !(shift && caps)){lett = "M";}else{lett="m";};break;}
                case 78: {if ((shift || caps) && !(shift && caps)){lett = "N";}else{lett="n";};break;}
                case 79: {if ((shift || caps) && !(shift && caps)){lett = "O";}else{lett="o";};break;}
                case 80: {if ((shift || caps) && !(shift && caps)){lett = "P";}else{lett="p";};break;}
                case 81: {if ((shift || caps) && !(shift && caps)){lett = "Q";}else{lett="q";};break;}
                case 82: {if ((shift || caps) && !(shift && caps)){lett = "R";}else{lett="r";};break;}
                case 83: {if ((shift || caps) && !(shift && caps)){lett = "S";}else{lett="s";};break;}
                case 84: {if ((shift || caps) && !(shift && caps)){lett = "T";}else{lett="t";};break;}
                case 85: {if ((shift || caps) && !(shift && caps)){lett = "U";}else{lett="u";};break;}
                case 86: {if ((shift || caps) && !(shift && caps)){lett = "V";}else{lett="v";};break;}
                case 87: {if ((shift || caps) && !(shift && caps)){lett = "W";}else{lett="w";};break;}
                case 88: {if ((shift || caps) && !(shift && caps)){lett = "X";}else{lett="x";};break;}
                case 89: {if ((shift || caps) && !(shift && caps)){lett = "Y";}else{lett="y";};break;}
                case 90: {if ((shift || caps) && !(shift && caps)){lett = "Z";}else{lett="z";};break;}
	}
                

        return lett;
};

