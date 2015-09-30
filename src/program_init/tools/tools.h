#define GLEW_STATIC
#ifndef ToolsFuncs
#define ToolsFuncs

#include "../mainHeaders.h"
//#include "../basicStructures/gameStructs.h"
//#include "../objectControllers/objectController.h"

using namespace std;
//*****************************************//
//		Tools Functions		   //
//*****************************************//
bool readExtern(vector<string> &data,int &lines,string dir,string filename);
string searchDelim(vector<string> &data,int linestot,int linestart, int &lineend, string searchstring);
int searchLine(vector<string> &data,int linestot,int linestart, string searchline);
bool parseLine(string command,string &func,string &param);
string GetLetter(int key,bool shift,bool caps);
string nts(float N);
string its(int N);

#endif
