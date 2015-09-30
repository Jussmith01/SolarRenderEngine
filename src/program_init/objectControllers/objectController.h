#define GLEW_STATIC
#ifndef ObjPropControl
#define ObjPropControl

#define STD_LIGHT_3 0
#define STD_LIGHT_4 1
#define STD_SOURCE 2

#include "../mainHeaders.h"
#include "model.h"

using namespace std;
//------------------------------------------
//  	 	Object Class
//------------------------------------------
struct Object
{
	string Objname;
	string optlinfo;
	int ModID;
	int Nmesh;

	//objPhys PhysProps;

	vector<int> MeshTexID;
	vector<int> Shader;
	vector<string> lighting;//no, point, (eventually spot)

	//Constructor
	Object(string Objname) {this->Objname=Objname;};

	//Destructor
	~Object() 
	{
		Shader.clear();
		MeshTexID.clear();
	};
};

//------------------------------------------
//   Object Properties Controller Class
//------------------------------------------
class objPropController
{
	public:
	int NObjs;//Number of objects in the controller class
	int NMod;//Number of models in the controller class
	int NTex;//Number of textures in the controller class
	int NLit;//Number of lights in the controller class
	string opfsfile;//Stores the opf filename

	vector<Object> ObjLib; //Library of object properties -- Defined in bin/[filename].opf
	vector<Model> ModLib; //Library of models - Of course, this holds the mesh draw functions 
	vector<Texture> TexrLib; // Library of textures

	objPropController(string opfsfilename) {this->opfsfile = opfsfilename;NMod = 0;NTex = 0;NLit= 0;};
	objPropController() {NMod = 0;NTex = 0;NLit= 0;};

	~objPropController() 
	{
		ObjLib.clear();
		ModLib.clear();
		TexrLib.clear();
	};

	bool OpfLoader(void);//In charge of loading and registering all data from .opf file.
	void DrawObjects (Shader shader,int OID,int MESHID); //Draw all registered objects
	void DrawInstancedObjects(Shader shader,int OID,int MESHID,int NumInstances);
	int* populateArray(string data);

        void TextureToGPULoader(int OBJID)//In charge of loading the textures into the texrlib library
	{
	        for (int i=0;i<ObjLib[OBJID].Nmesh;++i)
		{
	        	int TID = ObjLib[OBJID].MeshTexID[i];
	        	TexrLib[TID].LoadTextureDataToGPU();
		}
	};
       //int DetermineShaderID(int MLTYPE);//In charge of loading the lighting data into the litlib library

	private:
	//******************
	//Register Functions
	//******************
	int RegisterModel (string objfile); //Register an object in modlib
	int RegisterTexture (int* Ut,string texfile); //Register an object in texrlib
};
#endif
