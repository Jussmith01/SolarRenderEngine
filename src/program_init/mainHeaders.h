#ifndef mainclassHeaded
#define mainclassHeader

//OGL AND GLFW LIBRARIES
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

//SOIL LIBRARY (TEXTURE LOADING)
//#include <SOIL/SOIL.h>
#include <SOIL.h>


//C/C++ STD LIBRARIES
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <cmath>
#include <unistd.h>
#include <ctgmath>
#include <time.h>
#include <random>

//GLM LIBRARIES
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

//OpenMP Library
#include <omp.h>

//FREETYPE LIBRARIES
//#include <ft2build.h>
//#include <freetype2/freetype.h>
//#include FT_FREETYPE_H

//CUSTOM PROGRAM LIBRARIES
#include "math/jmath.h"
#include "tools/tools.h"
#include "tools/screenwriter.h"

//Some Global Constants
#ifndef DEGTORAD_MACRO
#define DEGTORAD_MACRO true
const float DEGTORAD = 0.0174532925;
#endif

#ifndef GRAVC_MACRO
#define GRAVC_MACRO true
const float GRAVC=0.01;
#endif

/*
#include "objectControllers/objectController.h"
#include "objectControllers/shaderController.h"
#include "objectControllers/model.h"
#include "objectControllers/objectPhysics.h"
#include "objectControllers/meshloader.h"
#include "objectControllers/shader.h"
#include "objectControllers/object_texture.h"
#include "objectControllers/object_coords.h"
#include "objectControllers/object_light.h"
#include "basicStructures/gameStructs.h"
#include "basicStructures/fps.h"
#include "basicStructures/base_classes.h"
*/
#endif
