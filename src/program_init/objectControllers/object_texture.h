#pragma once

#include "../mainHeaders.h" 
#include "shader.h"
using namespace std;
//**************************
//Vertex Object Loader Class
//**************************
class Texture
{
public:
  	// Our program variables
        std::vector<GLuint> TextureID;
        std::vector<string> Uniform;
        std::vector<int> idx;
	std::vector<int> Ut;//Texture types included
	int ttex;//Total Textures

	//GLuint Texture;
	string texname;

	//Check if loaded to GPU
	bool CPUload;
	bool GPUload;


	//Uniform Types
	//Ut[0]=Diff;Ut[1]=Spec;Ut[2]=Glow;Ut[3]=Ovly;Ut[4]=Aovly;
	vector<int> w,h;
        vector<string> filenames;//Filenames
	vector<string> imageData;
	vector<unsigned char *> image;

	// Constructor reads and builds our shader -- First NDiff filenames are diffuse, last NSpec
	Texture (string filename) {this->texname = filename;ttex=0;GPUload=false;CPUload=false;};
	~Texture () {}

	void LoadTextureDataToCPU(int *Ut) 
	{
                if(!CPUload)
                {
			CPUload=true;
                	for (int i=0;i<8;++i)
			{
				this->Ut.push_back(Ut[i]);
                		ttex += Ut[i];
			}

                	string d = "d_"; string s = "s_"; 
                	string g = "g_"; 
                	string o = "o_"; string oa = "a_";
                	string b = "b_";
                	string dm = "dm_"; string n = "n_";
                	string ext = ".png";
	
	        	stringstream ss[this->Ut.size()];
	        	ss[0] << texname << "/" << d << texname << ext;
	        	ss[1] << texname << "/" << s << texname << ext;
	        	ss[2] << texname << "/" << g << texname << ext;
	        	ss[3] << texname << "/" << o << texname << ext;
	        	ss[4] << texname << "/" << oa << texname << ext;
	        	ss[5] << texname << "/" << b << texname << ext;
	        	ss[6] << texname << "/" << dm << texname << ext;
	        	ss[7] << texname << "/" << n << texname << ext;
	
                	string filename = "bin/textures/";

			filenames.resize(this->Ut.size());
                	for (int i=0;i<(int)this->Ut.size();++i)
			{
				string fntmp = filename + ss[i].str();
				filenames[i] = fntmp;
			}

			int Num = 0; 
			#pragma omp critical
			{
                	for (int i=0;i<(int)this->Ut.size();++i)
			{
				if (Ut[i]==1)
				{
					w.push_back(0);h.push_back(0);
					//THIS NEEDS TO BE THREAD SAFE FOR LINUX, NOT SURE ABOUT OTHER OSs
					image.push_back(SOIL_load_image(filenames[i].c_str(), &w[Num], &h[Num], 0, SOIL_LOAD_RGBA));
                			if (image[Num] == NULL) {cout << "ERROR: Texture failed to load: " << filenames[i].c_str() << "\n";}
					//cout << "\nIMAGE:\n " << image[Num] << endl;
					++Num;
				}
			}
			}
		}
		//cout << "Textures: " << texname << " " << Num << " loaded for a total of " << sizeof(image) << " bytes\n";
	};

	void LoadTextureDataToGPU()
	{
        	//Texture 1 Define
		//this->Ut = Ut;
		if(!GPUload)
		{
			GPUload=true;
			int Num = 0;
			for (int i = 0; i < (int)Ut.size(); ++i)
			{
				if (Ut[i]==1)
				{
					GLuint tex;
		        		glGenTextures(1, &tex);
					this->TextureID.push_back(tex);
	
	        			glBindTexture(GL_TEXTURE_2D, this->TextureID[Num]);
	        			//Load Texture 1
	       				//int w, h;
					//else {cout << "SUCCESS: Texture loaded: " << filename.c_str() << "\n";}
	        			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w[Num], h[Num], 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)image[Num]);
	        			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 100, 100, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData[Num].c_str());
	        			glGenerateMipmap(GL_TEXTURE_2D);
	
        				// Set our texture parameters
        				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
        				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        				// Set texture filtering
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
					cout << "Tex(" << Num << ") Binding Texture: " << filenames[i] << " Address: " << this->TextureID[Num] << "\n";
        				glBindTexture(GL_TEXTURE_2D, 0);

					this->Uniform.push_back(Generate_Uniform_String(i));
					this->idx.push_back(i);
					//cout << Uniform[Num] << "\n";
					++Num;
				}
			}
		}
	};

	void TextureDeleteFromGPU ()
	{
		cout << "Clearing GPU data...\n";
		GPUload=false;
		TextureID.clear();
		Uniform.clear();
		idx.clear();
		glDeleteTextures(TextureID.size(),&TextureID[0]);
	};

	void useTexture (Shader shader) 
	{
		for (int i = 0; i < ttex; ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
                	glBindTexture(GL_TEXTURE_2D, this->TextureID[i]);
                	glUniform1i(glGetUniformLocation(shader.Program, Uniform[i].c_str()), i);
		}
	};

	string Generate_Uniform_String(int i)
	{
		string us;

		switch(i)
		{
			case 0: {us = "material.diffuseMap";break;}
			case 1: {us = "material.specularMap";break;}
			case 2: {us = "material.glowMap";break;}
			case 3: {us = "material.overlayMap";break;}
			case 4: {us = "material.alphaoverlayMap";break;}
			case 5: {us = "material.bumpMap";break;}
			case 6: {us = "material.displacementMap";break;}
			case 7: {us = "material.normalMap";break;}
		}

		return us;
	};
};
