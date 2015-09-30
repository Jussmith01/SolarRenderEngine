#define GLEW_STATIC
#ifndef ProgInit
#define ProgInit

#include "mainHeaders.h"
#include "objectControllers/objectController.h"
//#include "objectControllers/objectPhysics.h"
#include "objectControllers/shaderController.h"
#include "objectControllers/skybox_loader.h"
#include "basicStructures/gameStructs.h"
#include "basicStructures/fps.h"
#include "basicStructures/statistics.h"
#include "DrawController/DrawStructs.h"
#include "EntityControllers/SolarSystemClass/SolarSystemClass.h"


using namespace std;
//*****************************************//
//		State Base		   //
//*****************************************//
//This state base allows for use of member //
//functions to be used as glfw callback    //
//functions.				   //
//*****************************************//
class StateBase
{
    	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
        virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos) = 0;
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) = 0;

    	static StateBase *event_handling_instance;
    	// technically setEventHandling should be finalized so that it doesn't
    	// get overwritten by a descendant class.

	public:
    	virtual void setEventHandling() { event_handling_instance = this; };

	//Key Callback Function
    	static void keycallback_dispatch(GLFWwindow *window,int key,int scancode,int action,int mods)
    	{
        if(event_handling_instance)
       		event_handling_instance->key_callback(window,key,scancode,action,mods);
	};

	//Mouse Callback Function
        static void mousecallback_dispatch(GLFWwindow* window, double xpos, double ypos)
        {
        if(event_handling_instance)
                event_handling_instance->mouse_callback(window,xpos,ypos);
        };

	//Scroll Callback Function
        static void scrollcallback_dispatch(GLFWwindow* window, double xoffset, double yoffset)
        {
        if(event_handling_instance)
                event_handling_instance->scroll_callback(window,xoffset,yoffset);
        };
};

//*****************************************//
//	   Engine Initializer Class	   //
//*****************************************//
class Engine_Init : StateBase
{
        //-------------------------------------------//
        //              Class Variables              //
        //-------------------------------------------//
	GLFWwindow* window;

	//Data
	DrawArray drawArray;
	DrawInstancedArray drawInstArray;

	//Controllers
	vector<objPropController> OPC;//Controls all objects - Basically loads models and
	shaderController SCC;//Controls all shaders

	//Entity Controllers
	Skybox skybox;
	SolarSystem solarsystem;

	//****DEPRECIATE****
	//objPhys OPY;//Handles all entities and calls required physics functions

	//Periphrial Classes	
	Properties props; //Handles all the engine properties/settings.
	Camera camera; //Handles all the math involved with modifying the camera pos/dir.
	deltaTime dt; //Engine timers class. Maintains the engines time.
	GeneralGlobalUBO GGUBO;//Handles loading of the general UBO to GPU
	
	//****DEPRECIATE****
	//TranMats TM; //Handles the view and projection matricies and uniform buffer object

	Environment Env; //Handles environment variables such as directional lights and background colors
	//Lighting Lits; //Handles point lighting currently, add spot lighting eventually
	ConsoleData CDat; // Handles console data
	ScreenWriter SW;//Handles text rendering
	//RandomInt RandI;//Random Generator
	
	bool keys[1024];//Bools for keys pressed.
	bool firstMouse;//Bools for mouse use.

	float offsetNormalization;

	//Console Variables
	int consoleRequest;

	public:
	Engine_Init() {};
	~Engine_Init() {};

        void Engine_Run(void) 
        {
                cout << "Initializing Engine...\n";
		for (int i = 0; i < 1024; ++i) {keys[i] = false;}; //Initialize the keys to false.
		firstMouse = true; //Initialize firstMouse to true *Handles mouse movements*.
		consoleRequest = 0; //DEFAULT
		//Lits.MaxPointLights = 3;//Make modifiable
		camera.aspect=15.0f;
		offsetNormalization = 0.00025;
                cout << "Running Display...\n";
                RunDisplay();//Begin the rendering calls, this function contains the main loop.
        };

	void RunDisplay(void);

	private:
	//Virtual Member Functions for Input Handling
        virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
        virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	//Engine Member Functions
	void ProcessDrawArray(void);
	void ProcessDrawStructs(DrawStruct *DrawData,int Size);
	void ProcessInstancedDrawStructs(vector<DrawInstancedStruct> &DrawInstData);
	//void ProcessDrawStructs(vector<DrawStruct> DrawData);
        void Do_Movement(void);//Handles any movement inputs.

        //************************
        //Error Handling Functions
        //************************
	void GLErrorCatch(string ErrorPos);

	//*****************
	//Console Functions
	//*****************
	void ConsoleFunctions(int key,int action);//Main loop calls this when a key is pressed and console == 3
	void Screen_Console(ScreenWriter &SW);//Checks for console calls

	
	void ConsoleKeySet(int key,int action);//Input handling for console
	void ShowMenu(int key,int action);//Input handling for loading entities

	void RenderConsoleHistory(void);

	void CommandEngine(string command);//Engine for carrying out commands
	void Printer(string param);
	void loadOPF(string param);
	void loader(string param);
	void Modify(string subcommand);
	void Generator(string param);
	void SkyboxManipulator(string subcommand);
	void help(string helpfile);

	//Menu Functions
	void SaveEntity(string *data);
	void ModEntity(string *data);
	void ModEnv(string *data);
	void ModPLight(string *data);
	void ModGravEnt(string *data);

	//Generator Functions
	int GenerateGravEntity(string name,int type);
	void SetGravEnt(int GID1,int GID2,float MagR);

	//Screen Printer Functions
	void PrintScreenTimers (ScreenWriter &SW,fpsTimer &ft)
	{
		ft.printfpsTimer(dt.t);
		float Font = props.FontSize;
		float Line = props.LineSpace;
		SW.RenderText(ft.mspf,props.WinWidth - 175.0f, props.WinHeight - 35.0f,Font,glm::vec3(0.0f,1.0f,0.0f));
		SW.RenderText(ft.fps,props.WinWidth - 125.0f, props.WinHeight - (35.0f + Line),Font,glm::vec3(0.0f,1.0f,0.0f));
		SW.RenderText(dt.tmult,props.WinWidth - 125.0f, props.WinHeight - (35.0f + 2.0f * Line),Font,glm::vec3(0.0f,1.0f,0.0f));
	}
};

#endif
