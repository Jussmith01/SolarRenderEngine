#define GLEW_STATIC
#ifndef objloader_class
#define objloader_class

#include "../mainHeaders.h"
//#include "../math/jmath.h"
//#include "shader.h"
#include "object_texture.h"
//#include "object_coords.h"
//#include "object_light.h"
#include "../basicStructures/base_classes.h"

using namespace std;

//**********************************
//Begin Mesh class -- Creates meshes
//**********************************
class Mesh
{
        public:
        int mID;
        vector<Vertex> vertices;
	vector<VertexwTang> verticeswtang;
        vector<GLuint> indices;
        Material materials;
	bool TBN;//Bool for tangent vectors

        //Constructor
        Mesh(int mID) {this->mID = mID;};

        //Destructor
        ~Mesh() {};

	void SetMeshOnDevice()
	{
		if (!TBN)
			{setupMeshRegular();}
		else
			{setupMeshWithTangents();}
	};

	// Render the mesh
        //*********************************************
        //         Draws the Mesh (Singular)
        //*********************************************
	void Draw()
	{
	        // Draw mesh
		//cout << "Drawing Mesh: " << mID << endl;
	        glBindVertexArray(this->VAO);
		//cout << "TEST1" << endl;
	        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		//cout << "TEST2" << endl;
	        glBindVertexArray(0);
		//cout << "TEST3" << endl;
	};

        // Render the mesh
        //*********************************************
        //         Draws the Mesh (Instanced)
        //*********************************************
        void DrawInstanced(int NumberInstanced)
        {
                // Draw mesh
                //cout << "Drawing " << NumberInstanced << " Instanced Meshes: " << mID << endl;
                glBindVertexArray(this->VAO);
                //cout << "TEST1" << endl;
               	//glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, NumberInstanced);
                //cout << "TEST2" << endl;
                glBindVertexArray(0);
                //cout << "TEST3" << endl;
        };

        private:
	/*  Render data  */
	GLuint VAO, VBO, EBO, IPBO;
	GLuint VAOidx;
	void* ptr;

