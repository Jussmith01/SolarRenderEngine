#define GLEW_STATIC
#ifndef GameStructs
#define GameStructs

#include "../mainHeaders.h"
#include "../objectControllers/objectController.h"
#include "../objectControllers/shaderController.h"
//#include "../objectControllers/objectPhysics.h"
//#include "../EngineHeader.h"

using namespace std;

//-------------------------------------------//
//               Properties Class            //
//-------------------------------------------//
struct Properties
{
	string filename;

	std::vector<string> data;
	int lines;

	int ResAuto;
        int WinWidth;
        int WinHeight;
        int MonWidth;//Monitor physical width in mm
        int MonHeight;//Monitor physical height in mm
        int FullScreen;//Fullscreen mode toggle 1 on 0 off
        int MSAA;//MSAA level

	//Text Formattint Data
	int DPmm;//Dots per millimeter
	float FontSize;//A scalar calculated from DPmm
	float LineSpace;//Calculated from DPmm

	void init (void)
	{
		filename = "bin/config.gcf";
		read_gcf();
		set_properties();
	};

        Properties () {};
        ~Properties () {};

	void read_gcf(void);
	void set_properties(void);
	void GetPrimaryResolution(int ResAuto);
	void GetPrimaryMonitorSize(void);
};

//-------------------------------------------//
//               Camera Class                //
//-------------------------------------------//
struct Camera
{
        //CAMERA POS VARIABLES
        GLfloat yaw;
        GLfloat pitch;


        glm::vec3 Pos;
        glm::vec3 PosOld;
        glm::vec3 Dir;
        glm::vec3 Up;

	glm::mat4 VM;
	glm::mat4 PM[3];

        GLfloat lastX;
        GLfloat lastY;

        GLfloat aspect;
        float SPEED_MULTI;

	int WindowWidth;
	int WindowHeight;

	bool CamReset;

        //Camera Initializer
        void init (Properties props)
        {
                //CAMERA POS VARIABLES
		CamReset = false;

                yaw = -90.0f;
                pitch = 0.0f;

                Pos = glm::vec3(0.0f, 0.0f, 0.0f);
                PosOld = glm::vec3(0.0f, 0.0f, 0.0f);
                Dir = glm::vec3(0.0f, 0.0f, -1.0f);
                Up = glm::vec3(0.0f, 1.0f, 0.0f);

                lastX = props.WinWidth / 2, lastY = props.WinHeight / 2;

		WindowWidth=props.WinWidth;
		WindowHeight=props.WinHeight;

                aspect = 45.0;
                SPEED_MULTI = 5.0f;
        };

        Camera() {};
        ~Camera() {};

	void PrepareCameraData ()
	{
		DetermineReset();
		ProduceVPTransMat();
	};

        //**************************
        //Class Peripheral Functions
        //**************************
	//This produces the view/persp matrix at a given depth level.
        void ProduceVPTransMat()
        {
		for (int i=0;i<3;++i)
		{
                	float LowR;
                	float HighR;
                	switch(i)
                	{
                	        /*LARGE */case 0: {LowR=10000.0f;HighR=500000.0f;break;}
                	        /*MEDIUM*/case 1: {LowR=0.5f;HighR=20000.0f;break;}
                	        /*SMALL */case 2: {LowR=1.0E-3f;HighR=3.0f;break;}
                	}
                	//Set projection matrix in the uniform buffer object
                	glm::mat4 proj = glm::perspective(aspect, (float)WindowWidth/(float)WindowHeight, LowR, HighR);
			PM[i] = proj;
		}
		VM = glm::lookAt(Pos, Pos + Dir, Up);
        };

	//******************************************//
	//           Determine Depth Level          //
	//******************************************//
	int DetermineDepthLevel(float Dist)
	{
	        int DLevel;
	        //Determine best depth level for the given distance
	        if (Dist > 12500)                          {DLevel=0;}
	        else if (Dist >= 2.5f && Dist <= 12500.0f) {DLevel=1;}
	        else                                      {DLevel=2;}
	        return DLevel;
	};

	//Determine if origin must be reset
	void DetermineReset()
	{
		if (glm::length(Pos) > 1000.0f)
		{
			cout << "Resetting World Origin...\n";
			CamReset = true;
			PosOld = Pos;
			Pos = glm::vec3(0.0f);
		} else {CamReset = false;}
	};

