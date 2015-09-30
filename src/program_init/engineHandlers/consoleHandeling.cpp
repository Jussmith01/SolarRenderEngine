#define GLEW_STATIC

//#include "../mainHeaders.h"
#include "../EngineHeader.h"
//#include "../objectControllers/objectPhysics.h"

using namespace std;

//-------------------------------------------//
//             Console Functions             //
//-------------------------------------------//
void Engine_Init::ConsoleFunctions(int key,int action)
{
	if (CDat.Console_Call)
		{ConsoleKeySet(key,action);}

	if (CDat.Menu_Call)
		{ShowMenu(key,action);}
};

//-------------------------------------------//
//             Console Functions             //
//-------------------------------------------//
void Engine_Init::ConsoleKeySet(int key,int action)
{
	//cout << "key: " << key << " action: " << action << "\n";
	
	//Set shift key
	if ((key == 340 || key == 344) && action == 1)
		{CDat.shift = true;}
	else if ((key == 340 || key == 344) && action == 0)
		{CDat.shift = false;}

	//Set caplock
        if (key == 280 && action == 1)
        {
		if (CDat.caps)
			{CDat.caps = false;}
		else
			{CDat.caps = true;}
	}

	//Read keys pressed
	if ((key >= 32 && key <= 96) && action == 1)
	{
		CDat.command += GetLetter(key,CDat.shift,CDat.caps);
	}

        //Delete Letter from end with backspace
        if ((key == 259 || key == 261) && (action == 1 || action == 2))
        {
		size_t end = CDat.command.size();
                CDat.command = CDat.command.substr(0,end - 1);
        }

	//Key up to reload last command
        if (key == 265 && (action == 1 || action == 2))
        {
                ++CDat.chpos;
		if (CDat.chpos > 19)
		{
			CDat.chpos = 19;
			CDat.command = CDat.commandhistory[CDat.chpos];
		} else {
			CDat.command = CDat.commandhistory[CDat.chpos];
		}
        }

        //Key down to reload last command
        if (key == 264 && (action == 1 || action == 2))
        {
                --CDat.chpos;
		if (CDat.chpos < 0)
		{
			CDat.chpos = -1;
			CDat.command = "";
		} else {
                	CDat.command = CDat.commandhistory[CDat.chpos];
		}
        }

        //Enter Pressed - Send command to command engine and reset commands
        if (key == 257 && action == 1)
        {
		CDat.Buffer(" "); CDat.Buffer(CDat.command);
		CDat.CommandBuffer(CDat.command);
		CommandEngine(CDat.command);
		CDat.command = "";
		CDat.chpos = -1;
	}


	//If F1 is pressed, close console
	if ((key == 290 || key == 256) && action == 1)
	{
		consoleRequest = 0;CDat.command = "";CDat.chpos = -1;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
};

//-------------------------------------------//
//          Console Entity Loader            //
//-------------------------------------------//
void Engine_Init::ShowMenu(int key,int action)
{
        //cout << "key: " << key << " action: " << action << "\n";

        //Set shift key
        if ((key == 340 || key == 344) && action == 1)
                {CDat.shift = true;}
        else if ((key == 340 || key == 344) && action == 0)
                {CDat.shift = false;}

        //Set caplock
        if (key == 280 && action == 1)
        {
                if (CDat.caps)
                        {CDat.caps = false;}
                else
                        {CDat.caps = true;}
        }

        //Read keys pressed
        if ((key >= 32 && key <= 96) && action == 1)
        {
                CDat.MC.data[CDat.MC.cursorpos] += GetLetter(key,CDat.shift,CDat.caps);
        }

        //Tab to change cursor pos
        if (key == 258 && (action == 1 || action == 2))
        {
		if (CDat.shift == true)
		{
			CDat.MC.cursorpos = (CDat.MC.cursorpos - 1) % CDat.MC.totvar;
			if (CDat.MC.cursorpos == -1)
				{CDat.MC.cursorpos = CDat.MC.totvar-1;}
		}
		else
			{CDat.MC.cursorpos = (CDat.MC.cursorpos + 1) % CDat.MC.totvar;}
        }

        // Right arrow to change cursor pos
        if (key == 262 && (action == 1 || action == 2))
        	{CDat.MC.cursorpos = (CDat.MC.cursorpos + 1) % CDat.MC.totvar;}

        // Right arrow to change cursor pos
        if (key == 263 && (action == 1 || action == 2))
        {
        	CDat.MC.cursorpos = (CDat.MC.cursorpos - 1) % CDat.MC.totvar;
                if (CDat.MC.cursorpos == -1) {CDat.MC.cursorpos = CDat.MC.totvar-1;}
	}

        //Delete Letter from end with backspace
        if ((key == 259 || key == 261) && (action == 1 || action == 2))
        {
                size_t end = CDat.MC.data[CDat.MC.cursorpos].size();
                CDat.MC.data[CDat.MC.cursorpos] = CDat.MC.data[CDat.MC.cursorpos].substr(0,end - 1);
        }

        //Enter Pressed - Send command to command engine and reset commands
        if (key == 257 && action == 1 && CDat.Enter)
        {
		cout << "CALL CHECK\n";
		//List Menu Functions
		//if(CDat.SetEntity_Call) {SaveEntity(CDat.MC.data);}
		//if(CDat.SetEntityMod_Call) {ModEntity(CDat.MC.data);}
		if(CDat.SetEnv_Call) {ModEnv(CDat.MC.data);}
		//if(CDat.SetPlight_Call) {ModPLight(CDat.MC.data);}
		//if(CDat.SetGrav_Call) {ModGravEnt(CDat.MC.data);}

		//Reset bools and cleanup
                CDat.Console_Call = true;
                CDat.Menu_Call = false;

                //Menu Bools
                if(CDat.SetEntity_Call) {CDat.SetEntity_Call=false;}
                if(CDat.SetEntityMod_Call) {CDat.SetEntityMod_Call=false;}
                if(CDat.SetEnv_Call) {CDat.SetEnv_Call=false;}
                if(CDat.SetPlight_Call) {CDat.SetPlight_Call=false;}
                if(CDat.SetGrav_Call) {CDat.SetGrav_Call=false;}

                CDat.MC.destroy_menu();
                CDat.command = "";CDat.chpos = -1;
        }


        //If F1 is pressed, close console
        if ((key == 290 || key == 256) && action == 1)
        {	
		//Reset bools and clean up
		CDat.Console_Call = true;
		CDat.Menu_Call = false;
		
		//Menu Bools
		if(CDat.SetEntity_Call) {CDat.SetEntity_Call=false;}
		if(CDat.SetEntityMod_Call) {CDat.SetEntityMod_Call=false;}
                if(CDat.SetEnv_Call) {CDat.SetEnv_Call=false;}
                if(CDat.SetPlight_Call) {CDat.SetPlight_Call=false;}
                if(CDat.SetGrav_Call) {CDat.SetGrav_Call=false;}

		CDat.MC.destroy_menu();
                CDat.command = "";CDat.chpos = -1;
        }

	CDat.Enter=true;
};


//-------------------------------------------//
//             Console Functions             //
//-------------------------------------------//
void Engine_Init::Screen_Console(ScreenWriter &SW)
{
	CDat.CursorBlink(dt.t);

        if (consoleRequest > 0 && consoleRequest < 3)
        {
               	//SW.RenderText("Console Open",25.0f, props.WinHeight - 25.0f,0.3f,glm::vec3(1.0f,1.0f,1.0f));
		++consoleRequest;
		CDat.Console_Call = true;
	}
	else if (consoleRequest == 3)
        {
		float Font = props.FontSize;
		float Line = props.LineSpace;
                SW.RenderText("Console Open",25.0f + Line, props.WinHeight - (25.0f + Line),Font*1.15f,glm::vec3(1.0f,1.0f,1.0f));

        	string car = "> ";

		RenderConsoleHistory();

		if (CDat.Console_Call)
		{
        		SW.RenderText(car + CDat.command + CDat.cursor,15.0f + Line, 15.0f + Line,Font,glm::vec3(1.0f,1.0f,1.0f));
		}

		if (CDat.Menu_Call)
		{
			int CentPos = props.WinWidth / 2;
			for (int i = 0; i < CDat.MC.MD.totlines;++i)
        		{SW.RenderText(CDat.MC.createline(i,CDat.cursor),CentPos, 200.0f - i * Line,Font,glm::vec3(1.0f,1.0f,1.0f));}	
		}
        }

};

//------------------------------------------
//         Render Console History
//------------------------------------------
void Engine_Init::RenderConsoleHistory()
{
	string car = "> ";
        float Font = props.FontSize;
        float Line = props.LineSpace;

	for (int i = 0; i < 50;++i)
	{	
		if (CDat.consolehistory[i].size() > 0)
		{SW.RenderText(car + CDat.consolehistory[i],15.0f + Line, (15.0f + 2*Line) + i * Line * 0.85,Font*0.8,glm::vec3(1.0f,1.0f,1.0f));}
	}
};

//------------------------------------------
//		Command Engine
//------------------------------------------
void Engine_Init::CommandEngine(string command)
{
	string func; string param;

	bool echk = parseLine(command,func,param);
	stringstream sl;
	sl << "Function: " << func << " -- Parameter: " << param;
	CDat.Buffer(sl.str());

	if (echk) {CDat.Buffer("   Syntax Error");}
        else if (func.compare("help") == 0)
                {help("consolehelp.txt");}
        else if (func.compare("loadopf") == 0)
                {loadOPF(param);}
        else if (func.compare("load") == 0)
                {loader(param);}
        else if (func.compare("skybox") == 0)
                {SkyboxManipulator(param);}
        else if (func.compare("mod") == 0)
                {Modify(param);}
	else if (func.compare("list") == 0)
		{Printer(param);}
        else if (func.compare("setent") == 0)
        {
		CDat.MC.init_menu(CDat.EntMD);
		CDat.SetEntity_Call=true;CDat.Menu_Call=true;
		CDat.Enter=false;CDat.Console_Call=false;
	}
        else if (func.compare("exit") == 0)//Exit console
        {
		consoleRequest = 0;CDat.command = "";
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
        else if (func.compare("term") == 0)//Terminate Program
        {
		glfwSetWindowShouldClose(window, GL_TRUE);
	} else {
		stringstream snf;
		snf << "Function \"" << func << "\" not found...";
		CDat.Buffer(snf.str());
	}

};

//------------------------------------------
//            Modify Function
//------------------------------------------
void Engine_Init::Modify(string subcommand)
{
	string subfunc; string param;

        bool echk = parseLine(subcommand,subfunc,param);
        stringstream sl;
        sl << "  SubFunction: " << subfunc << " -- Parameter: " << param;
        CDat.Buffer(sl.str());

	if (echk) {CDat.Buffer("   Syntax Error");}

	//else {cout << "   SubFunction \"" << subfunc << "\" not found...\n";}
};

//------------------------------------------
//		Print Objects
//------------------------------------------
void Engine_Init::Printer(string param)
{
	if (param.compare("objects") == 0)
	{
        	CDat.Buffer("Listing Loaded Objects: ");
        	unsigned int Nopc = OPC.size();
        	for (unsigned int i = 0; i < Nopc; ++i)
        	{
                	unsigned int No = OPC[i].NObjs;
       	        	for (unsigned int j = 0; j < No; ++j)
        	        {
				stringstream ss;
        	                ss << "  " 
				<< "OPC(" << i << "): " 
				<< OPC[i].ObjLib[j].Objname 
				<< " ID(" << j << ")";
				CDat.Buffer(ss.str());
        	        }
		}
	}
       	else if (param.compare("models") == 0)
        {
                CDat.Buffer("Listing Loaded Models: ");
                unsigned int Nopc = OPC.size();
                for (unsigned int i = 0; i < Nopc; ++i)
                {
                        unsigned int Nm = OPC[i].NMod;
                        for (unsigned int j = 0; j < Nm; ++j)
                        {
				stringstream ss;
                                ss << "  " 
				<< "OPC(" << i 
				<< ") MID(" << j 
				<< "): " << OPC[i].ModLib[j].objfile 
				<< " NMesh(" << OPC[i].ModLib[j].Nmesh << ")"; 
				CDat.Buffer(ss.str());
                        }
                }
	}
        else if (param.compare("shaders") == 0)
        {
                CDat.Buffer("Listing Loaded Shaders: ");
                int Nsf = (int)SCC.ShdrFiles.size();
                for (int i = 0; i < Nsf; ++i)
                {
			stringstream ss;
                        ss << "  "
			<< "ShaderFile: " << SCC.ShdrFiles[i] 
			<< " ShaderLocation(" << SCC.DetermLoc[i] << ")";
			CDat.Buffer(ss.str());
                }
        } else {
		stringstream ss;
		ss << "Parameter \"" << param << "\" not found in \"list\" function...\n";
		CDat.Buffer(ss.str());
	}
};

//------------------------------------------
//          Skybox Manipulator
//------------------------------------------
void Engine_Init::SkyboxManipulator(string subcommand)
{
        CDat.Buffer("Modifying Skybox: ");

	string subfunc; string param;

        bool echk = parseLine(subcommand,subfunc,param);
        stringstream sl;
        sl << "  SubFunction: " << subfunc << " -- Parameter: " << param;
        CDat.Buffer(sl.str());

	//Define -  Entirely loads a skybox
	if (subfunc.compare("define") == 0)
	{
		if(skybox.CPUload || skybox.GPUload)
		{
			CDat.Buffer("Error, GPU data not loaded. Cannot Delete.");
		} else {
			skybox.SetupClass(param);
			skybox.LoadSkyboxDataToCPU();
			skybox.LoadSkyboxDataToGPU();
			stringstream ss;
			ss << "Skybox " << param << " loaded.";
			CDat.Buffer(ss.str());
		}

        } else if (subfunc.compare("delete") == 0){
                if (skybox.GPUload && skybox.CPUload)
                {
                        skybox.DeleteSkyboxFromGPU();
			skybox.DeleteSkyboxFromCPU();
                        CDat.Buffer("Skybox Data Removed, you may now load a new Skybox.");
                } else {CDat.Buffer("Error, GPU data not loaded. Cannot Delete.");}


	} else if (subfunc.compare("deletegpu") == 0){
		if (skybox.GPUload)
		{
			skybox.DeleteSkyboxFromGPU();
			CDat.Buffer("Skybox GPU Data Removed.");
		} else {CDat.Buffer("Error, GPU data not loaded. Cannot Delete.");}

        } else if (subfunc.compare("loadgpu") == 0){
		if (skybox.CPUload)
		{
                	if (!skybox.GPUload)
                	{
                	        skybox.LoadSkyboxDataToGPU();
				CDat.Buffer("Skybox GPU Data Loaded.");
                	} else {CDat.Buffer("Error, GPU data currently loaded.");}
                } else {CDat.Buffer("Error, No CPU Data Loaded. Use skybox:loadcpu:[FILENAME] to do this.");}
        } else if (subfunc.compare("deletecpu") == 0){
                if (skybox.CPUload)
                {
                        skybox.DeleteSkyboxFromCPU();
			CDat.Buffer("Skybox CPU Data Removed.");
                } else {CDat.Buffer("Error, CPU data not loaded. Cannot Delete.");}

        } else if (subfunc.compare("loadcpu") == 0){
                if (!skybox.CPUload)
                {
			skybox.texname=param;
                        skybox.LoadSkyboxDataToCPU();
                        stringstream ss;
                        ss << "Skybox " << param << " loaded to CPU.";
                        CDat.Buffer(ss.str());
                        CDat.Buffer("   Must load to the gpu with skybox:loadgpu to show.");

                } else {CDat.Buffer("Error, CPU data already loaded.");}
        } else if (subfunc.compare("help") == 0){
		help("skyboxhelp.txt");
        } else {
        	stringstream ss;
                ss << "   Subfunction " << subfunc << " not found. Avaliable subfunctions are:";
                CDat.Buffer(ss.str());
                CDat.Buffer("     help");
                CDat.Buffer("     define:[filename]");
                CDat.Buffer("     deletegpu");
                CDat.Buffer("     loadgpu");
                CDat.Buffer("     deletecpu");
                CDat.Buffer("     loadcpu:[filename]");
                CDat.Buffer("     ");
	}

        if (echk) {CDat.Buffer("   Syntax Error");}

};

//------------------------------------------
//          OPF Console Loader
//------------------------------------------
void Engine_Init::loadOPF(string param)
{
	CDat.Buffer("Loading OPF File...");
	OPC.push_back(param); 
	size_t N = OPC.size()-1;
	if (OPC[N].OpfLoader())
		{CDat.Buffer("OPF Loading Complete");}
	else 
	{
		CDat.Buffer("!OPF Failed to Load!");
		OPC.erase(OPC.end());
	}
};

//------------------------------------------
//          Shader Console Loader
//------------------------------------------
void Engine_Init::loader(string param)
{
	if (param.compare("shaders") == 0)
        {
		CDat.Buffer("Loading Shaders...");
       		SCC.ShaderLoader();
		CDat.Buffer("Shader Loading Complete");
        } else {
                stringstream ss;
                ss << "Parameter \"" << param << "\" not found in \"load\" function...\n";
                CDat.Buffer(ss.str());
        }
};

//------------------------------------------
//         Mod Environment Function
//------------------------------------------
void Engine_Init::ModEnv(string *data)
{
	Env.sunPos = glm::vec3(atof(data[0].c_str()),atof(data[1].c_str()),atof(data[2].c_str()));

	Env.sunColor = glm::vec3(atof(data[3].c_str()),atof(data[4].c_str()),atof(data[5].c_str()));
	Env.bgColor = glm::vec3(atof(data[7].c_str()),atof(data[8].c_str()),atof(data[9].c_str()));

	Env.intensity = atof(data[6].c_str());
};

//------------------------------------------
//         	Help Function
//------------------------------------------
void Engine_Init::help(string helpfile)
{
	CDat.Buffer("Command Help File Output: ");
	string line;

	stringstream ss;

	ss << "bin/menus/console/" << helpfile;
	CDat.Buffer(ss.str());

	ifstream myfile (ss.str().c_str());
	if (myfile.is_open())
	{
		 while ( getline (myfile,line) )
		{
			//cout << "LINE: " << line << endl;
	 		CDat.Buffer(line);
		}
		myfile.close();
	} else cout << "Unable to open file"; 
};

