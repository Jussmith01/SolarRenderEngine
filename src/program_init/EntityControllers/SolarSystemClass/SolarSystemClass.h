#define GLEW_STATIC
#ifndef SolarSysClass
#define SolarSysClass

#include "../../mainHeaders.h"
#include "../../objectControllers/objectController.h"
#include "../../objectControllers/shaderController.h"
#include "SolarSystemStructs.h"
#include "../../DrawController/DrawStructs.h"
#include "../../basicStructures/statistics.h"

//******************************************//
//             Solar System Class           //
//******************************************//
//This Class controls all aspects of solar 
//system rendering and control aspects.
class SolarSystem
{
	float InitialSeed;//Used 
	int SUNSOPCID;
	int PLTSOPCID;
	int MONSOPCID;
	int ASTSOPCID;
	bool Active;

	RandomInt Rand;//Random Generator
	//NormRandomReal NormRandf;//Random Generator

        //******************************
        //	    Sun Data
        //******************************
	Sun sun;

	//******************************
	//Linearly Stored Planetary Data
	//******************************
	Planet planets;
	Moon moons;
	Asteroid asteroids;

        //******************************
        //Linearly Stored Astroid Data
        //******************************
	//std::vector<Astroid> astroid;//These Are Instanced
	
	public:
        SolarSystem(float ISeed)
        	{this->InitialSeed=ISeed;Active=true;};

        SolarSystem()
                {this->InitialSeed=glfwGetTime();Active=true;};

        ~SolarSystem () {};

	//**************************
	//Class Management Functions
	//**************************
	void PrepareData(vector<objPropController> &OPC,DrawArray &drawArray,DrawInstancedArray &drawInstArray,Camera &camera,deltaTime dt,bool pause);
	void SetupClass(vector<objPropController> &OPC);
	void GenerateSolarSystem(vector<objPropController> &OPC,int Size,int RA);
	void LoadSolarSystem(void);
	void SaveSolarSystem(void);
	
	private:
        //**************************
        //Class Peripheral Functions
        //**************************
	//float DetermineDepthLevel(float Dist);
	void GenRandomSun(objPropController &SUNOPC,int SUNSOPCID);
	void GenRandomPlanet(objPropController &PLTSOPC,int PLTID,int PLTSOPCID);
	void GenRandomMoon(objPropController &MONSOPC,int PLTID,int MONID,int MONSOPCID);
	void GenRandomAsteroids(objPropController &ASTSOPC,int NumAsteroids);

	//**************************
	//Class Physics Functions
	//**************************
	glm::vec4 CalculateRotationVector(void);
	glm::vec4 CalculateOrbitalVector(float MagR,float MASS2);
	glm::vec4 CalculateOrbitalVector(float MagR,float MASS2,glm::vec3 OrbitAxis);
	glm::vec3 RandomStartingPosition(float MagR);
	void PropagatePlanets(float dt);
	void PropagateMoons(float dt);
	void PropagateAsteroids(float dt);
};
#endif
