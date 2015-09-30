#define GLEW_STATIC
#ifndef Statistics
#define Statistics

#include "../mainHeaders.h"
#include "gameStructs.h"

using namespace std;

struct StatsObject
{
	string caption;
	double diff;
	double timestore;

	long int N;
	string opt;

	StatsObject(string caption){this->caption=caption;timestore=0;diff=0;N=0;};

	string RendLine (double TT,bool reset)
	{
		if (reset)
		{
			double perctime = 100.0f * (timestore / (double)TT);
			double avgtime = 1000 * (timestore / (double)N);

			stringstream ss;
			ss << std::setprecision(3) << perctime << "% - " << avgtime << "ms - " << caption;
			opt = ss.str();
		}
	
		return opt;
	};
};

class TimingStats
{
	//Screen Writer to be Registered in Constuctor
	ScreenWriter SW;
	Properties props;

	double ptime;

	double MTT;//Minute total time
	double STT;//1/2 second total time
	vector<StatsObject> SO;

	public:
	//Constructor
	TimingStats(ScreenWriter &SW,Properties &props)
		{this->SW=SW;this->props=props;MTT=0;STT=0;ptime=0;};

	void SetObject(string caption)
		{SO.push_back(caption);};

	//Renderer
	void CalcAndRenderStats()
	{
		MTT += glfwGetTime() - ptime;
		STT += glfwGetTime() - ptime;
		 
		ptime = glfwGetTime();
		
		float WW = (float)props.WinWidth;
		//float WH = (float)props.WinHeight;

		bool reset = false;
		if (STT > 0.5)
			{STT = 0;reset = true;}

		float Font = props.FontSize;
		float Line = props.LineSpace;
		for (int i=0;i<(int)SO.size();++i)
		{
			SW.RenderText(SO[i].RendLine(MTT,reset),WW - 1700.0f * Font, 75.0f + i * Line,Font,glm::vec3(1.0f,1.0f,1.0f));
		}

		if (MTT > 60)
		{
			for (int i=0;i<(int)SO.size();++i)
                		{SO[i].timestore = 0;SO[i].N = 0;}
			MTT = 0;
		}
	};

	//Start Timer
	void TS(int SOBJID)
		{SO[SOBJID].diff = glfwGetTime();};

	//End Timer
	void TE(int SOBJID)
                {SO[SOBJID].timestore += glfwGetTime() - SO[SOBJID].diff;++SO[SOBJID].N;};

};

class RandomInt
{
	int seedOffset;
	
	public:
	RandomInt() 
	{
		time_t Time;
		time(&Time);
		seedOffset=(int)Time;
	};

	int GenRandInt(int high,int low)
	{
		srand(glfwGetTime()+seedOffset);
		int RN = (int)(rand() % (high-low+1) + low);
		++seedOffset;
		seedOffset = seedOffset % 5000;
		return RN;
	};
};

class NormRandomReal
{
        std::default_random_engine generator;
        std::vector<int> array;
        int index;

        public:
        NormRandomReal(){};
        NormRandomReal(int w,int seed){Setup(w,seed);};

        void Setup(int w,int i)
        {
                time_t Time;
                time(&Time);
                int seedOffset=(int)Time;

                array.resize(w);

                int t = (int)omp_get_wtime()+i;
                std::seed_seq seed = {seedOffset,t,i+100};
                seed.generate(array.begin(),array.end());//Seed the generator
                index = 0;
        };

        float GenRandReal(float mean,float std)
        {
                generator.seed(array[index]);//Seed the generator
                std::normal_distribution<float> distribution(mean,std);//Setup the distribution
                float RN = (float)distribution(generator);//Denerate the random number
                //std::cout << "RandomNumber: " << RN << std::endl;
                ++index;//Increase seed offset
                return RN;
        };
};

class UniformRandomReal
{
        std::default_random_engine generator;
        std::vector<int> array;
        int index;

        public:
        UniformRandomReal(){};
        UniformRandomReal(int w,int seed){Setup(w,seed);};

        void Setup(int w,int i)
        {
                time_t Time;
                time(&Time);
                int seedOffset=(int)Time;

                array.resize(w);

                int t = (int)omp_get_wtime()+i;
                std::seed_seq seed = {seedOffset,t,i+100};
                seed.generate(array.begin(),array.end());//Seed the generator
                index = 0;
        };

        double UniformRandReal(float low,float high)
        {
                generator.seed(array[index]);//Seed the generator
                std::uniform_real_distribution<double> distribution(low,high);//Setup the distribution
                double RN = (double)distribution(generator);//Denerate the random number
                //std::cout << "RandomNumber: " << RN << std::endl;
                ++index;//Increase seed offset
                return RN;
        };
};

#endif
