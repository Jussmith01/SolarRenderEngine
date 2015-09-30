#define GLEW_STATIC
#ifndef SolarSysStructs
#define SolarSysStructs

#include "../../mainHeaders.h"
#include "../../DrawController/DrawStructs.h"
#include "../../basicStructures/gameStructs.h"

//******************************************//
//                Sun Structure             //
//******************************************//
struct Sun
{
	int OPCID;
	int OBJID;
	float Mass;
	float Scale;
	glm::vec3 Position;
	glm::vec3 PlaneNormal;
        jsm::quat<float> GlbRotQuat;//This is your global rotation quat
        glm::vec4 vRotVec;//Build a local rotation matrix out of this
	glm::vec3 Color;
	float Intensity;

	//**********************//
	//  Produce DrawStruct  //
	//**********************//
	DrawStruct GetDrawStuct (Camera camera,int dlevel)
	{
		DrawStruct DS;
		DS.OPCID=OPCID;
		DS.OBJID=OBJID;

		//Calculate angle between campos and sunpos
		glm::vec2 CamP2 = glm::vec2(camera.Pos.x,camera.Pos.z);
		glm::vec2 Pos2 = glm::vec2(Position.x,Position.z);
		glm::vec2 Norm2 = glm::vec2(PlaneNormal.x,PlaneNormal.z);

		glm::vec2 C = glm::normalize(CamP2 - Pos2);
		glm::vec2 N = -glm::normalize(Norm2);
	
		float DNC = glm::dot(N,C);	
		float theta = acos(-DNC);

		if (C.y < 0.0f)
			{theta = -theta;}

		//if (C.y >= 0.0f)
		//	{theta = theta + 3.141592654;}
		//float theta = acos(0.001f);

		if (theta != theta) {theta=0.0;}
		glm::vec3 BiNorm = glm::vec3(0.0,1.0f,0.0);

		//cout << "Theta: " << theta / DEGTORAD << " DNC: " << DNC << " Cam: " << C.x << "," << C.y << " Norm: " << N.x << "," << N.y << endl;

		

		jsm::quat<float> cR = jsm::quatGenLocal( theta,BiNorm );
		//jsm::quat<float> cR2 = jsm::quatGenLocal( theta,BiNorm );

		//GlbRotQuat = cR * GlbRotQuat;
		//PlaneNormal = jsm::quatRotVec3(cR2,PlaneNormal);
	
                glm::mat4 model;
                model = glm::translate(model, Position);
                model = glm::scale(model, glm::vec3(Scale));
                model = model * jsm::quatUnittoMatrix(cR);
                //model = model * jsm::quatUnittoMatrix(GlbRotQuat);
                //model = glm::rotate(model,-theta,glm::vec3(0.0f,1.0f,0.0f));

		//DS.TransMat = camera.GetVPTransMat(dlevel);
		DS.ModelMat = model;

		DS.objPos = Position;
		//cout << "SUNDS OPCID: " << DS.OPCID << " OBJID: " << DS.OBJID << endl;
		return DS;
	};
};

//******************************************//
//       	Planet Structure            //
//******************************************//
struct Planet
{
	struct MoonIDs
	{
		vector<int> MID;
		MoonIDs(int null){};
	};

	int NumPlts;
        //std::vector<int> PLANETID;
        std::vector<int> OPCID;
        std::vector<int> OBJID;
        std::vector<int> Habitable;//0 for gas giants
        std::vector<float> Scale;
        std::vector<float> Mass;
        std::vector<glm::vec4> OrbitRotVec;//Build a local rotation matrix out of this
        std::vector<glm::vec3> Position;//w.r.t. SUN this + SUNpos = pos
        //std::vector<glm::vec3> Velocity;
        std::vector<jsm::quat<float>> GlbRotQuat;//This is your global rotation quat
        std::vector<glm::vec4> vRotVec;//Build a local rotation matrix out of this

	std::vector<MoonIDs> MIDS;

        //**********************//
        //  Produce DrawStruct  //
        //**********************//
        DrawStruct GetDrawStuct (Camera camera,glm::vec3 sunPos,int pltIdx,int dlevel)
        {
                DrawStruct DS;
                DS.OPCID = OPCID[pltIdx];
                DS.OBJID = OBJID[pltIdx];

                glm::mat4 model;
                model = glm::translate(model, Position[pltIdx] + sunPos);
                model = glm::scale(model, glm::vec3(Scale[pltIdx]));
                model = model * jsm::quatUnittoMatrix(GlbRotQuat[pltIdx]);

                //DS.TransMat = camera.GetVPTransMat(dlevel);
		DS.ModelMat = model;

		DS.objPos = Position[pltIdx];
		//cout << "PLTIDX: " << pltIdx << " OPCID: " << DS.OPCID << " OBJID: " << DS.OBJID << endl;
		return DS;
        };
};

