#define GLEW_STATIC
#ifndef ModelClass
#define ModelClass

#include "../mainHeaders.h"
#include "meshloader.h"

using namespace std;

class Model
{
	public:
	//Declare a vector for our mesh classes
	glm::vec3 ModelPosition;
	std::vector<Mesh> mesh;
	std::vector<bool> mTBN; //Calculates the meshes tangent and bitangent vectors
	string objfile;
	int Nmesh;
	bool CPULoad;
	bool GPULoad;
	bool INSTLoad;

	Model(string objfile) 
	{
		this->objfile = objfile;
		CPULoad = false;
		GPULoad = false;
		INSTLoad = false;
	};

	void LoadModelToCPU()
	{
		if (!CPULoad)
		{
			CPULoad=true;
        	        objLoader f(objfile.c_str());
        	        Nmesh = f.RtnNumMesh();
	
                	for (int i = 0; i < Nmesh; ++i)
                	{
                	        mesh.push_back(f.CreateMesh(i,mTBN[i]));
                	        //mesh[i].SetMeshOnDevice();
                	}
                	ModelPosition = glm::vec3(0.0f,0.0f,-1.0f);
		}
	};

	void LoadModelToGPU()
	{
		if (CPULoad)
		{
			if (!GPULoad)
			{
				GPULoad=true;
				//cout << "loading Model to GPU: MESHSIZE: " << mesh.size() << "\n";
                		for (int i = 0; i < Nmesh; ++i)
               			 	{mesh[i].SetMeshOnDevice();}
			}
		} else {cout << "ERROR: Data not loaded to CPU!\n";}
	};

        void SetupInstancing(vector<glm::mat4> &modelMatrices)
        {
                if (CPULoad)
                {
                        if (GPULoad)
                        {
                                //cout << "loading Model to GPU: MESHSIZE: " << mesh.size() << "\n";
				INSTLoad = true;
                                for (int i = 0; i < Nmesh; ++i)
                                        {mesh[i].setupInstanceArray(modelMatrices);}
                        } else {cout << "ERROR: Data not loaded to GPU!\n";}
                } else {cout << "ERROR: Data not loaded to CPU!\n";}
        };

        void UpdateInstancedModelMatrices(vector<glm::mat4> &modelMatrices)
        {
                if (CPULoad)
                {
                        if (GPULoad)
                        {
				if (INSTLoad)
				{
                                	for (int i = 0; i < Nmesh; ++i)
                                	        {mesh[i].UpdateInstancedModelMatrices(modelMatrices);}
				} else {cout << "ERROR: Instancing is not setup for this model!\n";}
                        } else {cout << "ERROR: Data not loaded to GPU!\n";}
                } else {cout << "ERROR: Data not loaded to CPU!\n";}
        };

	~Model() {};
};

#endif
