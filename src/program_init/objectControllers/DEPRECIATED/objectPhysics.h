#define GLEW_STATIC
#ifndef PhysClass
#define PhysClass

#include "../mainHeaders.h"
#include "objectController.h"
#include "shaderController.h"
//#include "../basicStructures/gameStructs.h"

//class objPropController;

using namespace std;
struct entityProps
{
	string NAME;
	int OPCID;
	int OBJID;
	float scale;
	glm::vec3 pos;
	glm::vec3 vel;

	//Rotational Information
	glm::vec3 entqvaxis;
	glm::vec3 entqgaxis;

	float entrv;
	float enta;

	//Physics Information
	int dyn;
	int grav;
	float MASS;

        //Icon Information
        bool HasIcon;
        int IOPCID;
        int IOBJID;

	entityProps() {HasIcon=false;};
};

struct entity
{
	string NAME;
	int OPCID;
	int OBJID;
	vector<int> SHDRID;
	int DYN;
	int GRV;
	int dynidx;
	int gravidx;
	
	//Icon Information
	bool HasIcon;
	int IOPCID;
	int IOBJID;
	int ISHDR;

	entity(string NAME) {this->NAME=NAME;HasIcon=false;};
	~entity() {SHDRID.clear();};
};

struct orbit
{
	int GIDHeavy;	
	int GIDLight;

	orbit (int GIDLight) {this->GIDLight=GIDLight;};
};

//------------------------------------------
//              Object Class
//------------------------------------------
struct objPhys
{
	vector<entity> ents;//There are equal ents and obj
	vector<float> scale;//Holds the scales of entities
	vector<int> dynamicents;//Holds the index of the dynamic objects
	vector<float> entsmass;//Holds the index of the mass of entities
	
        //-----------------------------------------------------
        //                 Gravitational Data
        //-----------------------------------------------------
	vector<int> gravityents;//Holds the index of the gravitationally active entities
	vector<int> gravityToDynIdx;//Holds the index of the gravitationally active entities
	vector<orbit> orbits;//Holds the orbits in the scene


        //-----------------------------------------------------
        //                 Translational Data
        //-----------------------------------------------------
	//Tracks the centers position of the object in space
        vector<glm::vec3> entpositions;

	//Contains the centers translational velocity vector of all dynamic entities
        vector<glm::vec3> entvelocities;
	
	//-----------------------------------------------------
	//		     Rotational Data
	//-----------------------------------------------------
	//Successive application of quat_local to quat_global 
	//each frame gives a rotational effect. So, here, 
	//quat_local can be thought of as a rotational velocity 
	//w/ rotational axis. The local quat can be produced 
	//with the function: 
	//quat_local = jsm::quatGenLocal( T a,glm::vec3 v ) 
	//where a is the angles through which you wish to 
	//rotate and v is the axis of rotation.
	//----------------------------------------------------
	vector<glm::vec3> entqvaxis; 
	vector<float> entrv; //Contains the axis and angular velocity of the rotation
        vector<jsm::quat<float>> entquat_gbl; //Tracks the global rotational quaternion of the object
        vector<glm::mat4> entrotmat_gbl; //Tracks the global rotation of the object

	objPhys() {};

