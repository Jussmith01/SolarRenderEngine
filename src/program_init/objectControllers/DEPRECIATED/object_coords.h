#pragma once

#include "../mainHeaders.h"
#include "../basicStructures/gameStructs.h"

using namespace std;
  
#include "shader.h"
//**************************
//Vertex Object Loader Class
//**************************
class ObjectCoords
{
public:
  	// Our program variables
        GLint modelLoc;
        GLint viewLoc;
        GLint projLoc;

	// Constructor reads and builds our shader
	ObjectCoords () {};
	~ObjectCoords () {};

	void SetLocs(Shader shader)
	{
		GLint Prog = shader.Program;
                modelLoc = glGetUniformLocation(Prog, "model");
                viewLoc = glGetUniformLocation(Prog, "view");
                projLoc = glGetUniformLocation(Prog, "proj");
		//cout << "SHADER: " << Prog << " model: " << modelLoc << " view: " << viewLoc << " proj: " << projLoc << endl;
	}

	void SetView (glm::mat4 view,glm::mat4 projection)
	{
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	};

	void MoveObject (glm::vec3 posvec,glm::mat4 entrotmat_gbl,float scale)
	{
		glm::mat4 model;
		//model = entrotmat_gbl;
		model = glm::translate(model, posvec);
		model = glm::scale(model, glm::vec3(scale));
		model = model * entrotmat_gbl;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	};
};