        //*********************************************
        //            Setup a Regular Mesh
        //*********************************************
        void setupMeshRegular()
        {
		cout << "Setting Up Regular Mesh" << endl;
                // Create buffers/arrays
                glGenVertexArrays(1, &this->VAO);
                glGenBuffers(1, &this->VBO);
                glGenBuffers(1, &this->EBO);

                glBindVertexArray(this->VAO);
                // Load data into vertex buffers
                glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
                // A great thing about structs is that their memory layout is sequential for all its items.
                // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
                // again translates to 3/2 floats which translates to a byte array.
                glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

                // Set the vertex attribute pointers
                // Vertex Positions
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
                // Vertex Tex Coords
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture));
                // Vertex Normals
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		
		VAOidx = 2;

		glBindVertexArray(0);
        };

        //*********************************************
        //    Setup Mesh with Calculated Tangents
        //*********************************************
        void setupMeshWithTangents()
        {
		cout << "Setting Up Mesh With Tangents" << endl;
                // Create buffers/arrays
                glGenVertexArrays(1, &this->VAO);
                glGenBuffers(1, &this->VBO);
                glGenBuffers(1, &this->EBO);

                glBindVertexArray(this->VAO);
                // Load data into vertex buffers
                glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
                // A great thing about structs is that their memory layout is sequential for all its items.
                // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
                // again translates to 3/2 floats which translates to a byte array.
                glBufferData(GL_ARRAY_BUFFER, this->verticeswtang.size() * sizeof(VertexwTang), &this->verticeswtang[0], GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

                // Set the vertex attribute pointers
                // Vertex Positions
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexwTang), (GLvoid*)0);
                // Vertex Tex Coords
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexwTang), (GLvoid*)offsetof(VertexwTang, texture));
                // Vertex Normals
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexwTang), (GLvoid*)offsetof(VertexwTang, normal));
                // Vertex Tangent
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexwTang), (GLvoid*)offsetof(VertexwTang, tangent));
                // Vertex BiTangent
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexwTang), (GLvoid*)offsetof(VertexwTang, bitangent));

		VAOidx = 4;

		glBindVertexArray(0);
        };

	public:
	//*********************************************
	//	Add an instancing VBO to the VAO
	//*********************************************
        void setupInstanceArray(vector<glm::mat4> &modelMatrices)
	{
		cout << "Setting Up Instancing VBO\n";
		int amount = modelMatrices.size();

		// Vertex Buffer Object
		glBindVertexArray(VAO);
		glGenBuffers(1, &IPBO);
		cout << "Amount: " << amount << " VAO/IPBO: " << VAO << "/" << IPBO << endl;
		cout << "ModMatPos: " << modelMatrices[0][3][0] << "," << modelMatrices[0][3][1] << "," << modelMatrices[0][3][2] << endl; 
		glBindBuffer(GL_ARRAY_BUFFER, IPBO);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STREAM_DRAW);
		// Vertex Attributes
		glEnableVertexAttribArray(VAOidx+1); 
		glVertexAttribPointer(VAOidx+1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)0);
		glEnableVertexAttribArray(VAOidx+2); 
		glVertexAttribPointer(VAOidx+2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(VAOidx+3); 
		glVertexAttribPointer(VAOidx+3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(VAOidx+4); 
		glVertexAttribPointer(VAOidx+4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(3 * sizeof(glm::vec4)));
	
		glVertexAttribDivisor(VAOidx+1, 1);
		glVertexAttribDivisor(VAOidx+2, 1);
		glVertexAttribDivisor(VAOidx+3, 1);
		glVertexAttribDivisor(VAOidx+4, 1);
		//ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		glBindVertexArray(0);
	};

        //*********************************************
        //      Add an instancing VBO to the VAO
        //*********************************************
        void UpdateInstancedModelMatrices(vector<glm::mat4> &modelMatrices)
        {
		// Now copy data into memory
                int amount = modelMatrices.size();
		//long int MemSize = amount * sizeof(glm::mat4);

		//if (MemSize > 1.0E6)
		//{
               // 	glBindBuffer(GL_ARRAY_BUFFER, IPBO);
		//	glBufferSubData(IPBO,0,MemSize,&modelMatrices[0]);
 		//} else {
	
	        	//GLenum ErrorChk1 = glGetError();
			//double timeStartMain = glfwGetTime();
                	//int amount = modelMatrices.size();

                	glBindBuffer(GL_ARRAY_BUFFER, IPBO);
			// Get pointer
			GLfloat* NullPtr = NULL;
			glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), NullPtr, GL_STREAM_DRAW);
			//double timeMapBuffer = glfwGetTime();
	
		        GLenum ErrorChk2 = glGetError();
		        if (ErrorChk2 != GL_NO_ERROR)
		                {cout << "!!GL Error " << ErrorChk2 << " Detected!! :: " << gluErrorString(ErrorChk2) << " :: In ErrorPos1 " << "\n";}

			
			GLvoid* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		        GLenum ErrorChk3 = glGetError();
		        if (ErrorChk3 != GL_NO_ERROR)
		                {cout << "!!GL Error " << ErrorChk3 << " Detected!! :: " << gluErrorString(ErrorChk3) << " :: In ErrorPos2 " << "\n";}

			//timeMapBuffer = (glfwGetTime() - timeMapBuffer)*1000.0;

			if (ptr == NULL)
				{cout << "ERROR::MAPPEDBUFFERPOINTER::Problem Mapping Buffer to Pointer" << endl;}

			// Now copy data into memory
			//double timeStart = glfwGetTime();
	
			try
				{memcpy(ptr, &modelMatrices[0], amount * sizeof(glm::mat4));}
        	        catch ( exception e )
        	                {cout << "ERROR::INSTANCEDMEMCPY::Problem Loading Data" << endl;}

			//double MemCpyTime = glfwGetTime() - timeStart;
			//double MemCpySpeed = (double)MemSize / (MemCpyTime);
	
			// Make sure to tell OpenGL we're done with the pointer
			if(!glUnmapBuffer(GL_ARRAY_BUFFER))
				{cout << "!!!Error: Unable to unmap VBO!!!\n";};
	
			//cout << "Instancing Memcopy Speed: " << MemCpySpeed / (double)(1024*1024*1024) << " GB/s - Total Time(ms): " << (glfwGetTime() - timeStartMain) * 1000.0 << " MemCpySize(MB): " << MemSize/(1024.0*1024.0) << " MemCpyTime(ms): " << MemCpyTime*1000 << " BufferMapTime(ms): " << timeMapBuffer << endl;
		//}
	};

        //*********************************************
        //      	Set Materials
        //*********************************************
	void setMaterial (GLint Prog)
	{
                GLint matAmbientLoc = glGetUniformLocation(Prog, "light.ambient");
                GLint matDiffuseLoc = glGetUniformLocation(Prog, "light.diffuse");
                GLint matSpecularLoc = glGetUniformLocation(Prog, "light.specular");
                GLint matShineLoc = glGetUniformLocation(Prog, "light.shininess");

		//cout << "Ambient [" << materials.Ka.x << "," << materials.Ka.y << "," << materials.Ka.z << "]\n";

                glUniform3f(matAmbientLoc, materials.Ka.x, materials.Ka.y, materials.Ka.z);
                glUniform3f(matDiffuseLoc, materials.Kd.x, materials.Kd.y, materials.Kd.z);
                glUniform3f(matSpecularLoc, materials.Ks.x, materials.Ks.y, materials.Ks.z);
                glUniform1f(matShineLoc, materials.shine);
	};	
};

