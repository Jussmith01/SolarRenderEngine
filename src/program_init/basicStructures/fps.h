#define GLEW_STATIC

#include "../mainHeaders.h"

using namespace std;

class fpsTimer
{
	int frames;
	double timestore;

	public:
	string mspf;
	string fps;

	fpsTimer(){timestore = 0;frames = 0;};

	void printfpsTimer(float dt)
	{
		timestore += dt;
		++frames;
		if (timestore > 1.0f)
		{
			float msPf = 1000.0f / (float)frames;
			float fPs = (float)frames;

			stringstream ss;
			ss << msPf << "ms/f";

                        stringstream ss2;
                        ss2 << fPs << "f/s";

			timestore = 0;
			frames = 0;

			mspf = ss.str();
			fps = ss2.str();
		}
	};

	~fpsTimer(){};
};

