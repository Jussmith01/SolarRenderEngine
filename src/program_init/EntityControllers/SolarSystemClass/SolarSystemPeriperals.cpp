#include "SolarSystemClass.h"

//**********************//
// Generate Random Sun  //
//**********************//
void SolarSystem::GenRandomSun(objPropController &SUNOPC,int SUNSOPCID)
{
        //Set OPCID
        sun.OPCID = SUNSOPCID;
        //Choose a Random Sun from the sun OPC
        sun.OBJID = Rand.GenRandInt(0,0);

	//Get the planes normal - This assumes the sun is a plane
	int MID = SUNOPC.ObjLib[sun.OBJID].ModID;
	sun.PlaneNormal = SUNOPC.ModLib[MID].mesh[0].vertices[0].normal;

        //Choose a Random Scale
        sun.Scale = (float)Rand.GenRandInt(200,50);
        //Set Mass based on Scale
        sun.Mass = sun.Scale * 3000.0f;
        //Set Initial Position
        sun.Position = glm::vec3(0.0f);
        //Set Rotation
	sun.GlbRotQuat = jsm::quat<float>();
        sun.vRotVec = glm::vec4(0.0f,1.0f,0.0f,0.0f);

	//Prepare model by loading the meshes to the GPU
	SUNOPC.ModLib[MID].LoadModelToGPU();

	//Prepare by loading texture to GPU
	SUNOPC.TextureToGPULoader(sun.OBJID);
};

//**********************//
//Generate Random Planet//
//**********************//
void SolarSystem::GenRandomPlanet(objPropController &PLTSOPC,int PLTID,int PLTSOPCID)
{
        //Set OPCID
        planets.OPCID.push_back(PLTSOPCID);
        //Choose a Random Sun from the sun OPC
        planets.OBJID.push_back(Rand.GenRandInt(2,0));
        //planets.OBJID.push_back(Rand.GenRandInt(PLTSOPC.ObjLib.size()-1,0));
        //planets.OBJID.push_back(0);

        //Choose a Random Scale
        if (PLTSOPC.ObjLib[planets.OBJID[PLTID]].optlinfo.compare("normal")==0)
        {
                //Normal Planet
                planets.Scale.push_back((float)Rand.GenRandInt(200,50)/10.0f);
                planets.Mass.push_back(planets.Scale[PLTID]);
        } else {
                //Gas Giant
                planets.Scale.push_back((float)Rand.GenRandInt(300,120)/5.0f);
                planets.Mass.push_back(planets.Scale[PLTID] * 6.0f);

        }
        //Set Initial Position
        float MagR=(float)Rand.GenRandInt(10000,1000);
        planets.Position.push_back(RandomStartingPosition(MagR));

        //Calculate Orbital Vector
        planets.OrbitRotVec.push_back(CalculateOrbitalVector(MagR,sun.Mass));

        //Set Rotation
	planets.GlbRotQuat.push_back(jsm::quat<float>());
        planets.vRotVec.push_back(CalculateRotationVector());

        //Prepare model by loading the meshes to the GPU
        int MID = PLTSOPC.ObjLib[planets.OBJID[PLTID]].ModID;
        PLTSOPC.ModLib[MID].LoadModelToGPU();

	//Prepare by loading texture to GPU
	PLTSOPC.TextureToGPULoader(planets.OBJID[planets.OBJID.size()-1]);
};

//**********************//
//Generate Random Moon  //
//**********************//
void SolarSystem::GenRandomMoon(objPropController &MONSOPC,int PLTID,int MONID,int MONSOPCID)
{
        //Set Planet ID
        moons.PLANETID.push_back(PLTID);
        //Set OPCID
        moons.OPCID.push_back(MONSOPCID);
        //Choose a Random Moon from the Moons OPC
        moons.OBJID.push_back(Rand.GenRandInt(MONSOPC.ObjLib.size()-1,1));
        //moons.OBJID.push_back(Rand.GenRandInt(0,0));
        //moons.OBJID.push_back(0);
        //Choose a Random Scale
        moons.Scale.push_back((float)Rand.GenRandInt(250,50)/100.0f);
        moons.Mass.push_back(moons.Scale[PLTID]/10.0f);
        //Set Initial Position
        float MagR=(float)Rand.GenRandInt(std::ceil(planets.Scale[PLTID]) * 14.0f,std::ceil(planets.Scale[PLTID]) * 3.0f);
        moons.Position.push_back(RandomStartingPosition(MagR));
        //Calculate Orbital Vector
        moons.OrbitRotVec.push_back(CalculateOrbitalVector(MagR,planets.Mass[PLTID]));
        //Set Rotation
	moons.GlbRotQuat.push_back(jsm::quat<float>());
        //moons.vRotVec.push_back(CalculateRotationVector());
        moons.vRotVec.push_back(glm::vec4(0.0f,1.0f,0.0f,10.0f * DEGTORAD));

        //Prepare model by loading the meshes to the GPU
        int MID = MONSOPC.ObjLib[moons.OBJID[MONID]].ModID;
        MONSOPC.ModLib[MID].LoadModelToGPU();

	//Prepare by loading texture to GPU
	MONSOPC.TextureToGPULoader(moons.OBJID[moons.OBJID.size()-1]);
};

