#define GLEW_STATIC
#ifndef ShaderControl
#define ShaderControl

#include "../mainHeaders.h"
#include "shader.h"

using namespace std;
//------------------------------------------
//   	Shader Controller Class
//------------------------------------------
class shaderController
{
	public:
	size_t Nshdr;

	vector<Shader> ShdrLib; //Library of shaders
	vector<string> ShdrFiles; //Shader files
	vector<int> DetermLoc; //Shader files
	vector<bool> ActvShdrs; //Determine if a shader is active

	shaderController() {};

	~shaderController() {ShdrLib.clear();ShdrFiles.clear();DetermLoc.clear();ActvShdrs.clear();};

	void ShaderLoader()//In charge of loading and registering all data from shader.lib file.
	{
	        int lines;
	        vector<string> DATA;
	        readExtern(DATA,lines,"bin/shaders/","shader.lib");

	        for (int i = 0; i < lines; ++i)
	        {
	                ShdrFiles.push_back(DATA[i]);
	                ActvShdrs.push_back(false);
	                DetermLoc.push_back(-1);
	                //cout << "Shaders Avaliable (" << i << "): " << DATA[i] << endl;
			int SID = SetShaderOnDevice(i);
			SID = SID+1;
	        }
		Nshdr = ShdrLib.size();
	};

	int SetShaderOnDevice (int setshader)
	{
		int shdrID;
		if (!ActvShdrs[setshader])
		{
			ShdrLib.push_back(ShdrFiles[setshader]);
			shdrID = (int)ShdrLib.size() - 1;
			DetermLoc[setshader] = shdrID;
			ActvShdrs[setshader] = true;
			cout << "Setting Shader (" << setshader << "): " << ShdrFiles[setshader] << " to location ID " << shdrID << endl;
		} else { 
			shdrID = DetermLoc[setshader];
			cout << "Using Shader ID: " << shdrID << endl;
		}

		return shdrID;
	};
};
#endif