	//******************************************//
        //               UBO Handling               //
        //******************************************//
        GLuint uboMatrices;

        void SetProjViewUBO(shaderController &SCC)
        {
		//do this for all active shaders	
		for (int i=0;i<(int)SCC.ShdrLib.size();++i)
		{
			GLuint P = SCC.ShdrLib[i].Program;
			GLuint uniformBlockIndex = glGetUniformBlockIndex(P, "Matrices");
			glUniformBlockBinding(P, uniformBlockIndex, 1);
		}

                glGenBuffers(1, &uboMatrices);

                glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
                glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);

                glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboMatrices, 0, 2 * sizeof(glm::mat4));
        };

        void UpdateProjViewUBO(int depthLevel)
        {
                //Bind the UBO
                glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);

                //Set the Projection Matrix in the UBO
                glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(PM[depthLevel]));
                glBindBuffer(GL_UNIFORM_BUFFER, 0);

                //Set the View Matrix in the UBO
                glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
                glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(VM));
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
        };

};

//-------------------------------------------//
//            Input Handling Class           //
//-------------------------------------------//
struct deltaTime
{
        GLfloat t;       // Time between current frame and last frame
        GLfloat lastFrame;       // Time of last frame
	GLfloat mult;
	string tmult;

        //Camera Initializer
        deltaTime()
        {
                t = 0.0f;       // Time between current frame and last frame
                lastFrame = 0.0f;       // Time of last frame
		mult = 1.0f;
        };

        ~deltaTime() {};

        //**************************
        //Class Peripheral Functions
        //**************************
        void SetTime ()
        {
                GLfloat currentFrame = glfwGetTime();
                t = (currentFrame - lastFrame);
                lastFrame = currentFrame;

		stringstream ss;
		ss << "x" << mult;
		tmult = ss.str();
        };

	float GetScaledTime()
		{return t*mult;};

	string GetTimeString ()
	{
		stringstream ss;
		ss << t * 1000 << "ms";
		return ss.str();
	};
};

//-------------------------------------------//
//            Environment Class              //
//-------------------------------------------//
struct Environment
{
        glm::vec3 sunPos;// Suns position vector
        glm::vec3 sunColor;// Currently not set
        glm::vec3 bgColor;// Sets the background color
	GLfloat intensity;

        //
        Environment() 
	{
        	sunPos = glm::vec3(-0.3f,-0.4f,0.3f);
		bgColor = glm::vec3(0.0f,0.0f,0.0f);
		intensity = 1.0f;
	};
        ~Environment() {};
};

//-------------------------------------------//
//               TranMats Class            //
//-------------------------------------------//
struct GeneralGlobalUBO
{
	GLuint uboGlblVars;

	void SetGlobalUBO(shaderController &SCC)
	{
                for (int i=0;i<(int)SCC.ShdrLib.size();++i)
                {
                        GLuint P = SCC.ShdrLib[i].Program;
                        GLuint uniformBlockIndex = glGetUniformBlockIndex(P, "GlobalVars");
                        glUniformBlockBinding(P, uniformBlockIndex, 0);
                }

		glGenBuffers(1, &uboGlblVars);
  
		glBindBuffer(GL_UNIFORM_BUFFER, uboGlblVars);
		glBufferData(GL_UNIFORM_BUFFER, 32, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
  
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboGlblVars, 0, 32);
	};

	void UpdateGlobalUBO(Camera camera)
	{
                //Bind the UBO
                glBindBuffer(GL_UNIFORM_BUFFER, uboGlblVars);

                //Set the total time in the UBO
                GLfloat time = glfwGetTime();
                glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, (const GLvoid*)&time);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);

                //Set the camera position in the UBO
		//cout << "cPos: [" << camera.Pos.x << "," << camera.Pos.y << "," << camera.Pos.z << "]\n";
                glBindBuffer(GL_UNIFORM_BUFFER, uboGlblVars);
                glBufferSubData(GL_UNIFORM_BUFFER, 16, 16, glm::value_ptr(camera.Pos));
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
	};
};

//-------------------------------------------//
//     Point Light Attenuation Class         //
//-------------------------------------------//
struct PointLightAttenuation
{
	//Preset Attenuation ID... Defines the constants below
	int AID;

	//Attenuation Constants
	float c; //Constant Value
        float l; //Linear Value
        float q; //Quadratic Value

