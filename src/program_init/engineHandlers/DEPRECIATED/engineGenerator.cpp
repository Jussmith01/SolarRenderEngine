#define GLEW_STATIC

//#include "../mainHeaders.h"
#include "../EngineHeader.h"
//#include "../objectControllers/objectPhysics.h"
//#include "../objectControllers/object_coords.h"

using namespace std;


//-------------------------------------------//
//               Generator		     //
//-------------------------------------------//
void Engine_Init::Generator(string param)
{
        //Generates a Random solar system
        if (param.compare("solsys") == 0)
        {
                CDat.Buffer("Generating Solar System:");
                CDat.Buffer("Generating Sun...");
                int SUNEID = GenerateGravEntity("SUN",0);
                int SUNGID = OPY.ents[SUNEID].gravidx;

                //int Np = RandI.GenRandInt(20,6);
                int Np = RandI.GenRandInt(10,10);

		float DistRange = OPY.scale[SUNEID] * (((float)RandI.GenRandInt(100,50) / (float)Np)+5.0f);
		
		float PlanetRangeLow = OPY.scale[SUNEID] * 5.0f;
		float PlanetRangeHigh = PlanetRangeLow + DistRange;

                for (int i = 0; i < Np ; ++i)
                {
                        //int PType = RandI.GenRandInt(2,1);
                        int PType = RandI.GenRandInt(2,1);
                        stringstream ss; ss << "Planet-" << i;
                        int PEID = GenerateGravEntity(ss.str(),PType);
                        int PGID = OPY.ents[PEID].gravidx;
                        int Dist = (float)RandI.GenRandInt(PlanetRangeHigh,PlanetRangeLow);
                        SetGravEnt(PGID,SUNGID,(float)Dist);

			cout << "Producing Planet in range [" << PlanetRangeLow << "," << PlanetRangeHigh << "]\n\n";

			PlanetRangeLow=PlanetRangeHigh;
			PlanetRangeHigh=PlanetRangeHigh+DistRange; 

                	//int Nm = RandI.GenRandInt(PType*3,1);
                	int Nm = RandI.GenRandInt(1,1);
			for (int i = 0; i < Nm ; ++i)
	                {
        	                stringstream ss2;ss2 << ss.str() << "-Moon-" << i;
        	                int MEID = GenerateGravEntity(ss2.str(),3);
        	                int MGID = OPY.ents[MEID].gravidx;
        	                int MDist = (float)RandI.GenRandInt((int)OPY.scale[PEID] * 4 * PType,(int)OPY.scale[PEID] * 2);
        	                SetGravEnt(MGID,PGID,(float)MDist);
        	        }
			stringstream ssp; ssp << "Produced Planet " << i << " with " << Nm << "moons."; 
			CDat.Buffer(ssp.str());
                }
        } else {
                stringstream ss;
                ss << "Parameter \"" << param << "\" not found in \"list\" function...\n";
                CDat.Buffer(ss.str());
        }
};

