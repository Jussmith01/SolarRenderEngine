#pragma once
#include "../mainHeaders.h"
using namespace std;
  
class Shader
{
public:
  	// Our program ID
	GLuint Program;
	string shdrname;

	// Constructor reads and builds our shader
	Shader() {};
	Shader(string file) {ShaderSet(file);};

	void ShaderSet(string filename)
	{
		//Produce Filename
		this->shdrname = filename;
		string dir = "bin/shaders/";
		stringstream ssv;
		stringstream ssf;

		ssv << dir << filename << ".vs";
		ssf << dir << filename << ".fs";

		// 1. Retrieve the vertex/fragment source code from filePath
		string vertexCode;
		string fragmentCode;
		try 
		{
	 		// Open files
	 		ifstream vShaderFile(ssv.str().c_str());
	 		ifstream fShaderFile(ssf.str().c_str());
	 		stringstream vShaderStream, fShaderStream;
	 		// Read file's buffer contents into streams
	 		vShaderStream << vShaderFile.rdbuf();
	 		fShaderStream << fShaderFile.rdbuf();		
	 		// close file handlers
	 		vShaderFile.close();
	 		fShaderFile.close();
			// Convert stream into GLchar array
	 		vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();		
		}
		catch(exception e)
		{
	 	 	cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		}

        	const GLchar* vShaderCode = vertexCode.c_str();
        	const GLchar* fShaderCode = fragmentCode.c_str();

		//**************************************************
		// 2. Compile shaders
		//**************************************************
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		//**************************************************
		// 		    Vertex Shader
		//**************************************************
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
		};
		//**************************************************
		// 		  Fragment Shader
		//**************************************************
                fragment = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragment, 1, &fShaderCode, NULL);
                glCompileShader(fragment);
                // Print compile errors if any
                glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
                if(!success)
                {
                        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
                };

		//**************************************************
		// 		  Shader Program
		//**************************************************
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		// Print linking errors if any
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if(!success)
		{
		  	glGetShaderInfoLog(this->Program, 512, NULL, infoLog);
		  	cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		GLuint UniformBlockIdx = glGetUniformBlockIndex(this->Program, "Matrices");
		glUniformBlockBinding(this->Program, UniformBlockIdx, 0);
	};

	// Uses the current shader
	void Use() 
	{
		if(!glIsProgram(this->Program)) {cout << "ERROR: Shader not found.\n";}
		glUseProgram(this->Program);
	};
};