        //
        PointLightAttenuation(int AID) {this->AID = AID;SetAttenuation(this->AID);};
        ~PointLightAttenuation() {};

	void ExternSetAtt()
        	{SetAttenuation(AID);};

	void SetAttenuation(int dist)
	{
                switch (dist)
                {
                        case 7: {c = 1.0f; l = 0.7; q = 1.8; break;}
                        case 13: {c = 1.0f; l = 0.35; q = 0.44; break;}
                        case 20: {c = 1.0f; l = 0.22; q = 0.20; break;}
                        case 32: {c = 1.0f; l = 0.14; q = 0.07; break;}
                        case 50: {c = 1.0f; l = 0.09; q = 0.032; break;}
                        case 65: {c = 1.0f; l = 0.07; q = 0.017; break;}
                        case 100: {c = 1.0f; l = 0.04; q = 0.01; break;}
                        case 160: {c = 1.0f; l = 0.027; q = 0.0028; break;}
                        case 200: {c = 1.0f; l = 0.022; q = 0.0019; break;}
                        case 325: {c = 1.0f; l = 0.014; q = 0.0007; break;}
                        case 600: {c = 1.0f; l = 0.007; q = 0.0002; break;}
                        case 3250: {c = 1.0f; l = 0.0014; q = 0.000007; break;}
                        case 0: {c = 1.0f; l = 0.0f; q = 0.0f; break;}
                        default: {c = 1.0f; l = 0.014; q = 0.0007; break;}
                }
	};
};

