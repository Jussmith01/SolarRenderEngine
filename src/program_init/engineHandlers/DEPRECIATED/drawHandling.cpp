#define GLEW_STATIC

//#include "../mainHeaders.h"
#include "../EngineHeader.h"
//#include "../objectControllers/objectPhysics.h"
#include "../objectControllers/object_coords.h"

using namespace std;

struct PerspLevel
{
	vector<int> EID;
};
//-------------------------------------------//
//             Console Functions             //
//-------------------------------------------//
void Engine_Init::DrawAll()
{
	glm::vec3 CamPos = camera.Pos;
	PerspLevel PL[3];
        for (unsigned int i = 0; i < OPY.ents.size(); ++i)
        {
                float Dist = glm::length(OPY.entpositions[i] - CamPos);
		//float ClipDist = Dist/(float)OPY.scale[i];

		//if (ClipDist < 1000)
		//{
			if (Dist > 8500.0f)                       {PL[0].EID.push_back(i);} 
			else if (Dist >= 1.5f && Dist <= 8500.0f) {PL[1].EID.push_back(i);} 
			else 					  {PL[2].EID.push_back(i);}
		//} //else {cout << "Entity " << i << " Clipped\n";}
	}

	//glDepthFunc(GL_LESS);
        for (unsigned int j = 0; j < 3; ++j)
	{
		//glDepthRange(0.666f-j*0.3333f,1.0-j*0.333f);
		//glDepthRange(0.0f,1.0f);
		TM.UpdatePerspective(camera,props,j);
		//cout << "Drawing " << PL[j].EID.size() << " ents at persp level " << j << endl;
		int meshCounter = 0;
        	for (unsigned int l = 0; l < PL[j].EID.size(); ++l)
		{
			int i = PL[j].EID[l];
			int OPCID = OPY.ents[i].OPCID;
			int OID = OPY.ents[i].OBJID;
			//cout << "OPY POS: " << OPY.entpositions[i].x << "," << OPY.entpositions[i].y << ","<< OPY.entpositions[i].z << "\n";
			//cout << "Entity: " << i + 1 << " of " << OPY.ents.size() << " OPCID: " << OPCID << " OID: " << OID << endl;
	
			glm::vec3 cPos = camera.Pos;
	
			glm::vec3 IconPos;
			int Nmesh = OPC[OPCID].ModLib[OPC[OPCID].ObjLib[OID].ModID].Nmesh;
        		for (int k = 0; k < Nmesh; ++k)
        		{
				int MESHID = k;
		
				//Handle Shader Call
				int SID = OPY.ents[i].SHDRID[MESHID];//Shader ID
				SCC.ShdrLib[SID].Use();
	
				GLint Prog = SCC.ShdrLib[SID].Program;
				GLint lightDirLoc = glGetUniformLocation(Prog, "Dlight.direction");
		        	GLint intensityLoc = glGetUniformLocation(Prog, "Dlight.intensity");

		        	glUniform3f(lightDirLoc, Env.sunPos.x, Env.sunPos.y, Env.sunPos.z);
		        	glUniform1f(intensityLoc, Env.intensity);
	
				//Set Lighting
				Lits.LoadLightUniforms(meshCounter,SCC.ShdrLib[SID],OPY);
	
				//Handle Model Position
        			ObjectCoords CoordSet;//Set the objects coordinates.
		
				CoordSet.SetLocs(SCC.ShdrLib[SID]);
        			//THIS CAN BE PUT OFF ONTO A GLOBAL UNIFORM AT SOMEPOINT, DO IT!
				CoordSet.MoveObject(OPY.entpositions[i],OPY.entrotmat_gbl[i],OPY.scale[i]);//Move the object in the scene by updating the tranformation matrix.
				IconPos += OPY.entpositions[i];
	
				OPC[OPCID].DrawObjects(SCC.ShdrLib[SID],OID,MESHID);
				++meshCounter;
			}

			//Now, Draw Icon if there is one
			//glDisable(GL_CULL_FACE);
			if (OPY.ents[i].HasIcon)
				{//IconIdents.push_back(i);}

				IconPos *= 1/(float)Nmesh;
				int MESHID = 0;

	                	//THIS CAN BE PUT OFF ONTO A GLOBAL UNIFORM AT SOMEPOINT, DO IT!
				//glm::vec3 iconNormal = glm::vec3(0.0f,1.0f,0.0f);
				glm::vec3 Position = cPos - IconPos;
				float dist = glm::length(Position);
	
				float IconScale = 1.1f;
				float oscale = OPY.scale[i];
				float cutoff = 100.0 * oscale;
				float scale = oscale * (IconScale + (dist / cutoff));
				if (scale > oscale * IconScale)
					{scale = scale;} 
				else 
					{scale = oscale * IconScale;}

		        	//Handle Shader Call
		        	int SID = OPY.ents[i].ISHDR;//Shader ID
		       	 	SCC.ShdrLib[SID].Use();

				//cout << "ICON SID: "  << SID << endl;
	
			        //Handle Model Position
			        ObjectCoords CoordSet;//Set the objects coordinates.

			        CoordSet.SetLocs(SCC.ShdrLib[SID]);
	
	        	        //glm::vec3 Axis = glm::normalize(glm::cross(iconNormal,Position));
		                //float angle = acos(glm::dot(iconNormal,Position)/dist);
	                	//jsm::quat<float> LocalQuat = jsm::quatGenLocal(angle,Axis);
	                	//glm::mat4 RotMat = jsm::quatUnittoMatrix(LocalQuat);
	                	glm::mat4 RotMat;
	
				//scale = oscale * (scale + (dist / cutoff));
				//scale = oscale * 1.2f;
		        	CoordSet.MoveObject(IconPos,RotMat,scale);//Move the object in the scene by updating the tranformation matrix.

				//Draw Overlay
		        	OPC[OPY.ents[i].IOPCID].DrawObjects(SCC.ShdrLib[SID],OPY.ents[i].IOBJID,1);

				//Draw Grid
		        	OPC[OPY.ents[i].IOPCID].DrawObjects(SCC.ShdrLib[SID],OPY.ents[i].IOBJID,0);
			}
			//glEnable(GL_CULL_FACE);
		}
		//At each perspective level, clear the depth buffer.
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	
	//glDepthRange(1.0f,0.0f);
	//glDepthFunc(GL_LESS);
}
