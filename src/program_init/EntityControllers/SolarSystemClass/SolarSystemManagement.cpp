#include "SolarSystemClass.h"

int DetermineLoadedOpf(vector<objPropController> OPC,string filename)
{
	int POS=-1;
	for (int i=0;i<(int)OPC.size();++i)
	{
		if(OPC[i].opfsfile.compare(filename)==0)
			{POS=i;}
	}
	return POS;
};

//******************************************//
//	   Solar System Generator	    //
//******************************************//
//Parameter 1: Size = Solar System Size Value
//0 = 1 to 3 Planets;
//1 = 4 to 8 Planets;
//2 = 9 to 12 Planets;
//
//Parameter 2: RA = Resource Abundancy
//This controls the types of planets and
//astroids that are generated.
//0 = 30% Livable Planets;30% Astroid Abundancy
//1 = 50% Livable Planets;50% Astroid Abundancy
//2 = 80% Livable Planets;80% Astroid Abundancy
void SolarSystem::GenerateSolarSystem(vector<objPropController> &OPC,int Size,int RA)
{
		//Randomly Generate a New Solar System

		// Generate the sun
		GenRandomSun(OPC[SUNSOPCID],SUNSOPCID);

		//Generate random number of orbiting planets
		planets.NumPlts = Rand.GenRandInt(8,8);
		cout << "Generating " << planets.NumPlts << " planets...\n";
		for (int i=0;i<planets.NumPlts;++i)
			{GenRandomPlanet(OPC[PLTSOPCID],i,PLTSOPCID);}

                //Generate moons per planet
                //planets.NumPlts = Rand.GenRandInt(12,1);
		int MOONTOTAL = 0;
                for (int i=0;i<planets.NumPlts;++i)
                {
			planets.MIDS.push_back(0);
                	//int NMOONS = Rand.GenRandInt(ceil(planets.Scale[i]),0);
                	int NMOONS = Rand.GenRandInt(5,5);

			//cout << "Generating " << NMOONS << " moons...\n";
                	for (int j=0;j<NMOONS;++j)
				{GenRandomMoon(OPC[MONSOPCID],i,j,MONSOPCID);planets.MIDS[i].MID.push_back(MOONTOTAL);++MOONTOTAL;}
		}

		cout << "Generated " << MOONTOTAL << " moons...\n";
		moons.NumMoon = MOONTOTAL;

		//Build an asteroid belt
		//GenRandomAsteroids(OPC[ASTSOPCID],sun.Position,1000);
		GenRandomAsteroids(OPC[ASTSOPCID],1000);
};

//******************************************//
//           Solar System Loader            //
//******************************************//
void SolarSystem::LoadSolarSystem()
{
                //Load A Solar System
};

//******************************************//
//              Prepare Data                //
//******************************************//
void SolarSystem::PrepareData(vector<objPropController> &OPC,DrawArray &drawArray,DrawInstancedArray &drawInstArray,Camera &camera,deltaTime dt,bool pause)
{
		//TEST FOR SHIFTING POSITIONS
		if (camera.CamReset)
			{sun.Position = sun.Position - camera.PosOld;}

                //Prepare the solar systems data
		//1) Carry out motion calcs
		if (!pause)
		{
			float dTime=dt.GetScaledTime();
			//cout << "Carry out motion calcs - Time: " << dTime << "\n";
			PropagatePlanets(dTime);
			PropagateMoons(dTime);
			PropagateAsteroids(dTime);
		}

		//2) Determine Drawing if SS is active
		//cout << "Determine Drawing if SS is active\n";
		Active = true;
		if (Active)
		{
			glm::vec3 SunPos = sun.Position;

			//Determine Planets to Draw - ALWAYS DRAWN
                        #pragma omp parallel for shared(SunPos,camera,drawArray)
                        for (int i=0;i<planets.NumPlts;++i)
                        {
				//Prepare Planet
				glm::vec3 planetPos = planets.Position[i]+SunPos;
                                int DLevel = camera.DetermineDepthLevel(glm::length(planetPos - camera.Pos));

				drawArray.AddEntityToDrawArray(planets.GetDrawStuct(camera,SunPos,i,DLevel),DLevel);

				//Prepare Moons
				for (int j=0;j<(int)planets.MIDS[i].MID.size();++j)
				{
					int MID = planets.MIDS[i].MID[j];
                                	DLevel = camera.DetermineDepthLevel(glm::length((moons.Position[MID] + planetPos) - camera.Pos));
                                	if (DLevel != 0)
                                	{
						drawArray.AddEntityToDrawArray(moons.GetDrawStuct(camera,SunPos,planetPos,MID,DLevel),DLevel);
					}
				}
                        }
                        #pragma omp barrier

			int MID = OPC[ASTSOPCID].ObjLib[asteroids.OBJID].ModID;
			//cout << "SunPos=[" << SunPos.x << "," << SunPos.y << "," << SunPos.z << "]\n";
			vector<glm::mat4> ModMats = asteroids.CalculateModelMatVector(SunPos,planets);
			OPC[ASTSOPCID].ModLib[MID].UpdateInstancedModelMatrices(ModMats);
			drawInstArray.AddInstEntitiesToDrawArray(asteroids.GetDrawStuct(SunPos),1);

			//The sun is always drawn when in the solar system - Set it to be drawn
			//Sun draws last due to alphas.
                        int SDLevel = camera.DetermineDepthLevel(glm::length(SunPos - camera.Pos));
                        drawArray.AddEntityToDrawArray(sun.GetDrawStuct(camera,SDLevel),SDLevel);
		}
		
};