//-------------------------------------------//
//            Lighting Class                 //
//-------------------------------------------//
/*struct MeshIdent
{
	int OPCID;
	int OBJID;
	int MESHID;
	int EID;

	MeshIdent (int OPCID) {this->OPCID = OPCID;};
};

struct Lighting
{
	int MaxPointLights; //Maximum number of lights that act on a single mesh.
	vector<MeshIdent> MeshRegIdx; //Mesh index for non-lighting meshes. These meshs are lit by the lighting meshes.
	vector<MeshIdent> MeshLiteIdx; //Lighting Index for meshs which light the scene. These meshes create light.
	vector<glm::vec3> LiteMeshPos; //Position vector of the light mesh.
	vector<glm::vec3> LiteMeshGlobPos; //Position vector of the light mesh.
	vector<PointLightAttenuation> PLA; //Class stores the point light attenuation variables

	vector<int> LiteToMeshIdx; //Matches the Reg IDX to the [MAXLIGHTS] closest lights, idx strided by MaxPointLights.
	vector<int> MIMap; //Maps MeshRegIdx to the EID index

        //
        Lighting() {};
        ~Lighting() {};

	//-------------------------------------------//
	//     Set New Ent int as Light or Reg       //
	//-------------------------------------------//
	void SetNewEntity (int EID,int OPCID,int OBJID,glm::vec3 objpos,vector<objPropController> &OPC)
	{
		int Nmesh = OPC[OPCID].ObjLib[OBJID].Nmesh;
		for(int i = 0; i < Nmesh; ++i)
		{
			int MESHID = i;
			string lighting = OPC[OPCID].ObjLib[OBJID].lighting[MESHID];
			if (lighting.compare("no") == 0)
			{
				InitRegMesh(EID,OPCID,OBJID,MESHID);
			}
			else if (lighting.compare("point") == 0)
			{
				InitLightMesh(EID,OPCID,OBJID,MESHID,objpos,OPC);
				PLA.push_back(100);
			}
                        else if (lighting.compare("icon") == 0)
                        {
				//Do Nothing for now
			} else {cout << "Error: " << lighting << "is not a valid lighting type.";}
		}
	};

        //-------------------------------------------//
        //           Initialize Reg Mesh             //
        //-------------------------------------------//
        void InitRegMesh(int EID,int OPCID,int OBJID,int MESHID)
        {
                //Set the mesh index
                int N = MeshRegIdx.size();
                MeshRegIdx.push_back(OPCID);
                MeshRegIdx[N].OBJID = OBJID;
                MeshRegIdx[N].MESHID = MESHID;
                MeshRegIdx[N].EID = EID;
		MIMap.push_back(N);
        };

        //-------------------------------------------//
        //          Initialize Light Mesh            //
        //-------------------------------------------//
	void InitLightMesh(int EID,int OPCID,int OBJID,int MESHID,glm::vec3 objpos,vector<objPropController> &OPC)
        {
		//Set the mesh index
		int N = MeshLiteIdx.size();
		MeshLiteIdx.push_back(OPCID);
		MeshLiteIdx[N].OBJID = OBJID;
		MeshLiteIdx[N].MESHID = MESHID;
                MeshLiteIdx[N].EID = EID;
		MIMap.push_back(-1);

		int MID = OPC[OPCID].ObjLib[OBJID].ModID;

		int Nverts = (int)OPC[OPCID].ModLib[MID].mesh[MESHID].vertices.size();
		
		glm::vec3 sumvec;
		for (int j = 0; j < Nverts; ++j)
		{
			sumvec += OPC[OPCID].ModLib[MID].mesh[MESHID].vertices[j].position;
		}
		sumvec *= (1/(float)Nverts);

		LiteMeshGlobPos.push_back(sumvec);
		LiteMeshPos.push_back(sumvec);
	};

        //-------------------------------------------//
        //          Update Light Positions            //
        //-------------------------------------------//
        void UpdateLightPos(objPhys &OPY)
        {
		for (int i = 0;i<(int)MeshLiteIdx.size();++i)
		{
			int EID = MeshLiteIdx[i].EID;
                	glm::vec3 entposL = OPY.entpositions[EID];
                	jsm::quat<float> entquat_gblL = OPY.entquat_gbl[EID];
                	glm::vec3 lightPos = jsm::quatRotVec3( entquat_gblL,LiteMeshPos[i] ) + entposL;

                	LiteMeshGlobPos[i] = lightPos;
		}
        };

        //-------------------------------------------//
        //          Determine Mesh Lights            //
        //-------------------------------------------//
	void DetermineMeshLights(objPhys &OPY)
	{
	if(!((int)MeshRegIdx.size() == 0) && !((int)MeshLiteIdx.size() == 0))
	{
		UpdateLightPos(OPY);
		LiteToMeshIdx.clear();
		for (int i = 0; i < (int)MeshRegIdx.size(); ++i)//For each regular non-light mesh
		{
			int *LITIDX = new int [MaxPointLights];
			float *DISTIDX = new float [MaxPointLights];

			for (int j = 0; j < MaxPointLights;++j)
				{LITIDX[j] = -1;DISTIDX[j] = 1.0E+6;}

			int EIDR = MeshRegIdx[i].EID;
			glm::vec3 entposR = OPY.entpositions[EIDR];

			for (int j = 0; j < (int)MeshLiteIdx.size(); ++j)//Search through each lighting mesh
			{
				//int EIDL = MeshLiteIdx[j].EID;
				glm::vec3 meshpos = LiteMeshGlobPos[j];

				glm::vec3 diffvec = entposR - meshpos;
				float dist = glm::length(diffvec);

				//cout << "MeshRegIdx: " << EIDR << " MeshLiteIdx: " << EIDL << " PosVec: " << diffvec.x << "," << diffvec.y << "," << diffvec.z << " DIST: " << dist << endl;

				for (int k = 0; k < MaxPointLights; ++k)
				{
					if (DISTIDX[k] > dist)
					{
						for (int l = 0; l < MaxPointLights - 1 - k; ++l)
						{
							DISTIDX[MaxPointLights - 1 - l] = DISTIDX[MaxPointLights - 2 - l];
							LITIDX[MaxPointLights - 1 - l] = LITIDX[MaxPointLights - 2 - l];
						}
						DISTIDX[k] = dist;
						LITIDX[k] = j;
						break;
					}
				}
			}

                        for (int j = 0; j < MaxPointLights;++j)
                                {LiteToMeshIdx.push_back(LITIDX[j]);}

			delete [] LITIDX;
			delete [] DISTIDX;
		}
	
		cout << "Light to Mesh index: \n";
                for (int i = 0; i < (int)LiteToMeshIdx.size();++i)
		{
			if (i % MaxPointLights == 0) {cout << endl;}
			if (i % MaxPointLights == 0) {cout << "  ";}
			cout << LiteToMeshIdx[i] << " | ";
		}
		cout << endl;
	}
	};

        //-------------------------------------------//
        //          Load Lighting Uniforms           //
        //-------------------------------------------//
	void LoadLightUniforms(int MID,Shader &shader,objPhys &OPY)
	{
        if(!((int)MeshRegIdx.size() == 0) && !((int)MeshLiteIdx.size() == 0))
        {
		int MIDX = MIMap[MID];//MESH INDEX
		if (!(MIDX == -1))
		{
			GLuint Prog = shader.Program;
        	        for (int i = 0; i < MaxPointLights; ++i)
        	        {
				int EIDL = LiteToMeshIdx[MaxPointLights * MIDX + i];
				//cout << "MIDX: " << MIDX << " EIDL: " << EIDL << endl;
				if (!(EIDL == -1))
				{
					//cout << "MID: " << MID  << " MIDX: " << MIDX << " EIDL: " << EIDL << endl;

		                        stringstream lps,cs,ls,qs;
		                        lps << "Plight[" << i << "].lightPos";
		                        cs << "Plight[" << i << "].constant";
		                        ls << "Plight[" << i << "].linear";
		                        qs << "Plight[" << i << "].quadratic";
	
	                        	GLuint PlightPosLoc = glGetUniformLocation(Prog, lps.str().c_str());
	                        	GLuint PcLoc = glGetUniformLocation(Prog, cs.str().c_str());
	                        	GLuint PlLoc = glGetUniformLocation(Prog, ls.str().c_str());
	                        	GLuint PqLoc = glGetUniformLocation(Prog, qs.str().c_str());

					//cout << "c: " << PLA[EIDL].c << " l: " << PLA[EIDL].l << " q: " << PLA[EIDL].q << endl;

					glUniform1f(PcLoc, PLA[EIDL].c);
	                        	glUniform1f(PlLoc, PLA[EIDL].l);
	                        	glUniform1f(PqLoc, PLA[EIDL].q);
	
        	                	glm::vec3 lightPos = LiteMeshGlobPos[EIDL];
	
                	        	glUniform3f(PlightPosLoc, lightPos.x, lightPos.y, lightPos.z);
				}
                	}
		}
	};
	}
};*/

