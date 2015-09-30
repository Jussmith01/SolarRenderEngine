#define GLEW_STATIC

//#include "../mainHeaders.h"
#include "../EngineHeader.h"
//#include "../objectControllers/objectPhysics.h"
//#include "../objectControllers/object_coords.h"

using namespace std;

//-------------------------------------------//
//           Process the Draw Array          //
//-------------------------------------------//
void Engine_Init::ProcessDrawArray()
{
	//For each depth, draw array then clear
	//depth buffer.

        //cout << "D1: " << drawArray.ND1 << " D2: " << drawArray.ND2  << " D3: " << drawArray.ND3 << "\n";
	//cout << "DRAWING D1\n";
	//D3 is the furthest entities
	glClear(GL_DEPTH_BUFFER_BIT);
	camera.UpdateProjViewUBO(0);
	ProcessDrawStructs(drawArray.DrawDataD1,drawArray.ND1);
	//ProcessDrawStructs(drawArray.DrawDataD1);
	glClear(GL_DEPTH_BUFFER_BIT);

	//cout << "DRAWING D2\n";
	//D3 is the middle range entities
	camera.UpdateProjViewUBO(1);
	ProcessInstancedDrawStructs(drawInstArray.DrawInstDataD2);
	ProcessDrawStructs(drawArray.DrawDataD2,drawArray.ND2);
	//ProcessDrawStructs(drawArray.DrawDataD2);
	glClear(GL_DEPTH_BUFFER_BIT);

	//cout << "DRAWING D3\n";
	//D1 is the closest entities
	camera.UpdateProjViewUBO(2);
	ProcessDrawStructs(drawArray.DrawDataD3,drawArray.ND3);
	//ProcessDrawStructs(drawArray.DrawDataD3);
	glClear(GL_DEPTH_BUFFER_BIT);
};

//-------------------------------------------//
//           Process the Draw Array          //
//-------------------------------------------//
void Engine_Init::ProcessDrawStructs(DrawStruct *DrawData,int Size)
{
	int N = Size;
	//int N = DrawData.size();
        for (int i=0;i<N;++i)
        {
		//cout << "  Drawing Item: " << i << " of " << N << endl;
		//cout << "  OPCID: " << DrawData[i].OPCID << " OBJID: " << DrawData[i].OBJID << endl;
		int OPCID = DrawData[i].OPCID;
		int OBJID = DrawData[i].OBJID;
		//glm::mat4 TransMat = DrawData[i].TransMat;
		glm::mat4 ModelMat = DrawData[i].ModelMat;
		glm::vec3 objDir = DrawData[i].objPos;
		int Nmesh = OPC[OPCID].ModLib[OPC[OPCID].ObjLib[OBJID].ModID].Nmesh;
		int meshCounter=0;
        	for (int j=0;j<Nmesh;++j)
		{ 
			//cout << "     Drawing Mesh: " << j << " of " << Nmesh << endl;
                	int MESHID = j;

                        //Handle Shader Call
                        int SID = OPC[OPCID].ObjLib[OBJID].Shader[j];//Shader ID
                        SCC.ShdrLib[SID].Use();
                        GLint Prog = SCC.ShdrLib[SID].Program;

			//GLuint GV_index = glGetUniformBlockIndex(Prog, "GlobalVars");
			//GLuint MAT_index = glGetUniformBlockIndex(Prog, "Matrices");
			//cout << "GVI: " << GV_index << " MI: " << MAT_index << endl;

			//glUniformBlockBinding(Prog, GV_index, 0);
			//glUniformBlockBinding(Prog, MAT_index, 1);

			//Upload Sun Data
			GLint lightDirLoc = glGetUniformLocation(Prog, "Dlight.direction");
                        GLint intensityLoc = glGetUniformLocation(Prog, "Dlight.intensity");

                        glUniform3f(lightDirLoc, objDir.x,objDir.y,objDir.z);
                        glUniform1f(intensityLoc, 1.0f);

                        //Handle TransMat Upload
                        GLint MONLoc = glGetUniformLocation(Prog, "material.offsetNormalization");
                        glUniform1f(MONLoc, offsetNormalization);

                        //Handle ModelMat Upload
                	GLint modelmatLoc = glGetUniformLocation(Prog, "modelmat");
			glUniformMatrix4fv(modelmatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));

			//cout << "UniLocs: " << lightDirLoc << "," << intensityLoc << "," << transmatLoc << "," << modelmatLoc << endl;

			//Handle Object Drawing
                        OPC[OPCID].DrawObjects(SCC.ShdrLib[SID],OBJID,MESHID);
                        ++meshCounter;	
			//cout << "      FINISHED\n";
		}
        }
};

//-------------------------------------------//
//           Process the Draw Array          //
//-------------------------------------------//
void Engine_Init::ProcessInstancedDrawStructs(vector<DrawInstancedStruct> &DrawInstData)
{
        int N = DrawInstData.size();
        //int N = DrawData.size();
        for (int i=0;i<N;++i)
        {
                //cout << "  Drawing Item: " << i << " of " << N << endl;
                //cout << "  OPCID: " << DrawData[i].OPCID << " OBJID: " << DrawData[i].OBJID << endl;
                int OPCID = DrawInstData[i].OPCID;
                int OBJID = DrawInstData[i].OBJID;
                int NumInst = DrawInstData[i].N;
		glm::vec3 SunDir = DrawInstData[i].SunDirection;
		//cout << "OPCID/OBJID: " << OPCID << "/" << OBJID << " NumInst: " << NumInst << " SunDir=[" << SunDir.x << "," << SunDir.y << "," << SunDir.z << "]" << endl;
                int Nmesh = OPC[OPCID].ModLib[OPC[OPCID].ObjLib[OBJID].ModID].Nmesh;
                int meshCounter=0;
                for (int j=0;j<Nmesh;++j)
                {
                        //cout << "     Drawing Mesh: " << j << " of " << Nmesh << endl;
                        int MESHID = j;

                        //Handle Shader Call
                        int SID = OPC[OPCID].ObjLib[OBJID].Shader[j];//Shader ID
                        SCC.ShdrLib[SID].Use();
                        GLint Prog = SCC.ShdrLib[SID].Program;

                        //GLuint GV_index = glGetUniformBlockIndex(Prog, "GlobalVars");
                        //GLuint MAT_index = glGetUniformBlockIndex(Prog, "Matrices");
                        //cout << "GVI: " << GV_index << " MI: " << MAT_index << endl;

                        //glUniformBlockBinding(Prog, GV_index, 0);
                        //glUniformBlockBinding(Prog, MAT_index, 1);

                        //Upload Sun Data
			//NOTE: THIS NOW REPRESENTS THE SUN POSITION RATHER THAN THE DIRECTION
                        GLint lightDirLoc = glGetUniformLocation(Prog, "Dlight.direction");
                        GLint intensityLoc = glGetUniformLocation(Prog, "Dlight.intensity");

                        glUniform3f(lightDirLoc, SunDir.x,SunDir.y,SunDir.z);
                        glUniform1f(intensityLoc, 1.0f);

                        //Handle TransMat Upload
                        GLint MONLoc = glGetUniformLocation(Prog, "material.offsetNormalization");
                        glUniform1f(MONLoc, offsetNormalization);

                        //cout << "UniLocs: " << lightDirLoc << "," << intensityLoc << "," << transmatLoc << "," << modelmatLoc << endl;

                        //Handle Object Drawing
                        OPC[OPCID].DrawInstancedObjects(SCC.ShdrLib[SID],OBJID,MESHID,NumInst);
                        ++meshCounter;
                        //cout << "      FINISHED\n";
                }
        }
};