//------------------------------------------
//         Generate Entity Function
//------------------------------------------
int Engine_Init::GenerateGravEntity(string name,int type)
{
        entityProps EP;

        //0=sun;1=planet;2=gasgiant;3=moon
        int objH=0;int objL=0;
        int sclH=0;int sclL=0;float sclS=1;
        glm::vec3 pos;
        glm::vec3 vel;
        glm::vec3 rot(0.0f,1.0f,0.0f);//BOTH
        float a1=0.0f;
        float a2=0.0f;
        float MASS = 1; //This simply becomes a multiplyer by size
        bool HasIcon=false;
        switch(type)
        {
                case 0:
                {
                        objH=1;objL=0;
                        sclH=100;sclL=25;sclS=2.0;
                        MASS = 3000;a2=-10;
                        HasIcon=false;
                        break;
                }
                case 1:
                {
                        objH=2;objL=2;
                        sclH=150;sclL=50;sclS=0.015;
                        MASS = 1;a2=-40;
                        HasIcon=true;
                        break;
                }
                case 2:
                {
                        objH=3;objL=3;
                        sclH=100;sclL=50;sclS=0.18;
                        MASS = 20;a2=-10;
                        HasIcon=true;
                        break;
                }
                case 3:
                {
                        objH=6;objL=4;
                        sclH=175;sclL=50;sclS=0.002;
                        MASS = 0.1;a2=-60;
                        HasIcon=false;
                        break;
                }
        }

        EP.NAME = name;
        EP.OPCID = 0;

        EP.OBJID = RandI.GenRandInt(objH,objL);
        EP.scale = (float)RandI.GenRandInt(sclH,sclL)*sclS;
        EP.pos = pos;
        EP.entqgaxis = rot;
        EP.enta = a1;
        EP.dyn = 1;
        EP.grav = 1;
        EP.MASS = MASS * EP.scale;

        EP.vel = vel;
        EP.entqvaxis = rot;
        EP.entrv = a2;

        EP.HasIcon = HasIcon;
        if (HasIcon)
        {
                EP.IOPCID = 0;
                EP.IOBJID = 7;
        }

        int EID = OPY.CreateEntity(OPC,SCC,EP);
        Lits.SetNewEntity(EID,EP.OPCID,EP.OBJID,EP.pos,OPC);

        return EID;
};

//------------------------------------------
//           Set Orbit Function
//------------------------------------------
void Engine_Init::SetGravEnt(int GID1,int GID2,float MagR)
{
        float DegtoRad = 0.0174532925;
        float G=0.01;

        glm::vec3 OrbitAxis(0.0f,1.0f,0.0f);
        glm::vec3 OrthoPVec(1.0f,0.0f,0.0f);
        glm::vec3 OrthoVVec(0.0f,0.0f,1.0f);

        int EID1 = OPY.gravityents[GID1];
        int EID2 = OPY.gravityents[GID2];
        int GTDID1 = OPY.gravityToDynIdx[GID1];
        int GTDID2 = OPY.gravityToDynIdx[GID2];
        float M1 = OPY.entsmass[GID1];
        float M2 = OPY.entsmass[GID2];

          /* initialize random seed: */
        srand (glfwGetTime() + GID1 + GID2 + MagR);

        /* generate secret number between 1 and 10: */
        int RandomNumber = rand() % 359;

        jsm::quat<float> RandomRotation = jsm::quatGenLocal((float)RandomNumber*DegtoRad,OrbitAxis);
        glm::vec3 RandPVec = jsm::quatRotVec3(RandomRotation,OrthoPVec);
        glm::vec3 RandVVec = jsm::quatRotVec3(RandomRotation,OrthoVVec);

        cout << "RandomRotation = " << RandomNumber << " SEED-glfwGetTime(): " << glfwGetTime() << endl;
        cout << "RandPVec = [" << RandPVec.x << "," << RandPVec.y << "," << RandPVec.z <<"]\n";
        cout << "RandVVec = [" << RandVVec.x << "," << RandVVec.y << "," << RandVVec.z <<"]\n";

        OPY.entpositions[EID1] = MagR * glm::normalize(RandPVec) + OPY.entpositions[EID2];

        glm::vec3 VelVec = glm::normalize(RandVVec);
        cout << "EID1: " << EID1 << " EID2: " << EID2 << " GTDID1: " << GTDID1 << " GTDID2: " << GTDID2 << " M1: " << M1 << " M2: " << M2 << endl;

        //float Fg = (G*M1*M2)/(MagR * MagR);
        //float MagV = sqrt((Fg * MagR) / M1);
        float MagV = sqrt((G * M2) / MagR);
        VelVec = MagV * VelVec;
        OPY.orbits.push_back(GID1);
        OPY.orbits[OPY.orbits.size()-1].GIDHeavy=GID2;

        cout << "VelVec = [" << VelVec.x << "," << VelVec.y << "," << VelVec.z <<"]\n";
        OPY.entvelocities[GTDID1] = VelVec + OPY.entvelocities[GTDID2];
};