//-------------------------------------------//
//           Stores Console Data             //
//-------------------------------------------//
struct MenuData
{
	int totlines;
	vector<string> setentlines;
        vector<int> varsperline;
};

struct MenuClass
{
	MenuData MD;
	int totvar;
        int cursorpos;

	string *data;

	void init_menu(MenuData MD)
	{
		this->MD = MD;
		int Nl = this->MD.totlines;
		totvar = 0;
		for (int i = 0; i < Nl; ++i)
			{totvar += this->MD.varsperline[i];}
		cursorpos = 0;
		data = new string [totvar];
	};

        //Produces string for drawing
        string createline(int linenum,string cursor)
        {
                string rtnstr = MD.setentlines[linenum];

                int prevvars = 0;
                size_t tpos;
                size_t totpos = 0;
                for (int i = 0;i < linenum;++i)
                {
                        tpos = 0;
                        totpos = 0;
                        while(true)
                        {
                                //cout << setentlines[i].substr(tpos) << " " << tpos << endl;
                                totpos += tpos;
                                tpos = MD.setentlines[i].substr(totpos).find("[");
                                //cout << setentlines[i].substr(tpos) << " " << tpos << endl;
                                if(tpos != string::npos)
                                        {++prevvars;++tpos;}
                                else {break;}
                        }
                }

                tpos = 0;
                totpos = 0;
                for (int i = 0;i < MD.varsperline[linenum];++i)
                {
                        tpos = rtnstr.substr(totpos + 1).find("[");
                        totpos += tpos + 1;

                        if (prevvars + i == cursorpos)
                        {
                                //cout << "TEST\n";
                                rtnstr.insert(totpos + 1,data[prevvars + i] + cursor);
                        } else {
                                rtnstr.insert(totpos + 1,data[prevvars + i]);
                        }
                }

                return rtnstr;
        };

	void destroy_menu()
	{
		cursorpos = 0;
		//this->MD = NULL;
		delete [] data;data = NULL;
	};
};

struct ConsoleData
{
        string cursor; float ctimer; bool Enter;
	MenuClass MC;

        //Stores the consoles history, written to screen
        string consolehistory[50];
        string commandhistory[20];
        int chpos;

        //Register Console Function
        bool Console_Call;

        //*********************************
        //	Menu Creation Function
        //*********************************
	bool Menu_Call;

