#include "SolarSystemClass.h"

//------------------------------------------//
//    Calculate Planetary Rotation Vecto    //
//------------------------------------------//
glm::vec4 SolarSystem::CalculateRotationVector()
{
        glm::vec4 vRotVec;
        glm::vec3 vRotAxis(0.0f,1.0f,0.0f);

        vRotVec.w = (float)Rand.GenRandInt(15,5) * DEGTORAD;
        vRotVec.x = vRotAxis.x;
        vRotVec.y = vRotAxis.y;
        vRotVec.z = vRotAxis.z;
        return vRotVec;
};

//------------------------------------------//
//    Calculate Orbital Rotation Vector     //
//------------------------------------------//
glm::vec4 SolarSystem::CalculateOrbitalVector(float MagR,float MASS2)
{
	glm::vec4 OrbitRotVec;
	glm::vec3 OrbitAxis(0.0f,1.0f,0.0f);

	float MagV = sqrt((GRAVC * MASS2) / MagR);

	OrbitRotVec.w = MagV / MagR;
	OrbitRotVec.x = OrbitAxis.x;
	OrbitRotVec.y = OrbitAxis.y;
	OrbitRotVec.z = OrbitAxis.z;
	return OrbitRotVec;
};

//------------------------------------------//
//    Calculate Orbital Rotation Asteroid   //
//------------------------------------------//
glm::vec4 SolarSystem::CalculateOrbitalVector(float MagR,float MASS2,glm::vec3 OrbitAxis)
{
        glm::vec4 OrbitRotVec;
        //glm::vec3 OrbitAxis(0.0f,1.0f,0.0f);

        float MagV = sqrt((GRAVC * MASS2) / MagR);

        OrbitRotVec.w = MagV / MagR;
        OrbitRotVec.x = OrbitAxis.x;
        OrbitRotVec.y = OrbitAxis.y;
        OrbitRotVec.z = OrbitAxis.z;
        return OrbitRotVec;
};

//------------------------------------------//
//    Calculate Random Starting Rotation    //
//------------------------------------------//
glm::vec3 SolarSystem::RandomStartingPosition(float MagR)
{
        glm::vec3 OrbitAxis(0.0f,1.0f,0.0f);
        glm::vec3 OrthoPVec(1.0f,0.0f,0.0f);

        int RandomNumber = Rand.GenRandInt(359,0);
        jsm::quat<float> RandomRotation = jsm::quatGenLocal((float)RandomNumber*DEGTORAD,OrbitAxis);
        glm::vec3 RandPVec = jsm::quatRotVec3(RandomRotation,OrthoPVec);

        return MagR * glm::normalize(RandPVec);
};

//******************************************//
//	      Propagate Bodies 	            //
//******************************************//
void SolarSystem::PropagatePlanets(float dt)
{
	int N = planets.NumPlts;
	#pragma omp parallel for firstprivate(dt)
	for (int i=0;i<N;++i)
	{
		//Orbital Rotation
		jsm::quat<float> OrbQuat = jsm::quatGenLocal(planets.OrbitRotVec[i].w*dt,glm::vec3(planets.OrbitRotVec[i]));
		planets.Position[i] = jsm::quatRotVec3(OrbQuat,planets.Position[i]);

		//Object Rotation
		jsm::quat<float> RotQuat = jsm::quatGenLocal(planets.vRotVec[i].w*dt,glm::vec3(planets.vRotVec[i]));
		planets.GlbRotQuat[i] = RotQuat * planets.GlbRotQuat[i];
	}
	#pragma omp barrier
};

void SolarSystem::PropagateMoons(float dt)
{
        int N = moons.NumMoon;
	#pragma omp parallel for firstprivate(dt)
        for (int i=0;i<N;++i)
        {
                //Orbital Rotation
                jsm::quat<float> OrbQuat = jsm::quatGenLocal(moons.OrbitRotVec[i].w*dt,glm::vec3(moons.OrbitRotVec[i]));
                moons.Position[i] = jsm::quatRotVec3(OrbQuat,moons.Position[i]);

                //Object Rotation
                jsm::quat<float> RotQuat = jsm::quatGenLocal(moons.vRotVec[i].w*dt,glm::vec3(moons.vRotVec[i]));
                moons.GlbRotQuat[i] = RotQuat * moons.GlbRotQuat[i];
        }
	#pragma omp barrier
};

void SolarSystem::PropagateAsteroids(float dt)
{
        int N = asteroids.NumAst;
        #pragma omp parallel for firstprivate(dt)
        for (int i=0;i<N;++i)
        {
                //Orbital Rotation
                jsm::quat<float> OrbQuat = jsm::quatGenLocal(asteroids.OrbitRotVec[i].w*dt,glm::vec3(asteroids.OrbitRotVec[i]));
                asteroids.Position[i] = jsm::quatRotVec3(OrbQuat,asteroids.Position[i]);

                //Object Rotation
                jsm::quat<float> RotQuat = jsm::quatGenLocal(asteroids.vRotVec[i].w*dt,glm::vec3(asteroids.vRotVec[i]));
                asteroids.GlbRotQuat[i] = RotQuat * asteroids.GlbRotQuat[i];
        }
        #pragma omp barrier
};

