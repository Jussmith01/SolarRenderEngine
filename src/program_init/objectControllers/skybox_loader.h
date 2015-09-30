#pragma once

#include "../mainHeaders.h" 
#include "shader.h"
#include "../basicStructures/gameStructs.h"
using namespace std;
//**************************
//Vertex Object Loader Class
//**************************
class Skybox
{
public:
  	//Texuter variables
        GLuint TextureID;
        string Uniform;

	//GLuint Texture;
	string texname;

	//Drawing Data
	Shader shader;
	GLuint skyboxVAO, skyboxVBO;
	vector<glm::vec3> skyboxverts;

	//Check if loaded to GPU
	bool CPUload;
	bool GPUload;

	//Image Data held on CPU
	vector<int> w,h;
        vector<string> filenames;//Filenames
	vector<unsigned char *> image;

	// Constructor reads and builds our shader -- First NDiff filenames are diffuse, last NSpec
	Skybox () {GPUload=false;CPUload=false;};
	Skybox (string filename) {GPUload=false;CPUload=false;SetupClass(filename);};
	~Skybox () {}

	void SetupClass(string texturename)
        {
		this->texname = texturename;
		shader.ShaderSet("cubemap");
	}

	void LoadSkyboxDataToCPU() 
	{
                if(!CPUload)
                {
			CPUload=true;

                	string Px = "Px_"; 
                	string Nx = "Nx_"; 
                	string Py = "Py_"; 
                	string Ny = "Ny_"; 
                	string Pz = "Pz_"; 
                	string Nz = "Nz_"; 
                	string ext = ".png";
	
	        	stringstream ss[6];
	        	ss[0] << texname << "/" << Px << texname << ext;
	        	ss[1] << texname << "/" << Nx << texname << ext;
	        	ss[2] << texname << "/" << Py << texname << ext;
	        	ss[3] << texname << "/" << Ny << texname << ext;
	        	ss[4] << texname << "/" << Pz << texname << ext;
	        	ss[5] << texname << "/" << Nz << texname << ext;
	
                	string filename = "bin/skybox/";

			filenames.resize(6);
                	for (int i=0;i<6;++i)
			{
				string fntmp = filename + ss[i].str();
				filenames[i] = fntmp;
			}

			#pragma omp critical
			{
                		for (int i=0;i<6;++i)
				{
					w.push_back(0);h.push_back(0);
					//THIS NEEDS TO BE THREAD SAFE FOR LINUX, NOT SURE ABOUT OTHER OSs
					image.push_back(SOIL_load_image(filenames[i].c_str(), &w[i], &h[i], 0, SOIL_LOAD_RGBA));
               				if (image[i] == NULL) {cout << "ERROR: CubeMap Texture failed to load: " << filenames[i].c_str() << "\n";}
				}
			}

			this->Uniform = "skybox";

        		//Set Skybox Positions        
       			skyboxverts.push_back(glm::vec3(-1.0f,  1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f, -1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f, -1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f,  1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f,  1.0f, -1.0f));
  
        		skyboxverts.push_back(glm::vec3(-1.0f, -1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f,  1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f,  1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f,  1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f, -1.0f,  1.0f));
  
        		skyboxverts.push_back(glm::vec3( 1.0f, -1.0f, -1.0f));
         		skyboxverts.push_back(glm::vec3( 1.0f, -1.0f,  1.0f));
         		skyboxverts.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f,  1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f, -1.0f, -1.0f));
   
        		skyboxverts.push_back(glm::vec3(-1.0f, -1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f,  1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f, -1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f, -1.0f,  1.0f));
  
        		skyboxverts.push_back(glm::vec3(-1.0f,  1.0f, -1.0f));
         		skyboxverts.push_back(glm::vec3( 1.0f,  1.0f, -1.0f));
         		skyboxverts.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f,  1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f,  1.0f, -1.0f));
  
        		skyboxverts.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f, -1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f, -1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f, -1.0f, -1.0f));
        		skyboxverts.push_back(glm::vec3(-1.0f, -1.0f,  1.0f));
        		skyboxverts.push_back(glm::vec3( 1.0f, -1.0f,  1.0f));
		}
		//cout << "Textures: " << texname << " " << Num << " loaded for a total of " << sizeof(image) << " bytes\n";
	};

	void LoadSkyboxDataToGPU()
	{
        	//Texture 1 Define
		//this->Ut = Ut;
		if(!GPUload)
		{
			GPUload=true;
		
			//Loading Texture Data
		        glGenTextures(1, &TextureID);
			glActiveTexture(GL_TEXTURE0);
	        	glBindTexture(GL_TEXTURE_CUBE_MAP, this->TextureID);
			for (int i = 0; i < 6; ++i)
			{
	        			//Load Texture 1
	        			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w[i], h[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)image[i]);
	
					cout << "CubeMap Tex(" << i << ") Binding Texture: " << filenames[i] << " Address: " << this->TextureID << "\n";
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
        		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			//Loading Box Data
			glGenVertexArrays(1, &skyboxVAO);
    			glGenBuffers(1, &skyboxVBO);
    			glBindVertexArray(skyboxVAO);
    			glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

			int sbvsize = skyboxverts.size();
			cout << "Skybox Size: " << sbvsize << endl;

    			glBufferData(GL_ARRAY_BUFFER, sbvsize * sizeof(glm::vec3), &skyboxverts[0], GL_STATIC_DRAW);
    			glEnableVertexAttribArray(0);
    			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    			glBindVertexArray(0);
		}
	};

	void DeleteSkyboxFromCPU ()
        {
                if(CPUload)
                {
                        cout << "Clearing Skybox CPU data...\n";
                        CPUload=false;
			//skyboxVAO=0;
			//skyboxVBO=0;
			skyboxverts.clear();
			w.clear();h.clear();
			filenames.clear();
			image.clear();
                } else {cout << "Skybox CPU Data not Set. Cannot Clear.";}
        };

	void DeleteSkyboxFromGPU ()
	{
		if(GPUload)
		{
			cout << "Clearing Skybox GPU data...\n";
			GPUload=false;
			skyboxVAO=0;
			skyboxVBO=0;
			glDeleteTextures(1,&TextureID);
			glDeleteVertexArrays(1,&skyboxVAO);
			glDeleteBuffers(1,&skyboxVBO);
			TextureID=0;
		} else {cout << "Skybox GPU Data not Set. Cannot Clear.";}
	};

	void DrawSkybox (Camera &camera,Properties &props)
	{
		if (GPUload)
		{
			//glDepthFunc(GL_LEQUAL);
			glDepthMask(GL_FALSE);
			shader.Use();

       		        glm::mat4 proj = glm::perspective(camera.aspect, (float)camera.WindowWidth/(float)camera.WindowHeight, 0.1f, 100.0f);
        	        glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(camera.Pos, camera.Pos + camera.Dir, camera.Up)));

        	        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        	        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

			// skybox cube
        		glBindVertexArray(skyboxVAO);
		        glActiveTexture(GL_TEXTURE0);
		        glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
			//cout << "Shader.Prog: " << shader.Program  << " UniLoc: " << glGetUniformLocation(shader.Program, "skybox") << " TextureID: " << TextureID << endl;
		        glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 0);
		        glDrawArrays(GL_TRIANGLES, 0, 36);
		        glBindVertexArray(0);
	
			glDepthMask(GL_TRUE);
			//glDepthFunc(GL_LESS);
		}
	};
};