//*********************************************
//Begin Object Loader class -- loads .obj files
//*********************************************
struct faceIndex
{
	int FID; //Face data
	int verts[3];//Stores the vertex index
	int tex[3];//Stores the texture index
	int normal[3];//Stores the normal index

	faceIndex(int FID) {this->FID = FID;};
};

struct materialIndex
{
	string mtlname;//Material name
	float Ns;// Shine
        glm::vec3 Ka;//Ambient lighting vector
	glm::vec3 Kd;//Diffuse lighting vector
	glm::vec3 Ks;//Specular lighting vector
	float Ni;//Intensity??
	float d;//Dont know????
	string texfilename;//Texture filename
        materialIndex(string mtlname) {this->mtlname = mtlname;};
};

struct meshData
{
	string objname;
        int Nv;int Nt;int Nn;
	vector<glm::vec3> verticies;//Stores vertex positions
	vector<glm::vec2> textures;//Stores vertex positions
	vector<glm::vec3> normals;//Stores vertex positions
	string mtlname;//Stores the material name
        int Nf; vector<faceIndex> facedat;//See faceIndex class
        //vector<materialIndex> mtldat;//See matertialIndex class
        meshData(string objname) {this->objname = objname;Nv=0;Nt=0;Nn=0;Nf=0;};
};

//Tanjent and binormal data, separate because we dont always use it
struct TBNdata
{
	vector<glm::vec3> tangent;//Stores the calculated tangent vector
	vector<glm::vec3> bitangent;//Stores the calculated bitangent vector
};

class objLoader
{
	//public:
	vector<string> data;
	int lines;

	string mtlfilename;

	int Nmesh;
	vector<meshData> md;

	int Nm;//Stores the material name
        vector<materialIndex> mtldat;//See matertialIndex class

	int tNv; int tNt; int tNn;

	public:
	int RtnNumMesh()
	{
		return Nmesh;
	}

	//Reads data from input filename and saves it to data vector
	void readData(string filename)
	{
		lines = 0;
		string line;
		stringstream fd;
		fd << "bin/models/" << filename;
		ifstream file (fd.str().c_str());
		if (file.is_open())
		{
			while ( getline (file,line) )
				{data.push_back(line);++lines;}
			file.close();
  		} else cout << "Error: Unable to open file: " << fd.str().c_str() << "\n"; 
	};

