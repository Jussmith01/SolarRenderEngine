#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
  
#include <GL/glew.h> // Include glew to get all the required OpenGL headers
#include "object_loader.h"
//**************************
//Vertex Object Loader Class
//**************************
class ObjectMaterial
{
public:
  	// Our program variables
        GLint matAmbientLoc;
        GLint matDiffuseLoc;
        GLint matSpecularLoc;
        GLint matShineLoc;

	glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shine;

	// Constructor reads and builds our shader
	void SetProperties(string material,GLuint Prog)
	{
		matAmbientLoc = glGetUniformLocation(Prog, "material.ambient");
		matDiffuseLoc = glGetUniformLocation(Prog, "material.diffuse");
		matSpecularLoc = glGetUniformLocation(Prog, "material.specular");
		matShineLoc = glGetUniformLocation(Prog, "material.shininess"); 
  	
		if (material.compare("default") == 0)
		{
			ambient = glm::vec3(1.0f,0.5f,0.31f);
			diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
			specular = glm::vec3(0.5f, 0.5f, 0.5f);
			shine = 32.0f;
		}
                else if (material.compare("basic_whitelight") == 0)
                {
                        ambient = glm::vec3(0.1f);
                        diffuse = glm::vec3(0.5f);
                        specular = glm::vec3(0.5f);
                        shine = 16.0f;
                }
                else if (material.compare("gold") == 0)
                {
                        ambient = glm::vec3(0.24725f,0.1995f,0.0745f);
                        diffuse = glm::vec3(0.75164f,0.60648f,0.22648f);
                        specular = glm::vec3(0.628281f,0.555802f,0.366065f);
                        shine = 128.0f * 0.4f;
                }
                else if (material.compare("chrome") == 0)
                {
                        ambient = glm::vec3(0.25f);
                        diffuse = glm::vec3(0.40f);
                        specular = glm::vec3(0.774597f);
                        shine = 128.0f * 0.6f;
                }

	}

	void SetMaterial() 
	{
		glUniform3f(matAmbientLoc, ambient.x, ambient.y, ambient.z);
                glUniform3f(matDiffuseLoc, diffuse.x, diffuse.y, diffuse.z);
                glUniform3f(matSpecularLoc, specular.x, specular.y, specular.z);
                glUniform1f(matShineLoc, shine); 
	}
};
