#pragma once
#ifndef objlight_class
#define objlight_class

#include "../basicStructures/base_classes.h"
using namespace std;
//**************************
//Vertex Object Loader Class
//**************************
class Light
{
	public:
  	// Our program variables
        GLint PcLoc[4]; GLint PlLoc[4]; GLint PqLoc[4];
	GLint FcLoc; GLint FlLoc; GLint FqLoc;

	GLint lightDirLoc;
	GLint intensityLoc; //Intensity of the directional light

	GLint spotDirLoc; GLint spotCutOffLoc; GLint spotOuterCutOffLoc;
	
	GLfloat Pc[4]; GLfloat Pl[4]; GLfloat Pq[4];
        GLfloat Fc; GLfloat Fl; GLfloat Fq;

	GLfloat sco; GLfloat soco;

        GLint matAmbientLoc; GLint matDiffuseLoc; GLint matSpecularLoc;
        GLint matShineLoc;

	GLint PlightPosLoc[4];
	GLint FlightPosLoc;
	GLint camPosLoc;

        glm::vec3 ambient; glm::vec3 diffuse; glm::vec3 specular;
        float shine; float intensity;

	void SetLightParms(float &c,float &l,float &q, int dist)
	{
                switch (dist)
                {
                        case 7: {c = 1.0f; l = 0.7; q = 1.8; break;}
                        case 13: {c = 1.0f; l = 0.35; q = 0.44; break;}
                        case 20: {c = 1.0f; l = 0.22; q = 0.20; break;}
                        case 32: {c = 1.0f; l = 0.14; q = 0.07; break;}
                        case 50: {c = 1.0f; l = 0.09; q = 0.032; break;}
                        case 65: {c = 1.0f; l = 0.07; q = 0.017; break;}
                        case 100: {c = 1.0f; l = 0.07; q = 0.017; break;}
                        case 160: {c = 1.0f; l = 0.027; q = 0.0028; break;}
                        case 200: {c = 1.0f; l = 0.022; q = 0.0019; break;}
                        case 325: {c = 1.0f; l = 0.014; q = 0.0007; break;}
                        case 600: {c = 1.0f; l = 0.007; q = 0.0002; break;}
                        case 3250: {c = 1.0f; l = 0.0014; q = 0.000007; break;}
                        default: {c = 1.0f; l = 0.014; q = 0.0007; break;}
                }
	};

        void SetMaterial(GLuint Prog)
        {
                matAmbientLoc = glGetUniformLocation(Prog, "light.ambient");
                matDiffuseLoc = glGetUniformLocation(Prog, "light.diffuse");
                matSpecularLoc = glGetUniformLocation(Prog, "light.specular");
                matShineLoc = glGetUniformLocation(Prog, "light.shininess");
        };

	// Constructor reads and builds our shader
	void SetProperties(Material materials,int Fdist,int Pdist,GLuint Prog)
	{
		for (int i = 0; i < 4; ++i)
		{
			stringstream lps,cs,ls,qs;
			lps << "Plight[" << i << "].lightPos";
			cs << "Plight[" << i << "].constant";
			ls << "Plight[" << i << "].linear";
			qs << "Plight[" << i << "].quadratic";

			PlightPosLoc[i] = glGetUniformLocation(Prog, lps.str().c_str());
			PcLoc[i] = glGetUniformLocation(Prog, cs.str().c_str());
			PlLoc[i] = glGetUniformLocation(Prog, ls.str().c_str());
			PqLoc[i] = glGetUniformLocation(Prog, qs.str().c_str());
		
			SetLightParms(Pc[i],Pl[i],Pq[i],Pdist);
		}

                FcLoc = glGetUniformLocation(Prog, "Flight.constant");
                FlLoc = glGetUniformLocation(Prog, "Flight.linear");
                FqLoc = glGetUniformLocation(Prog, "Flight.quadratic");

		FlightPosLoc = glGetUniformLocation(Prog, "Flight.lightPos");
		spotDirLoc = glGetUniformLocation(Prog, "Flight.spotDir");
		spotCutOffLoc = glGetUniformLocation(Prog, "Flight.spotCutOff");
		spotOuterCutOffLoc = glGetUniformLocation(Prog, "Flight.spotOuterCutOff");
  	
		lightDirLoc = glGetUniformLocation(Prog, "Dlight.direction");
		intensityLoc = glGetUniformLocation(Prog, "Dlight.intensity");
		
		camPosLoc = glGetUniformLocation(Prog, "light.viewPos");
		
		sco = glm::cos(glm::radians(18.5f));
		soco = glm::cos(glm::radians(25.5f));

		intensity = 1.0f;

		ambient = materials.Ka;
                diffuse = materials.Kd;
                specular = materials.Ks;
                shine = materials.shine;

		SetLightParms(Fc,Fl,Fq,Fdist);

		SetMaterial(Prog);
	};

	//Set Lights
	void SetLight(glm::vec3 camPos, glm::vec3 camDir, glm::vec3 sunDir,glm::vec3 lightPos[]) 
	{
                glUniform3f(lightDirLoc, sunDir.x, sunDir.y, sunDir.z); 
                glUniform1f(intensityLoc, intensity); 
                
                glUniform3f(spotDirLoc, camDir.x, camDir.y, camDir.z);
                glUniform1f(spotCutOffLoc, sco); 
                glUniform1f(spotOuterCutOffLoc, soco); 

		glUniform3f(matAmbientLoc, ambient.x, ambient.y, ambient.z);
                glUniform3f(matDiffuseLoc, diffuse.x, diffuse.y, diffuse.z);
                glUniform3f(matSpecularLoc, specular.x, specular.y, specular.z);
                glUniform1f(matShineLoc, shine);

                for (int i = 0; i < 4; ++i)
                {
                	glUniform1f(PcLoc[i], Pc[i]); 
                	glUniform1f(PlLoc[i], Pl[i]); 
                	glUniform1f(PqLoc[i], Pq[i]); 

                	glUniform3f(PlightPosLoc[i], lightPos[i].x, lightPos[i].y, lightPos[i].z);
		}
 
                glUniform1f(FcLoc, Fc);
                glUniform1f(FlLoc, Fl);
                glUniform1f(FqLoc, Fq);

		glUniform3f(FlightPosLoc, camPos.x, camPos.y, camPos.z);
		glUniform3f(camPosLoc, camPos.x, camPos.y, camPos.z);
	};
};
#endif