	//Function Parses the faces
        faceIndex parseFaces(int FID,string line)
        {
                vector<string> val;
                size_t spot = line.find_first_of(" ");
                vector<size_t> pos;
                int c = 0;
                while (spot!=string::npos)
                {
                        //cout << spot << endl;
                        pos.push_back(spot);
                        spot = line.find_first_of(" ",spot+1);
                        ++c;
                }

    		for (int i = 0; i < c-1; ++i)
                        {val.push_back(line.substr(pos[i],pos[i+1] - pos[i]));}

	       	val.push_back(line.substr(pos[c-1]));

                //for (int i = 0; i < c; ++i)
                //        {cout << i << " " << val[i] << endl;}

		faceIndex t(FID);
		for (int i = 0;i < 3;++i)
		{
	                size_t spot2 = val[i].find_first_of("/");
	                vector<size_t> pos2;
	                int c2 = 0;
	                while (spot2!=string::npos)
	                {
	                        //cout << spot2 << endl;
	                        pos2.push_back(spot2);
	                        spot2 = val[i].find_first_of("/",spot2+1);
	                        ++c2;
	                }


			t.verts[i] = atoi(val[i].substr(0,pos2[0]).c_str()) - tNv - 1;
			t.tex[i] = atoi(val[i].substr(pos2[0] + 1,pos2[1] - pos2[0] + 1).c_str()) - tNt - 1;
			t.normal[i] = atoi(val[i].substr(pos2[1] + 1).c_str()) - tNn - 1;

			//cout << atoi(val[i].substr(0,pos2[0]).c_str()) << "-" << atoi(val[i].substr(pos2[0] + 1,pos2[1] - pos2[0] + 1).c_str()) << "-" << atoi(val[i].substr(pos2[1] + 1).c_str()) << " | ";
			//cout << t.verts[i] << "-" << t.tex[i] << "-" << t.normal[i] << endl;
		}

		//cout << t.verts[0] << " " << t.verts[1] << " " << t.verts[2] << endl;
		//cout << t.tex[0] << " " << t.tex[1] << " " << t.tex[2] << endl;
		//cout << t.normal[0] << " " << t.normal[1] << " " << t.normal[2] << endl;

		return t;
        };

	//Function Parses the vectors for verts, texcoords, normals and mtl settings. 
	//This class only works with glm::vecN where N = 2,3,4
	template<class T>
	T parseVector(T v,string line)
	{
		vector<float> val;
		size_t spot = line.find_first_of(" ");
		vector<size_t> pos;
		int c = 0;
		while (spot!=string::npos)
		{
			//cout << spot << endl;
			pos.push_back(spot);
			spot = line.find_first_of(" ",spot+1);
			++c;
		}

		for (int i = 0; i < c-1; ++i)
			{v[i] = atof(line.substr(pos[i],pos[i+1] - pos[i]).c_str());}

		v[c-1] = atof(line.substr(pos[c-1]).c_str());

		//for (int i = 0; i < c; ++i)
		//	{cout << i << " " << v[i] << endl;}
		return v;
	};

        string parseFilename(string line)
        {
                size_t spot = line.find_first_of(" ");
                //cout << spot << endl;
                string s = line.substr(spot);
                spot = s.find_first_of(" ");
		while (spot!=string::npos)
		{
			//cout << spot << endl;
			s.erase(s.begin()+spot);
                	spot = s.find_first_of(" ",spot+1);
		}
                //cout << "string: " << s << endl;
                return s;
        };

        float parseFloat(string line)
        {
                size_t spot = line.find_first_of(" ");
                //cout << spot << endl;
                string s = line.substr(spot);
                spot = s.find_first_of(" ");
                while (spot!=string::npos)
                {
                        //cout << spot << endl;
                        s.erase(s.begin()+spot);
                        spot = s.find_first_of(" ",spot+1);
                }
                //cout << "string: " << s << endl;
		float n = atof(s.c_str());
                return n;
        };