        //-----------------------------------------------------
        //               Create Entity Function
        //-----------------------------------------------------
	int CreateEntity(vector<objPropController> &OPC,shaderController &SCC,entityProps EP)
	{
		float DegtoRad = 0.0174532925;

		cout << "\nCREATING NEW ENTITY:\n";
		cout << "NAME: " << EP.NAME << " OPCID: " << EP.OPCID << " OBJID: " << EP.OBJID << " DYNAMIC: " << EP.dyn << endl;
		cout << "POS = " << EP.pos.x << "," << EP.pos.y << "," << EP.pos.z << " SCALE: " << EP.scale <<endl;
		cout << "QGAxis = " << EP.entqgaxis.x << "," << EP.entqgaxis.y << "," << EP.entqgaxis.z << " a: " << EP.enta << endl;

		ents.push_back(EP.NAME); //Set ents name
		size_t EID = ents.size() - 1; // Entity ID
		ents[EID].OPCID = EP.OPCID; //Object props controller ID
		ents[EID].OBJID = EP.OBJID; // Object ID (within the OPC)
		ents[EID].DYN = 0;ents[EID].GRV = 0;
		ents[EID].HasIcon = EP.HasIcon;
		scale.push_back(EP.scale);

		//Load shaders and textures to GPU for each mesh
		for (int i = 0; i < OPC[EP.OPCID].ObjLib[EP.OBJID].Nmesh; ++i)
		{
			int ushdr = OPC[EP.OPCID].ObjLib[EP.OBJID].MeshLitType[i];
			ents[EID].SHDRID.push_back(SCC.SetShaderOnDevice(ushdr));

			OPC[EP.OPCID].TexrLib[OPC[EP.OPCID].ObjLib[EP.OBJID].MeshTexID[i]].LoadTextureDataToGPU();
		}

		if (ents[EID].HasIcon)
		{
			cout << "Using Icon OPCID(" << EP.IOPCID << ") OBJID(" << EP.IOBJID << ")\n";
			ents[EID].IOPCID = EP.IOPCID; //Object props controller ID
                	ents[EID].IOBJID = EP.IOBJID; // Object ID (within the OPC)

			int ishdr = OPC[EP.IOPCID].ObjLib[EP.IOBJID].MeshLitType[0];
                        ents[EID].ISHDR = SCC.SetShaderOnDevice(ishdr);

			
			for (int i = 0; i < OPC[EP.IOPCID].ObjLib[EP.IOBJID].Nmesh; ++i)
			{
				OPC[EP.IOPCID].TexrLib[OPC[EP.IOPCID].ObjLib[EP.IOBJID].MeshTexID[i]].LoadTextureDataToGPU();
				//OPC[EP.IOPCID].TexrLib[OPC[EP.IOPCID].ObjLib[EP.IOBJID].MeshTexID[i]].TextureDeleteFromGPU();
				//OPC[EP.IOPCID].TexrLib[OPC[EP.IOPCID].ObjLib[EP.IOBJID].MeshTexID[i]].LoadTextureDataToGPU();
			}
		}

		//If object is dynamic velocity and rotation velocity info is ignored
		//Here we set the Entity ID in the dynamic entity vector for later use
		if (EP.dyn == 1)
		{
			ents[EID].DYN = 1;
			ents[EID].dynidx = (int)entvelocities.size();
			dynamicents.push_back(EID);//Set the int as dynamic
			if (EP.grav == 1)
			{
				ents[EID].GRV = 1;
				gravityents.push_back(EID);
				gravityToDynIdx.push_back((int)dynamicents.size() - 1);
				ents[EID].gravidx = gravityents.size()-1; 
			}
			entsmass.push_back(EP.MASS);
			entvelocities.push_back(EP.vel);//Set the velocity
                	entqvaxis.push_back(glm::normalize(EP.entqvaxis));//Set the qv rotational axis
                	entrv.push_back(EP.entrv * DegtoRad);//set the rotational velocity
			cout << "VEL = " << EP.vel.x << "," << EP.vel.y << "," << EP.vel.z << endl;
			cout << "QVAxis = " << EP.entqvaxis.x << "," << EP.entqvaxis.y << "," << EP.entqvaxis.z << " rv: " << EP.entrv << endl;
		}

		entpositions.push_back(EP.pos);

		jsm::quat<float> quatinit = jsm::quatGenLocal( EP.enta * DegtoRad,glm::normalize(EP.entqgaxis) );
		glm::mat4 rotmatinit = quatUnittoMatrix(quatinit);
		entrotmat_gbl.push_back(rotmatinit);

		jsm::quat<float> quatinit2 = jsm::quatGenLocal( EP.enta * DegtoRad,glm::normalize(EP.entqgaxis) );
		entquat_gbl.push_back(quatinit2);
		cout << "ENTITY(" << EID << ") CREATED...\n";
		return EID;
	};

