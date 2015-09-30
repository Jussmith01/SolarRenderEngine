#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
  
#include <GL/glew.h> // Include glew to get all the required OpenGL headers
//**************************
//Vertex Object Loader Class
//**************************
class vObjectLoader
{
public:
  	// Our program ID
	int verts;
	GLuint VAO;
	GLuint VBO;
	// Constructor reads and builds our shader
	vObjectLoader(GLfloat vertices[],int num_vert)
	{
		//Produce Vertex Array Object
        	glGenVertexArrays(1, &VAO);
        	glGenBuffers(1, &VBO);
		verts = num_vert;
        	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
        	// 1. Bind our Vertex Array Object
        	glBindVertexArray(VAO);

        	// 2. Copy our vertices array and index array in a buffer for OpenGL to use
        	glBindBuffer(GL_ARRAY_BUFFER, VBO);
        	glBufferData(GL_ARRAY_BUFFER, 8 * num_vert * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);

        	// 3. Then set ounspose(inverse(model))) *r vertex attributes pointers
        	//Position Attribute
		glEnableVertexAttribArray(0);
        	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        	//Texture Attribute
        	glEnableVertexAttribArray(1);
        	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        	//Normal Attribute
        	glEnableVertexAttribArray(2);
        	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	
	        //4. Unbind our VAO (NOT the VBO)
	        glBindVertexArray(0);
	}


	// Uses the current shader
	void Draw() 
	{ 
        	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		//glEnableVertexAttribArray(0);
		//glEnableVertexAttribArray(1);
		//glEnableVertexAttribArray(2);
		glBindVertexArray(VAO); 
		glDrawArrays(GL_TRIANGLES, 0, verts); 
		glBindVertexArray(0); 
		//glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);
	}
};