	//Main Parser Program - Parses the object program
	void parseObjData()
	{
		int chk = 1;
		for (int i = 0; i < lines; ++i)
		{
			int t0 = data[i].find("o ");
			int t1 = data[i].find("mtllib");
			int t2 = data[i].find("v ");
			int t3 = data[i].find("vt ");
			int t4 = data[i].find("vn ");
			int t5 = data[i].find("usemtl ");
			int t6 = data[i].find("f ");
			if (t0 == 0) {md.push_back(parseFilename(data[i]));++Nmesh;chk = 1;}
			if (t1 == 0) {mtlfilename = parseFilename(data[i]);}
			if (t2 == 0) {md[Nmesh - 1].verticies.push_back(parseVector(glm::vec3(0.0f),data[i]));++md[Nmesh - 1].Nv;}
			if (t3 == 0) {md[Nmesh - 1].textures.push_back(parseVector(glm::vec2(0.0f),data[i]));++md[Nmesh - 1].Nt;}
			if (t4 == 0) {md[Nmesh - 1].normals.push_back(parseVector(glm::vec3(0.0f),data[i]));++md[Nmesh - 1].Nn;}
			if (t5 == 0) {md[Nmesh - 1].mtlname = parseFilename(data[i]);++Nm;}
			if (t6 == 0) {md[Nmesh - 1].facedat.push_back(parseFaces(md[Nmesh - 1].Nf,data[i]));++md[Nmesh - 1].Nf;}

			if (chk == 1 && Nmesh > 1)
			{
				tNv += md[Nmesh - 2].Nv;
				tNn += md[Nmesh - 2].Nn;
				tNt += md[Nmesh - 2].Nt;
	 	                //for (int i = 0; i < Nmesh; ++i)
	          	        //      {cout << "TNverts: " << tNv << " TNtex: " << tNt << " TNnorms: " << tNn << endl;};

				chk = 0;
			}
		}
		data.clear();
	};

        //Main Parser Program - Parses the material program
        void parseMtlData()
        {
		int num_f = 0;
                for (int i = 0; i < lines; ++i)
                {
                        int t1 = data[i].find("newmtl ");
                        int t2 = data[i].find("Ns ");
                        int t3 = data[i].find("Ka ");
                        int t4 = data[i].find("Kd ");
                        int t5 = data[i].find("Ks ");
                        int t6 = data[i].find("Ni ");
                        int t7 = data[i].find("d ");
                        //int t8 = data[i].find("map_Kd ");
                        if (t1 == 0) {mtldat.push_back(parseFilename(data[i]));}
                        if (t2 == 0) {mtldat[num_f].Ns = parseFloat(data[i]);}
                        if (t3 == 0) {mtldat[num_f].Ka = parseVector(glm::vec3(0.0f),data[i]);}
                        if (t4 == 0) {mtldat[num_f].Kd = parseVector(glm::vec3(0.0f),data[i]);}
                        if (t5 == 0) {mtldat[num_f].Ks = parseVector(glm::vec3(0.0f),data[i]);}
                        if (t6 == 0) {mtldat[num_f].Ni = parseFloat(data[i]);}
                        if (t7 == 0) {mtldat[num_f].d = parseFloat(data[i]);++num_f;}
                        //if (t8 == 0) {mtldat[num_f].texfilename = parseFilename(data[i]);}
                }
		data.clear();
        };

	void printData()
	{
		//for (int i = 0; i < lines; ++i)
		//	{cout << data[i] << endl;}

		cout << "Number of Meshes loaded: " << Nmesh << endl;
		cout << "Number of Materials loaded: " << Nm << endl;
		cout << mtldat[0].texfilename << endl;
		cout << mtldat[1].texfilename << endl;
		for (int i = 0; i < Nmesh; ++i)
			{cout << "Nverts: " << md[i].Nv << " Ntex: " << md[i].Nt << " Nnorms: " << md[i].Nn << endl;};

		//for (int i = 0; i < 3; ++i)
		//	{cout << mtldat[1].Kd[i] << endl;}

		
	};

        //Class constructor
        objLoader(string filename)
        {
                Nm = 0;Nmesh = 0;tNv = 0;tNt = 0;tNn = 0;
		readData(filename.c_str());
		parseObjData();	
		readData(mtlfilename.c_str());
		parseMtlData();
        };

        //Class destructor
        ~objLoader() {};

	void CalculateTangents(TBNdata &tbndat,int mID)
	{
		//For all faces
		cout << "Calculating Tangent Vectors for mID: " << mID << endl;
                for (int i = 0; i < md[mID].Nf; ++i)
                {
       			glm::vec3 & v0 = md[mID].verticies[md[mID].facedat[i].verts[0]];
       			glm::vec3 & v1 = md[mID].verticies[md[mID].facedat[i].verts[1]];
       			glm::vec3 & v2 = md[mID].verticies[md[mID].facedat[i].verts[2]];

       			glm::vec2 uv0 = md[mID].textures[md[mID].facedat[i].tex[0]];
       			glm::vec2 uv1 = md[mID].textures[md[mID].facedat[i].tex[1]];
       			glm::vec2 uv2 = md[mID].textures[md[mID].facedat[i].tex[2]];
			
			 // Edges of the triangle : postion delta
    			glm::vec3 deltaPos1 = v1-v0;
			glm::vec3 deltaPos2 = v2-v0;
 
    			// UV delta
			uv0 = glm::vec2(uv0.x,1-uv0.y);
			uv1 = glm::vec2(uv1.x,1-uv1.y);
			uv2 = glm::vec2(uv2.x,1-uv2.y);

			glm::vec2 deltaUV1 = uv1-uv0;
			glm::vec2 deltaUV2 = uv2-uv0;

			//Calculate tangents
			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
			glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

			//Store the tagents per face
			tbndat.tangent.push_back(tangent);
			tbndat.bitangent.push_back(bitangent);
		}
		
	};