	//-----------------------------------------------------
        //            Functions for setting Quaternions
        //-----------------------------------------------------
        //Arg 1: (qlaxis) The axis of rotation
	//Arg 2: (rv) Rotational Velocity (rad/s)
        //----------------------------------------------------
	/*void RotateQuaternion(float dt)
	{
		jsm::quat<float> quat_tmp1 = jsm::quatGenLocal( rv * dt,qvaxis );
		quat_vel = quat_tmp1;
		jsm::quat<float> quat_tmp2 = quat_vel * quat_gbl;
		quat_gbl = quat_tmp2;
	};

        //----------------------------------------------------
        //Arg 1: (qgaxis) The axis of rotation
        //Arg 2: (a) Number of Radians to rotate (rad/s)
        //----------------------------------------------------
        void SetQuatGlobal(glm::vec3 qgaxis,float a)
        {
		jsm::quat<float> quat_tmp = jsm::quatGenLocal( a,qgaxis );
		quat_gbl = quat_tmp; 
	};*/
};

struct meshPhys
{
	//For now we only track the position vector of each mesh
	//To get this initial vector we need to determine it from
	//the starting verticies. This needs to be done only one,
	//The the vector can always be determined by the overall
	//objects position and quat_global.
	//glm::vec3 position;
	//glm::vec3 velocity;
	//jsm::quat quaternion;
};

class Physics
{
	bool RunPhys;
	float G;
	float DegtoRad;
	int OrbitMaintCnt;
	int numThreads;

	public:
	Physics() 
	{
		RunPhys=true;
		G=0.01;
		DegtoRad = 0.0174532925;
		OrbitMaintCnt=0;

		//numThreads = omp_get_num_threads();
	};

	void PausePhysics () {RunPhys=false;};
	void UnpausePhysics () {RunPhys=true;};

        //------------------------------------------//
        //           Detect Physics Bool            //
        //------------------------------------------//
	void DetPhysics(int chk)
	{
		if (chk == 0)
			{RunPhys=true;}
		else
			{RunPhys=false;}
	}

        //------------------------------------------//
        //           Main Physics Function          //
        //------------------------------------------//
	void RunPhysics (float dt,float mult,objPhys &OPY,int consoleReq)
	{
		DetPhysics(consoleReq);
		if (RunPhys)
		{
			if (mult <= 1.0f)
			{
				float scaledtime = dt*mult;
				CalculateGravitationEffects(scaledtime,OPY);
				CalculatePositions(scaledtime,OPY);
        			CalculateRotations (scaledtime,OPY);
			} else if (mult > 1.0f) {
				int NUMRUN = (int)mult;
				for (int i=0;i<NUMRUN;++i)
				{
					CalculateGravitationEffects(dt,OPY);
                                	CalculatePositions(dt,OPY);
                                	CalculateRotations (dt,OPY);
				}
			}
		}
	};

        //------------------------------------------//
        //      Calculate Gravitation Effects       //
        //------------------------------------------//
        void CalculateGravitationEffects(float dt,objPhys &OPY)
        {
		if (OPY.gravityents.size() >=2)
		{
			int N = (int)OPY.orbits.size();
			//Add multi threading here

			#pragma omp parallel for
        		for (int i=0;i<N;++i)
        		{
				int GIDi = OPY.orbits[i].GIDLight;
				int GIDj = OPY.orbits[i].GIDHeavy;
				
        		        int EIDi = OPY.gravityents[GIDi];
        		        int EIDj = OPY.gravityents[GIDj];
				int GTDID = OPY.gravityToDynIdx[GIDi];
				float MASSi = OPY.entsmass[GIDi];
				
				glm::vec3 F = glm::vec3(0);
				glm::vec3 R = OPY.entpositions[EIDj] - OPY.entpositions[EIDi];
				float MagR = glm::length(R);
				R = glm::normalize(R);
				if (MagR > 0.1)
				{
				    glm::vec3 PFv = (G*MASSi*OPY.entsmass[GIDj] * R)/(MagR * MagR);
				    //cout << "FORCES: " << OPY.ents[EIDi].NAME << " -> " << OPY.ents[EIDj].NAME << " Mag: " << glm::length(PFv)/MASSi << endl;
				    F = PFv;
				}
	
        		        OPY.entvelocities[GTDID] = OPY.entvelocities[GTDID] +  F * (dt / MASSi);
                	}

			if (OrbitMaintCnt==10) {OrbitMaint(OPY);OrbitMaintCnt=0;}
			++OrbitMaintCnt;
		}
        };