//******************************************//
//         	Moon Structure              //
//******************************************//
struct Moon
{
	int NumMoon;
        //std::vector<int> MOONID;
        std::vector<int> PLANETID;
        std::vector<int> OPID;//ID of the planet it orbits
        std::vector<int> OPCID;
        std::vector<int> OBJID;
        std::vector<int> Habitable;//Habitability will be based on race and tech.
        std::vector<float> Scale;
        std::vector<float> Mass;
        std::vector<glm::vec4> OrbitRotVec;//Build a local rotation matrix out of this
        std::vector<glm::vec3> Position;//w.r.t the planet, done for precision, this + planetpos = pos
        //std::vector<glm::vec3> Velocity;
        std::vector<jsm::quat<float>> GlbRotQuat;//This is your global rotation quat
        std::vector<glm::vec4> vRotVec;//Build a local rotation matrix out of this

        //**********************//
        //  Produce DrawStruct  //
        //**********************//
        DrawStruct GetDrawStuct (Camera camera,glm::vec3 SunPosition,glm::vec3 PlanetPosition,int monIdx,int dlevel)
        {
                DrawStruct DS;
                DS.OPCID=OPCID[monIdx];
                DS.OBJID=OBJID[monIdx];

                glm::mat4 model;
                model = glm::translate(model, Position[monIdx] + PlanetPosition);
                model = glm::scale(model, glm::vec3(Scale[monIdx]));
                model = model * jsm::quatUnittoMatrix(GlbRotQuat[monIdx]);

                //DS.TransMat = camera.GetVPTransMat(dlevel);
		DS.ModelMat = model;

		DS.objPos = Position[monIdx] + PlanetPosition - SunPosition;
		//cout << "MONIDX: " << monIdx << " OPCID: " << DS.OPCID << " OBJID: " << DS.OBJID << endl;
                return DS;
        };
};

//******************************************//
//            Astroid Structure             //
//******************************************//
struct Asteroid
{
	int NumAst; //Number of this type of astroid
	int OPCID;
	int OBJID;

        std::vector<int> ORBTYPE; //Type of orbital - planet = 0 or astfield = 1
        std::vector<int> ORBID; //What orbital entity is it orbiting Either planet ID or astfield ID
        std::vector<int> ASTID; //The asteroids ID
        std::vector<float> Scale; //Size of the roid
	std::vector<float> Mass; //Mass of the roid
        std::vector<glm::vec4> OrbitRotVec;//Build a local rotation matrix out of this
	std::vector<glm::vec3> Position;//w.r.t SUN
	//std::vector<glm::vec3> Velocity;
        std::vector<jsm::quat<float>> GlbRotQuat;//This is your global rotation quat
        std::vector<glm::vec4> vRotVec;//Build a local rotation quaternion out of this
	
	//*******************************//
        //    Produce Model Matrices     //
        //*******************************//
	vector<glm::mat4> CalculateModelMatVector(glm::vec3 SunPos, Planet &planets)
	{
		vector<glm::mat4> modelMatrices;
		modelMatrices.resize(NumAst);

		#pragma omp parallel for shared(SunPos)
		for (int i=0;i<NumAst;++i)
		{
			glm::mat4 model;
                	model = glm::translate(model, Position[i] + planets.Position[ORBID[i]] + SunPos);
                	model = glm::scale(model, glm::vec3(Scale[i]));
                	model = model * jsm::quatUnittoMatrix(GlbRotQuat[i]);
			//cout << "ModMatPos: " << model[3][0] << "," << model[3][1] << "," << model[3][2] << endl;
			modelMatrices[i] = model;
		}
		#pragma omp barrier
		return modelMatrices;
	};

	//*******************************//
        //  Produce DrawInstancedStruct  //
        //*******************************//
        DrawInstancedStruct GetDrawStuct (glm::vec3 SunPosition)
        {
                DrawInstancedStruct DS;
                DS.OPCID=OPCID;
                DS.OBJID=OBJID;

                //DS.TransMat = camera.GetVPTransMat(dlevel);
                DS.SunDirection = SunPosition;
                DS.N = NumAst;
                //cout << "MONIDX: " << monIdx << " OPCID: " << DS.OPCID << " OBJID: " << DS.OBJID << endl;
                return DS;
        };
};
#endif