	//Menu for setting new entity
        bool SetEntity_Call;
	MenuData EntMD;
        void InitializeEntMenu()
        {
                EntMD.setentlines.push_back("Name[] ");EntMD.varsperline.push_back(1);
                EntMD.setentlines.push_back("OPCID[] OBJID[] SCALE[]");EntMD.varsperline.push_back(3);
                EntMD.setentlines.push_back("POS[],[],[] - ROT[],[],[],[]");EntMD.varsperline.push_back(7);
                EntMD.setentlines.push_back("Is Dynamic?(y=1/n=0):[] GravityInteration?(y=1/n=0):[] Mass:[]");EntMD.varsperline.push_back(3);
                EntMD.setentlines.push_back("VEL[],[],[] vROT[],[],[],[]");EntMD.varsperline.push_back(7);
		EntMD.totlines=5;
                SetEntity_Call = false;
        };

        //Menu for modding entity data
        bool SetEntityMod_Call;
        MenuData EntModMD;
        void InitializeEntModMenu()
        {
                EntModMD.setentlines.push_back("EntityID []");EntModMD.varsperline.push_back(1);
                EntModMD.setentlines.push_back("POS[],[],[] - ROT[],[],[],[] SCALE[]");EntModMD.varsperline.push_back(8);
                EntModMD.setentlines.push_back("Reset Rotation? (y/n) []");EntModMD.varsperline.push_back(1);
                EntModMD.setentlines.push_back("Is Dynamic?(y=1/n=0):[] GravityInteration?(y=1/n=0):[] Mass:[]");EntModMD.varsperline.push_back(3);
                EntModMD.setentlines.push_back("VEL[],[],[] vROT[],[],[],[]");EntModMD.varsperline.push_back(7);
                EntModMD.totlines=5;
                SetEntityMod_Call = false;
        };

        //Menu for modding environment data
        bool SetEnv_Call;
        MenuData EnvModMD;
        void InitializeEnvModMenu()
        {
                EnvModMD.setentlines.push_back("SunPos[],[],[]");EnvModMD.varsperline.push_back(3);
                EnvModMD.setentlines.push_back("SunColor[],[],[] - SunIntensity[]");EnvModMD.varsperline.push_back(4);
                EnvModMD.setentlines.push_back("BackgroundColor[],[],[]");EnvModMD.varsperline.push_back(3);
                EnvModMD.totlines=3;
                SetEnv_Call = false;
        };

        //Menu for modding plight data
        bool SetPlight_Call;
        MenuData PlightModMD;
        void InitializePlightModMenu()
        {
                PlightModMD.setentlines.push_back("LightID[] Attenuation[]");PlightModMD.varsperline.push_back(2);
                PlightModMD.setentlines.push_back("Color[],[],[]");PlightModMD.varsperline.push_back(3);
                PlightModMD.totlines=2;
                SetPlight_Call = false;
        };

        //Menu for modding plight data
        bool SetGrav_Call;
        MenuData GravModMD;
        void InitializeGravModMenu()
        {
                GravModMD.setentlines.push_back("GravEntID1[] GravEntID2[]");GravModMD.varsperline.push_back(2);
                GravModMD.setentlines.push_back("Radius[]; All orbits set to x,z Plane");GravModMD.varsperline.push_back(1);
                GravModMD.totlines=2;
                SetGrav_Call = false;
        };

	//******************************//
	//    BASIC STRUCT FUNCTIONS	//
	//*****************************//
        //Stores the current command line
        string command;
        //Detect Shift
        bool shift;
        bool caps;
        bool iptreq; //Input Requested

        ConsoleData()//Initializes a bunch of needed variables 
        {
		//Predfined Variables
                shift = false;caps = false;Enter=false;
                Console_Call = true;
		Menu_Call = false;
                chpos = -1; ctimer = 0;
                cursor = "|";

		//Initialize Menus
		InitializeEntMenu();
		InitializeEntModMenu();
		InitializeEnvModMenu();
		InitializePlightModMenu();
		InitializeGravModMenu();
        };

        void Buffer(string line)
        {
                for (int i = 0; i < 49; ++i)
                        {consolehistory[49 - i] = consolehistory[48 - i];}
                consolehistory[0] = line;
        };

        void CommandBuffer(string line)
        {
                for (int i = 0; i < 19; ++i)
                        {commandhistory[19 - i] = commandhistory[18 - i];}
                commandhistory[0] = line;
        };

        void CursorBlink(float dt)
        {
                ctimer += dt;
                if (ctimer > 0.5 && ctimer < 1.0)
                        {cursor=" ";}
                else if (ctimer >= 1.0)
                        {cursor="|";ctimer=0;}
        };
};

#endif 