//*******************************//
//Generate Random Asteroid Field //
//*******************************//
void SolarSystem::GenRandomAsteroids(objPropController &ASTSOPC,int NumAsteroids)
{
	asteroids.NumAst = NumAsteroids;
	NormRandomReal NormRandf(2*NumAsteroids,12487+NumAsteroids+ASTSOPCID);//Random Generator
	UniformRandomReal UniRandf(2*NumAsteroids,547542+NumAsteroids+ASTSOPCID);//Random Generator

	cout << "GEN NUM INSTANCES: " << asteroids.NumAst << endl;
        //Set OPCID
        asteroids.OPCID = ASTSOPCID;

	//Choose planet to orbit for belts
	//int PLTID = Rand.GenRandInt(planets.NumPlts,0);
	

        //Choose a Random Moon from the Moons OPC
        //moons.OBJID.push_back(Rand.GenRandInt(MONSOPC.ObjLib.size()-1,0));
        asteroids.OBJID = 0;
        //moons.OBJID.push_back(0);
	
        //Choose a Random Scale
	for (int i=0;i<NumAsteroids;++i)
	{
		int PLTID = Rand.GenRandInt(planets.NumPlts-1,0);
		asteroids.ORBID.push_back(PLTID);
        	asteroids.Scale.push_back((float)Rand.GenRandInt(250,25)/1000.0f);
        	asteroids.Mass.push_back(0.001f);
        	//Set Initial Position
        	float MagR=(float)NormRandf.GenRandReal(planets.Scale[PLTID] * 2.5f,2.0f);

        	glm::vec3 OrbitAxis(0.0f,0.85f,0.15f);
        	glm::vec3 OrthoPVec(1.0f,0.0f,0.0f);

        	float RandomNumber = UniRandf.UniformRandReal(359.0f,0.0f);
		//cout << "RandNum: " << RandomNumber << endl;
        	jsm::quat<float> RandomRotation = jsm::quatGenLocal((float)RandomNumber*DEGTORAD,OrbitAxis);
        	glm::vec3 RandPVec = jsm::quatRotVec3(RandomRotation,OrthoPVec);
		glm::vec3 Pos = MagR * glm::normalize(glm::vec3(RandPVec.x,RandPVec.y + NormRandf.GenRandReal(0.0f,0.005f),RandPVec.z));

        	asteroids.Position.push_back(Pos);
        	//asteroids.Position.push_back(RandomStartingPosition(MagR));
        	//Calculate Orbital Vector
        	asteroids.OrbitRotVec.push_back(CalculateOrbitalVector(MagR,planets.Mass[PLTID],OrbitAxis));
       		//Set Rotation
        	float RandomNumber2 = UniRandf.UniformRandReal(359,0);
		jsm::quat<float> StartingRotation = jsm::quatGenLocal((float)RandomNumber2*DEGTORAD,OrbitAxis);
        	asteroids.GlbRotQuat.push_back(StartingRotation);
        	//moons.vRotVec.push_back(CalculateRotationVector());
        	asteroids.vRotVec.push_back(glm::vec4(0.0f,1.0f,0.0f,0.01f));
	}

        //Prepare model by loading the meshes to the GPU
        int MID = ASTSOPC.ObjLib[asteroids.OBJID].ModID;
        ASTSOPC.ModLib[MID].LoadModelToGPU();

        //Prepare by loading texture to GPU
        ASTSOPC.TextureToGPULoader(asteroids.OBJID);

        vector<glm::mat4> modelMatrices;

	//#pragma omp parallel for shared(SunPos)
        for (int i=0;i<asteroids.NumAst;++i)
        {
		int PLTID = asteroids.ORBID[i];
                glm::mat4 model;
                model = glm::translate(model, asteroids.Position[i] + (planets.Position[PLTID] + sun.Position));
                model = glm::scale(model, glm::vec3(asteroids.Scale[i]));
                model = model * jsm::quatUnittoMatrix(asteroids.GlbRotQuat[i]);
                modelMatrices.push_back(model);
        }
	//#pragma omp barrier

	ASTSOPC.ModLib[MID].SetupInstancing(modelMatrices);
};

