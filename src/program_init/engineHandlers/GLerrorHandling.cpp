#define GLEW_STATIC

//#include "../mainHeaders.h"
#include "../EngineHeader.h"

using namespace std;


//-------------------------------------------//
//            Input Class Functions          //
//-------------------------------------------//
//Function handles all press and hold movements
void Engine_Init::GLErrorCatch(string ErrorPos)
{
	GLenum ErrorChk = glGetError();
        if (ErrorChk != GL_NO_ERROR)
		{cout << "!!GL Error " << ErrorChk << " Detected!! :: " << gluErrorString(ErrorChk) << " :: In ErrorPos: " << ErrorPos.c_str() << "\n";}

}

