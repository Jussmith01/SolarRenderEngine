#define GLEW_STATIC

//#include "../mainHeaders.h"
#include "objectController.h"

using namespace std;

//------------------------------------------
//            Model Register
//------------------------------------------
int objPropController::RegisterModel (string objfile)
{
	//If beginning the vector, put new model, else search for existing
	int rtnID;
	if (NMod == 0)
	{
                        ModLib.push_back(objfile);
                        rtnID = NMod;
			#pragma omp critical
			{++NMod;}
	} else {
		bool srhval = false;
		for (int i = 0; i < NMod;++i)
		{
			//cout << "[" << objfile << "] =? [" << ModLib[i].objfile << "]\n";
			if (objfile.compare(ModLib[i].objfile) == 0)
			{
				rtnID = i;
				srhval = false;
				break;
			} else {srhval = true;}
		}

		//If Model isnt loaded, create a new one.
		if (srhval)
		{
			ModLib.push_back(objfile);
			rtnID = NMod;
			#pragma omp critical
			{++NMod;}
		}
	}
	//cout << "MODEL ID: " << rtnID << endl;
	return rtnID;
};

//------------------------------------------
//            Model Register
//------------------------------------------
int objPropController::RegisterTexture (int *Ut,string texfile)
{
        //If beginning the vector, put new model, else seach for existing
        int rtnID;
        if (NTex == 0)
        {
                        TexrLib.push_back(texfile);
			TexrLib[NTex].LoadTextureDataToCPU(Ut);
			//TexrLib[NTex].LoadTextureDataToGPU();
                        rtnID = NTex;
			#pragma omp critical
                        {++NTex;}
        } else {
                bool srhval = false;
                for (int i = 0; i < NTex;++i)
                {
                        if (texfile.compare(TexrLib[i].texname) == 0)
                        {
                                rtnID = i;
                                break;
                        } else {srhval = true;}
                }

                //If Model isnt loaded, create a new one.
                if (srhval)
                {
                        TexrLib.push_back(texfile);
			TexrLib[NTex].LoadTextureDataToCPU(Ut);
			//TexrLib[NTex].LoadTextureDataToGPU();
                        rtnID = NTex;
			#pragma omp critical
                        {++NTex;}
                }
        }
        return rtnID;
};

//------------------------------------------
//              OPF Loader
//------------------------------------------
//Defines our 
int* objPropController::populateArray(string data)
{
	size_t beg = data.find_first_of("[")+1;
	size_t end = data.find_first_of("]");

	//cout << "DATA: " << data << " beg: " << beg << " end: " << end << endl;
	string subdat = data.substr(beg,end-beg);
	//cout << "SUBDAT: " << subdat << endl;

	vector<int> vals;
	bool chk = true;
	while (chk)
	{
		size_t tpos = subdat.find_first_of(",");
		//cout << "tpos: " << tpos << endl;

		if (tpos != string::npos)
		{
			string sval = subdat.substr(0,tpos);
			subdat = subdat.substr(tpos+1);
			//cout << "sval: " << sval << " subdat: " << subdat << endl;
			vals.push_back(atoi(sval.c_str()));
		} else {	
			vals.push_back(atoi(subdat.c_str()));
			chk = false;
		}
	}

	int *rtn = new int [vals.size()];

	for (int i=0;i<(int)vals.size();++i)
		{rtn[i] = vals[i];}

	return rtn;	
};

