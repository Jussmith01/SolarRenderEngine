#define GLEW_STATIC
//Included Shader Program
//#include "headers/shader.h"
//#include "mainHeaders.h"
//#include "objectControllers/model.h"
//#include "basicStructures/fps.h"
#include "EngineHeader.h"

//ERROR CHECKERS
StateBase * StateBase::event_handling_instance;

//*******************//
//GLFW error checking//
//*******************//
void errchk(unsigned int code)
{
	if (code == GL_FALSE)
	{
		fprintf(stderr, "ERROR: GLFW Could Not Start. Exiting.\n");
		exit(0);
	}
}

//********************//
//Engine Display Class//
//********************//
void Engine_Init::RunDisplay()
{
	//GLFW Initiate
	errchk(glfwInit());
	cout << glfwGetVersionString() << endl;

	//GLFW Set Window Properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, props.MSAA);

	props.init(); //Initialize the engine properties from the properties input file "Settings."
	camera.init(props); //Initialize the camera class.

	//Initialize Window
	switch(props.FullScreen)
	{
	 case 0: 
	 {window = glfwCreateWindow(props.WinWidth, props.WinHeight, "GameEngine Test", nullptr, nullptr);break;}
	 case 1: 
	 {window = glfwCreateWindow(props.WinWidth, props.WinHeight, "GameEngine Test", glfwGetPrimaryMonitor(), nullptr);break;}
	}

	if( window == NULL ){
        	fprintf( stderr, "Failed to open GLFW window.\n" );
        	glfwTerminate();
    	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Set the call back functions
	setEventHandling();
        glfwSetKeyCallback(window, StateBase::keycallback_dispatch);
        glfwSetCursorPosCallback(window, StateBase::mousecallback_dispatch);
        glfwSetScrollCallback(window, StateBase::scrollcallback_dispatch);

	//Initialize Glew
	glewExperimental = GL_TRUE;
	glewInit();

	//Set view port dimensions
	glViewport(0, 0, props.WinWidth, props.WinHeight); 

	//Setup OpenGL Options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
    	glEnable(GL_CULL_FACE);
    	glEnable(GL_BLEND);
    	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Poly Fill Mode	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	//System Processors
	int NProc = sysconf( _SC_NPROCESSORS_ONLN );
	cout << "Number of System Processors: " << NProc << endl;

	//Max Number of Attributes
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	GLErrorCatch("GL Preamble");

	//Set the frames per second timer
	fpsTimer ft;

	//This sets the View and Projection "Matrices" Uniform Buffer Object at global location 0;
	//TM.SetViewUniformBufferObject();

	//Setup screen text writing
	SW.Setup("bin/fonts/FreeSans.ttf",props.WinWidth,props.WinHeight);//Setup screen font
	GLErrorCatch("Screen Writer Setup");

	//Initialize Shaders
	SCC.ShaderLoader();
	GLErrorCatch("Shader Handling");

	//Initialize SolarSystem
	double TIMECHECK = glfwGetTime();
	solarsystem.SetupClass(OPC);
	cout << "SolarSys CLASS SETUP TIME: " << glfwGetTime() - TIMECHECK  << "\n";
	cout << "SolarSystem class Setup...\n";
	solarsystem.GenerateSolarSystem(OPC,0,0);
	cout << "SolarSystem Generated...\n";
	GLErrorCatch("SolarSystem Generation");

	//Setup Global Uniform Handler
	GGUBO.SetGlobalUBO(SCC);
	camera.SetProjViewUBO(SCC);
	
	GLErrorCatch("Uniform Handling");

	//Setup Timing Stats
	TimingStats TS(SW,props);
	TS.SetObject("Preamble");
	TS.SetObject("camera.PrepareCameraData");
	TS.SetObject("GGUBO.UpdateGlobalUBO");
	TS.SetObject("solarsystem.PrepareData");
	TS.SetObject("ProcessDrawArray");
	TS.SetObject("PrintScreenTimers");
	TS.SetObject("Screen_Console");
	TS.SetObject("glfwSwapBuffers");
	TS.SetObject("drawArray.ClearDrawArrays");
	TS.SetObject("EventPolling");
	TS.SetObject("Skybox Drawing");

	while(!glfwWindowShouldClose(window))
	{
                //*********************
                //Frame Timer
                //*********************
		TS.TS(0);
                dt.SetTime();

		//*************************
		//Sets the background color
                //*************************
                glClearColor(Env.bgColor.x, Env.bgColor.y, Env.bgColor.z, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
		TS.TE(0);

		//***************
		//Setup Scene
		//***************
		//Prepare the camera class for the frame
		//cout << "***TEST1***" << endl;
		TS.TS(1);
		camera.PrepareCameraData();
		TS.TE(1);
		//Update the global uniform buffer objects
		TS.TS(2);
		GGUBO.UpdateGlobalUBO(camera);
		TS.TE(2);
		//Prepare the solarsystem data
		//cout << "***TEST2***" << endl;
		TS.TS(3);
		solarsystem.PrepareData(OPC,drawArray,drawInstArray,camera,dt,false);
		TS.TE(3);

		//***************
                //Scene Rendering
                //***************
		//cout << "***TEST3***" << endl;
		//Draw all objects

		TS.TS(10);
		skybox.DrawSkybox(camera,props);
		TS.TE(10);

		TS.TS(4);
		ProcessDrawArray();
		TS.TE(4);

		//*******************
		//Interface Rendering
		//*******************
		//Render Frame Timer
		TS.TS(5);
		PrintScreenTimers(SW,ft);
		TS.TE(5);
		//cout << "***TEST4***" << endl;
		//Console Functions
		TS.TS(6);
		Screen_Console(SW);
		TS.TE(6);

		//Statistics Rendering
		TS.CalcAndRenderStats();

		//*************
		//Swap Buffers
		//*************
		TS.TS(7);
		//glfwSwapInterval(1); //Seems to slow the program to 30fps
		glfwSwapBuffers(window);//Something is wrong here, SUPER expensive
		TS.TE(7);

                //*********************
                //Check and call events
                //*********************
                TS.TS(9);
                glfwPollEvents();
                //glfwWaitEvents();
                Do_Movement();
                TS.TE(9);

                //*************
                //Frame Cleanup
                //*************
		TS.TS(8);
		drawArray.DrawArrayReset();
		drawInstArray.ClearInstDrawArrays();
		TS.TE(8);
		//cout << "FRAME END\n";

		//GLErrorCatch("MainLoop");
	}

	drawArray.ClearDrawArrays();
	glfwTerminate();
}

