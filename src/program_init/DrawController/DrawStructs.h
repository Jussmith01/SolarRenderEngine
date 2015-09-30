#define GLEW_STATIC
#ifndef DrawStructs
#define DrawStructs

#include "../mainHeaders.h"

//******************************************//
//                Draw Structure            //
//******************************************//
struct DrawStruct
{
	int OPCID;
	int OBJID;
	glm::mat4 ModelMat;
	glm::vec3 objPos;

        //Class Blank Constructor
        DrawStruct() {};

        //Class Assignment Constructor
        DrawStruct(const DrawStruct& instance)
        {
                this-> OPCID = instance.OPCID;
                this-> OBJID = instance.OBJID;
                this-> ModelMat = instance.ModelMat;
                this-> objPos = instance.objPos;
        };

        //Class Assignment
        DrawStruct& operator=(const DrawStruct& instance)
        {
                this-> OPCID = instance.OPCID;
                this-> OBJID = instance.OBJID;
                this-> ModelMat = instance.ModelMat;
                this-> objPos = instance.objPos;
                return *this;
        };
};

/*struct DrawArray
{
	//After the entity controllers populate these
	//arrays, the lighting controller uses them to
	//determine scene lighting and then the Draw
	//controller uses them to draw the scene. After
	//that the interface controller uses them to 
	//draw the interface and icons to the screen.
        vector<DrawStruct> DrawDataD1;//Depth Level 1
        vector<DrawStruct> DrawDataD2;//Depth Level 2
        vector<DrawStruct> DrawDataD3;//Depth Level 3

        **************************
        //Class Peripheral Functions
        **************************
	//This function adds a DrawStruct type to the correct DLevel array
	void AddEntityToDrawArray(DrawStruct DS,int DLevel)
	{
	 	#pragma omp critical
		{
			switch(DLevel)
			{
				case 0: {DrawDataD1.push_back(DS);break;}
				case 1: {DrawDataD2.push_back(DS);break;}
				case 2: {DrawDataD3.push_back(DS);break;}
			}
		}
	};

        //This function clears the arrays
        void ClearDrawArrays()
        {
        	DrawDataD1.clear();
        	DrawDataD2.clear();
        	DrawDataD3.clear();
        };
};*/

struct DrawArray
{
        //After the entity controllers populate these
        //arrays, the lighting controller uses them to
        //determine scene lighting and then the Draw
        //controller uses them to draw the scene. After
        //that the interface controller uses them to 
        //draw the interface and icons to the screen.
        DrawStruct *DrawDataD1;//Depth Level 1
        DrawStruct *DrawDataD2;//Depth Level 2
        DrawStruct *DrawDataD3;//Depth Level 3

	int ND1;int ND2;int ND3;

	//Constructor
	DrawArray()
	{
		ND1=0;ND2=0;ND3=0;
		AllocateMemoryDrawArrays(10000);
	};

        //**************************
        //Class Peripheral Functions
        //**************************

	//Allocate the memory
	void AllocateMemoryDrawArrays(int NO)
        {
		DrawDataD1 = new DrawStruct [NO];
		DrawDataD2 = new DrawStruct [NO];
		DrawDataD3 = new DrawStruct [NO];
	};

	//Add an entry to the array
        void AddEntityToDrawArray(DrawStruct DS,int DLevel)
	{
		{
			#pragma omp critical
			{
                        	switch(DLevel)
                        	{
                        	        case 0: {if (!(ND1>=10000)) {DrawDataD1[ND1]=DS;++ND1;}break;}
                        	        case 1: {if (!(ND2>=10000)) {DrawDataD2[ND2]=DS;++ND2;}break;}
                        	        case 2: {if (!(ND3>=10000)) {DrawDataD3[ND3]=DS;++ND3;}break;}
                        	}
			}
                }
	};

	void DrawArrayReset()
	{
		ND1=0;ND2=0;ND3=0;
	};
        //This function adds a DrawStruct type to the correct DLevel array
        void ClearDrawArrays()
        {
		delete [] DrawDataD1;
		delete [] DrawDataD2;
		delete [] DrawDataD3;
		ND1=0;ND2=0;ND3=0;
        };
};

//******************************************//
//         Draw Instanced Structure         //
//******************************************//
//
//
struct DrawInstancedStruct
{
        int OPCID;
        int OBJID;
	glm::vec3 SunDirection;
	int N;//Number of objects being instanced

        //Class Blank Constructor
        DrawInstancedStruct() {};

        //Class Assignment Constructor
        DrawInstancedStruct(const DrawInstancedStruct& instance)
        {
                this-> OPCID = instance.OPCID;
                this-> OBJID = instance.OBJID;
                this-> N = instance.N;
                this-> SunDirection = instance.SunDirection;
        };

        //Class Assignment
        DrawInstancedStruct& operator=(const DrawInstancedStruct& instance)
        {
                this-> OPCID = instance.OPCID;
                this-> OBJID = instance.OBJID;
                this-> N = instance.N;
                this-> SunDirection = instance.SunDirection;
                return *this;
        };
};

struct DrawInstancedArray
{
        //After the entity controllers populate these
        //arrays, the lighting controller uses them to
        //determine scene lighting and then the Draw
        //controller uses them to draw the scene. After
        //that the interface controller uses them to 
        //draw the interface and icons to the screen.
        vector<DrawInstancedStruct> DrawInstDataD1;//Depth Level 1
        vector<DrawInstancedStruct> DrawInstDataD2;//Depth Level 2
        vector<DrawInstancedStruct> DrawInstDataD3;//Depth Level 3

        //**************************
        //Class Peripheral Functions
        //**************************
        //This function adds a DrawStruct type to the correct DLevel array
        void AddInstEntitiesToDrawArray(DrawInstancedStruct DS,int DLevel)
        {
                {
                        switch(DLevel)
                        {
                                case 0: {DrawInstDataD1.push_back(DS);break;}
                                case 1: {DrawInstDataD2.push_back(DS);break;}
                                case 2: {DrawInstDataD3.push_back(DS);break;}
                        }
                }
        };

        //This function clears the arrays
        void ClearInstDrawArrays()
        {
                DrawInstDataD1.clear();
                DrawInstDataD2.clear();
                DrawInstDataD3.clear();
        };
};

#endif