//******************************************//
//              Setup Class                 //
//******************************************//
void SolarSystem::SetupClass(vector<objPropController> &OPC)
{
	//Loads OPFs and saves their OPCID;
	struct Indexer 
	{
		int OPCID;
		int MODID;
		Indexer (int OPCID) {this->OPCID=OPCID;};
	};

	int TotIdx = 0;
	vector<Indexer> IDX;

        int IDXi[4];

	SUNSOPCID=DetermineLoadedOpf(OPC,"suns.opf");
	IDXi[0] = SUNSOPCID;
	if (SUNSOPCID==-1)
	{
		SUNSOPCID=OPC.size();OPC.push_back((string)"suns.opf");
	}

        PLTSOPCID=DetermineLoadedOpf(OPC,"planets.opf");
	IDXi[1] = PLTSOPCID;
        if (PLTSOPCID==-1)
        {
                PLTSOPCID=OPC.size();OPC.push_back((string)"planets.opf");
        }

        MONSOPCID=DetermineLoadedOpf(OPC,"moons.opf");
	IDXi[2] = MONSOPCID;
        if (MONSOPCID==-1)
        {
                MONSOPCID=OPC.size();OPC.push_back((string)"moons.opf");
        }

        ASTSOPCID=DetermineLoadedOpf(OPC,"asteroids.opf");
	IDXi[3] = ASTSOPCID;
        if (ASTSOPCID==-1)
        {
                ASTSOPCID=OPC.size();OPC.push_back((string)"asteroids.opf");
        }

	int IDXf[4];
	IDXf[0] = SUNSOPCID;IDXf[1] = PLTSOPCID;
	IDXf[2] = MONSOPCID;IDXf[3] = ASTSOPCID;
	#pragma omp parallel for shared(OPC,IDXi,IDXf)
	for(int i=0;i<4;++i)
	{
        	if (IDXi[i]==-1)
        	{
			//cout << "LOADINGOPF " << i << endl;
                	if(!OPC[IDXf[i]].OpfLoader()) {cout << "!Error Loading OPF!\n";}

			//cout << "NUMMOD " << OPC[IDXf[i]].NMod << endl;
                	for (int j=0;j<OPC[IDXf[i]].NMod;++j)
                	{
				#pragma omp critical
				{
                	        	IDX.push_back(IDXf[i]);
                	        	IDX[TotIdx].MODID = j;
                	        	++TotIdx;
				}
                	}
        	}
		//cout << "FINISHLOADINGOPF " << i << endl;
	}
	#pragma omp barrier

	#pragma omp parallel for shared(OPC,IDX)
	for (int i=0;i<TotIdx;++i)
	{
		//cout << "LOADINGMODEL " << i << " of " << TotIdx-1 << endl;
		//cout << "   OPCID " << IDX[i].OPCID << " MODID: " << IDX[i].MODID << endl;
		OPC[IDX[i].OPCID].ModLib[IDX[i].MODID].LoadModelToCPU();
	}
	#pragma omp barrier
};