	int indexer(vector<glm::ivec3> va, glm::ivec3 v,int Nidx)
	{
		int chk = -1;
		for (int i = 0; i < Nidx; ++i)
		{
			if (va[i][0] == v[0] && va[i][1] == v[1] && va[i][2] == v[2])
				{chk = i;break;};
		}
		return chk;
	};

	Mesh CreateMesh(int mID,bool TBN)
	{
		Mesh mesh(mID);
		mesh.TBN = TBN;

		TBNdata tbndat;
		if (TBN)
			{CalculateTangents(tbndat,mID);}

		vector<glm::ivec3> va;
		int Nidx = 0;
		//Set verticies and indicies
		for (int i = 0; i < md[mID].Nf; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				glm::ivec3 v;

				v.x = md[mID].facedat[i].verts[j];
				v.y = md[mID].facedat[i].tex[j];
				v.z = md[mID].facedat[i].normal[j];

				int swt;
				if (Nidx == 0)
					{swt = -1;}
				else {swt = indexer(va,v,Nidx);}

				if (swt >= 0)
				{
					mesh.indices.push_back(swt);
					if(TBN)
					{
						mesh.verticeswtang[swt].tangent += tbndat.tangent[i];
						mesh.verticeswtang[swt].bitangent += tbndat.bitangent[i];
					}
				} 
				else 
				{
					va.push_back(v);

                                        //Save vertex
					if(!TBN)
					{
                                        	Vertex vert;
                                        	vert.position = md[mID].verticies[v[0]];
                                        	vert.texture = md[mID].textures[v[1]];
                                        	vert.normal = md[mID].normals[v[2]];
                                        	mesh.vertices.push_back(vert);
					} else {
	                                        VertexwTang vert;
	                                        vert.position = md[mID].verticies[v[0]];
	                                        vert.texture = md[mID].textures[v[1]];
	                                        vert.normal = md[mID].normals[v[2]];
                                        	vert.tangent = tbndat.tangent[i];
                                        	vert.bitangent = tbndat.bitangent[i];
                                        	mesh.verticeswtang.push_back(vert);
					}

                                        //Save index of the vertex
                                        mesh.indices.push_back(Nidx);
					//cout << "INDEX: " << mesh.indices[i * 3 + j] 
						//<< " POS [" << vert.position.x << "," << vert.position.y << "," << vert.position.z << "]"  
						//<< " TEX [" << vert.texture.x << "," << vert.texture.y << "]"
						//<< " NORM [" << vert.normal.x << "," << vert.normal.y << "," << vert.normal.z << "]"
						//<< "\n";
                                        ++Nidx;

				}
				//cout << "INDEX: " << mesh.indices[i * 3 + j] << "\n";
			}
		}
		//Set materials
		for (int i = 0; i < Nm; ++i)
		{
			if (mtldat[i].mtlname.compare(md[mID].mtlname) == 0)
			{
				//cout << "Using Material: " << mtldat[i].mtlname << endl;
				mesh.materials.shine = mtldat[i].Ns;
				mesh.materials.Ka = mtldat[i].Ka;
				mesh.materials.Kd = mtldat[i].Kd;
				mesh.materials.Ks = mtldat[i].Ks;
				mesh.materials.texfilename = mtldat[i].texfilename;
			}
		};

		cout << "Total Vertices: " << 3 * md[mID].Nf << " Total Indexed: " << Nidx << "\n";
		//Return the mesh
		return mesh;
	};
};

#endif