//------------------------------------------
//   		OPF Loader
//------------------------------------------
//Defines our 
bool objPropController::OpfLoader()
{
	//Begin Parsing the OPF file
	int lines;
	vector<string> DATA;
	bool errorchk;

	if (readExtern(DATA,lines,"bin/opfs/",opfsfile))
	{
		errorchk=true;
		/*for (int i = 0; i < lines; ++i)
		{
			cout << "LINE(" << i << "): "  << DATA[i] << endl;
		}*/
	
		cout << "FILENAME: " << opfsfile << endl;
	
		//Begin Registering Classes
		int wkline;
		this->NObjs = atoi(searchDelim(DATA,lines,0,wkline,"&NUM_OBJS").c_str());
		for (int i = 0; i < NObjs; ++i)
		{
			//cout << "OBJECT " << i << endl;
			//Determine starting point of object block
			int srtline = searchLine(DATA,lines,wkline+1,"&BEGINOBJECT:");
	
			//Create the new object class
			ObjLib.push_back(searchDelim(DATA,lines,srtline+1,wkline,"objname"));
	
			//Register the model within the class
			ObjLib[i].ModID = RegisterModel(searchDelim(DATA,lines,wkline+1,wkline,"objfile"));

                        //Read number of Meshs
                        ObjLib[i].optlinfo = searchDelim(DATA,lines,wkline+1,wkline,"optlinfo");
	
        	        //Read number of Meshs
			ObjLib[i].Nmesh = atoi(searchDelim(DATA,lines,wkline+1,wkline,"nummesh").c_str());
	
			//Loop through mesh data
			for (int j = 0; j < ObjLib[i].Nmesh; ++j)
			{
				//cout << "MESH " << j << endl;
	                        ObjLib[i].lighting.push_back(searchDelim(DATA,lines,wkline+1,wkline,"meshlight"));
	                        //Save lighting type
				int shader = atoi(searchDelim(DATA,lines,wkline+1,wkline,"meshshader").c_str());
	                        ObjLib[i].Shader.push_back(shader);

                		//Load and register textures
				int *Ut = populateArray(searchDelim(DATA,lines,wkline+1,wkline,"meshmaps"));
                		ObjLib[i].MeshTexID.push_back(RegisterTexture(Ut,searchDelim(DATA,lines,wkline+1,wkline,"meshtex")));
				if (Ut[6]==1 || Ut[7]==1)
					{ModLib[ObjLib[i].ModID].mTBN.push_back(true);}
				else 
					{ModLib[ObjLib[i].ModID].mTBN.push_back(false);}

				delete [] Ut;
			}
		
			//Set the next working line
			wkline = searchLine(DATA,lines,wkline+1,"&ENDOBJECT:");
		
		}
		//cout << "NUMBER OF OBJECTS: " << NObjs << endl;

		DATA.clear();
	} else {errorchk=false;cout << "!OPF Failed to load!\n";}
	return errorchk;
};

//------------------------------------------
//     Draw All Entities Currently Set
//------------------------------------------
//EVENTUALLY WE NEED TO CHANGE THIS TO ALLOW FOR INSTANCING AND SAVE A BILLION DRAW CALLS
void objPropController::DrawObjects(Shader shader,int OID,int MESHID)
{
	int MID = ObjLib[OID].ModID;//Model ID

	//Set Materials
	//cout << "Set Material\n";
	ModLib[MID].mesh[MESHID].setMaterial(shader.Program);

       	//Handle Texture Binding
	//cout << "Set Textures\n";
       	int MTID = ObjLib[OID].MeshTexID[MESHID];//Mesh Texture ID
       	TexrLib[MTID].useTexture(shader);//Set the correct textures for the mesh

       	// Draw mesh
	//cout << "Draw Mesh\n";
       	ModLib[MID].mesh[MESHID].Draw();
       
       	// Reset the active textures
	//cout << "Reset Textures\n";
       	glActiveTexture(GL_TEXTURE0);
       	glBindTexture(GL_TEXTURE_2D, 0);
};

//------------------------------------------
//     Draw All Instances Currently Set
//------------------------------------------
//EVENTUALLY WE NEED TO CHANGE THIS TO ALLOW FOR INSTANCING AND SAVE A BILLION DRAW CALLS
void objPropController::DrawInstancedObjects(Shader shader,int OID,int MESHID,int NumInstances)
{
        int MID = ObjLib[OID].ModID;//Model ID

        //Set Materials
        //cout << "Set Material\n";
        ModLib[MID].mesh[MESHID].setMaterial(shader.Program);

        //Handle Texture Binding
        //cout << "Set Textures\n";
        int MTID = ObjLib[OID].MeshTexID[MESHID];//Mesh Texture ID
        TexrLib[MTID].useTexture(shader);//Set the correct textures for the mesh

        // Draw mesh
        //cout << "Draw Mesh\n";
        ModLib[MID].mesh[MESHID].DrawInstanced(NumInstances);

        // Reset the active textures
        //cout << "Reset Textures\n";
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
};