	//------------------------------------------//
	//           Set Orbit Function		    //
	//------------------------------------------//
	void OrbitMaint(objPhys &OPY)
	{
                int N = (int)OPY.orbits.size();

		float fG = this->G;
		#pragma omp parallel for shared(fG)
                for (int i=0;i<N;++i)
                {
	                int GID1 = OPY.orbits[i].GIDLight;
                        int GID2 = OPY.orbits[i].GIDHeavy;

			int EID1 = OPY.gravityents[GID1];
			int EID2 = OPY.gravityents[GID2];
			int GTDID1 = OPY.gravityToDynIdx[GID1];
			int GTDID2 = OPY.gravityToDynIdx[GID2];
			//float M1 = OPY.entsmass[GID1];
			float M2 = OPY.entsmass[GID2];

                        glm::vec3 OrbitAxis(0.0f,1.0f,0.0f);
                        glm::vec3 PVecL = OPY.entpositions[EID1];
                        glm::vec3 PVecH = OPY.entpositions[EID2];
                        glm::vec3 VVecL = glm::cross(OrbitAxis,PVecL-PVecH);
                        glm::vec3 VVecH = OPY.entvelocities[GTDID2];

			glm::vec3 VelVec = glm::normalize(VVecL);
			float MagR = glm::length(PVecL-PVecH);
	
			//float Fg = (G*M1*M2)/(MagR * MagR);
			//float MagV = sqrt((Fg * MagR) / M1);
		        float MagV = sqrt((fG * M2) / MagR);
        		VelVec = MagV * VelVec;

        		//cout << "GID = " << GID1 << " VelVec = [" << VelVec.x << "," << VelVec.y << "," << VelVec.z <<"] Radius: " << MagR << "\n";
        		OPY.entvelocities[GTDID1] = VelVec + VVecH;
		}
	};

        //------------------------------------------//
        //      Apply Velocities to Positions       //
        //------------------------------------------//
	void CalculatePositions(float dt,objPhys &OPY)
	{
		#pragma omp parallel for shared(dt)
		for (int i=0;i<(int)OPY.dynamicents.size();++i)
		{
			int EID = OPY.dynamicents[i];
			OPY.entpositions[EID] = OPY.entpositions[EID] + OPY.entvelocities[i]*dt;
		}
	};

        //------------------------------------------//
        //       Apply Rotational Velocities        //
        //------------------------------------------//
        void CalculateRotations(float dt,objPhys &OPY)
        {
		float fDegtoRad = this->DegtoRad;
		#pragma omp parallel for shared(dt,fDegtoRad)
                for (int i=0;i<(int)OPY.dynamicents.size();++i)
                {
                        int EID = OPY.dynamicents[i];

	                jsm::quat<float> rotquat = jsm::quatGenLocal( OPY.entrv[i] * fDegtoRad * dt,glm::normalize(OPY.entqvaxis[i]) );
	                glm::mat4 rotmat = quatUnittoMatrix(rotquat);

                        OPY.entrotmat_gbl[EID] = OPY.entrotmat_gbl[EID] * rotmat;
                }
        };

};

#endif
