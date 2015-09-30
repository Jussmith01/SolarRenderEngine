#define GLEW_STATIC

//#include "../mainHeaders.h"
//#include "../EngineHeader.h"
#include "gameStructs.h"

using namespace std;

//-------------------------------------------//
//        Properties Class Functions         //
//-------------------------------------------//
void Properties::read_gcf()
{
	cout << "Reading Properties..." << endl;
        lines = 0;
        string line;
        ifstream file (filename.c_str());
        if (file.is_open())
        {
                while ( getline (file,line) )
                        {data.push_back(line);++lines;}
                file.close();
        } else cout << "Error: Unable to open file: " << filename.c_str() << "\n";
	this->lines = lines;
};

void Properties::set_properties()
{
	for (int i = 0; i < lines; ++i)
	{
		int pos = data[i].find_first_of("=");
		string vname = data[i].substr(0,pos);

		string cs = "reswidth";
		if (vname.compare(cs) == 0)
		{
			string val = data[i].substr(pos+1);
			WinWidth = atoi(val.c_str());
		}

                cs = "resauto";
                if (vname.compare(cs) == 0)
                {
                        string val = data[i].substr(pos+1);
                        ResAuto = atoi(val.c_str());
                }

		cs = "resheight";
                if (vname.compare(cs) == 0)
                {
                        string val = data[i].substr(pos+1);
			WinHeight = atoi(val.c_str());
                }

		cs = "fullscreen";
                if (vname.compare(cs) == 0)
                {
                        string val = data[i].substr(pos+1);
			FullScreen = atoi(val.c_str());
                }

		cs = "msaa";
                if (vname.compare(cs) == 0)
                {
                        string val = data[i].substr(pos+1);
			MSAA = atoi(val.c_str());
                }
	}

	GetPrimaryResolution(ResAuto);
	GetPrimaryMonitorSize();

	//Calculate DPmm
	DPmm = (WinWidth * WinHeight)/(MonWidth * MonHeight);
	FontSize = 0.00961538f * (float)DPmm + 0.22f;
	LineSpace = 0.48f * (float)DPmm + 15.0f;

	//cout << "Fullscreen Mode: " << FullScreen << endl;
	//cout << "Auto Resolution: " << ResAuto << endl;
	//cout << "Screen Resolution: " << WinWidth << "X" << WinHeight << endl;
	cout << "MSAA Sampling: " << MSAA << " DPmm: " << DPmm << " pixels/mm" << endl;
	data.clear();
};

void Properties::GetPrimaryResolution(int ResAuto)
{
	if (ResAuto == 1)
	{
		GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
		if (Monitor)
		{
			const GLFWvidmode* Mode = glfwGetVideoMode(Monitor);
			cout << "Detected Video Mode: " << Mode->width << "x" << Mode->height << endl;
			WinWidth = Mode->width;
			WinHeight = Mode->height;
		} else {cout << "Error Obtaining Primary Monitor.\n";};
	}
};

void Properties::GetPrimaryMonitorSize()
{
        GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
        if (Monitor)
        {
                glfwGetMonitorPhysicalSize(Monitor,&MonWidth,&MonHeight);
                cout << "Detected Monitor Size: " << MonWidth << "x" << MonHeight << endl;
	} else {cout << "Error Obtaining Primary Monitor.\n";};
};

//-------------------------------------------//
//            Input Class Functions          //
//-------------------------------------------//

