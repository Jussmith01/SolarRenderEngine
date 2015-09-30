#pragma once
#ifndef jsmmath_class
#define jsmmath_class


using namespace std;

#include "../mainHeaders.h"

namespace jsm
{
	template<class T>
	struct quat
	{
		//Class Variables
	        T w; T x; T y; T z;

		//Default Constructor
	        quat()
	        {
	                this->w = (T)0;
	                this->x = (T)0;
	                this->y = (T)1;
	        	this->z = (T)0;
		}

		//Defined Constructor
                quat(T w, T x, T y, T z)
                {
                        this->w = (T)w;
                        this->x = (T)x;
                        this->y = (T)y;
                        this->z = (T)z;
                }

		//Class Destructor
		~quat() {};

		//Class Assignment
                quat& operator=(const quat& instance)
                {
                        this-> w = instance.w;
                        this-> x = instance.x;
                        this-> y = instance.y;
                        this-> z = instance.z;
                        return *this;
                };

		//Class Intrinsic Sum
		quat& operator+=(quat& instance)
		{
			this->w += instance.w;
                        this->x += instance.x;
                        this->y += instance.y;
                        this->z += instance.z;
                        return *this;
		};

		//Class Extrinsic Sum
		quat& operator+(quat& instance)
                {
                        return quat(*this) += instance;
                };

		//Class Intrinsic Product
		quat& operator*=(quat& instance)
		{
			T w1 = this->w; T w2 = instance.w;
			T x1 = this->x; T x2 = instance.x;
			T y1 = this->y; T y2 = instance.y;
			T z1 = this->z; T z2 = instance.z;

			this->w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
                        this->x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
                        this->y = w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2;
                        this->z = w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2;
                        return *this;
		};

		//Class Extrinsic Product
                quat& operator*(quat& instance)
                {
                        return quat(*this) *= instance;
                };

		//Class Formatted ostream Printer
		friend std::ostream& operator<<(std::ostream& os, quat& q)
    		{
			stringstream qout;
			qout << "[" << q.w << "," << q.x << "i + " << q.y << "j + " << q.z << "k] ";
        		return os << qout.str();
    		};
	};

	//Function calculates the length of the quat class elements
        template<typename T>
	T quatLength(quat<T> &q)
	{
		T magnitude;
		magnitude = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
		return magnitude;
	};

	//Function normalizes the given quaternion
        template<typename T>
        quat<T> quatNormalize(quat<T> &q)
        {
		quat<T> qr;
                T magnitude = quatLength(q);
                qr.w = q.w / (T)magnitude;
                qr.x = q.x / (T)magnitude;
                qr.y = q.y / (T)magnitude;
                qr.z = q.z / (T)magnitude;
		return qr;
        };

	//Function Converts the given quaternion to a 4x4 rotation transformation matrix
        template<typename T>
        glm::mat4 quatUnittoMatrix(quat<T> &qi)
        {
		quat<T> q = quatNormalize(qi);
                glm::mat4 mr;
                mr[0][0] = 1 - 2 * ( q.y * q.y + q.z * q.z );
                mr[0][1] = 2 * ( q.x * q.y - q.z * q.w );
                mr[0][2] = 2 * ( q.x * q.z + q.y * q.w );
                mr[0][3] = 0;
                mr[1][0] = 2 * ( q.x * q.y + q.z * q.w );
                mr[1][1] = 1 - 2 * ( q.x * q.x + q.z * q.z );
                mr[1][2] = 2 * ( q.y * q.z - q.x * q.w );
                mr[1][3] = 0;
                mr[2][0] = 2 * ( q.x * q.z - q.y * q.w );
                mr[2][1] = 2 * ( q.y * q.z + q.x * q.w );
                mr[2][2] = 1 - 2 * ( q.x * q.x + q.y * q.y );
                mr[2][3] = 0; mr[3][0] = 0; mr[3][1] = 0;
                mr[3][2] = 0; mr[3][3] = 1;
                return mr;
        };

	//Function generates the local quaternion of a given glm::vec3 and float (radians)
	template<typename T>
	quat<T> quatGenLocal( T a,glm::vec3 v )
	{	
		quat<T> ql;
		ql.w = cos(a/(T)2);
		ql.x = v.x * sin(a/(T)2);
		ql.y = v.y * sin(a/(T)2);
		ql.z = v.z * sin(a/(T)2);
		return quatNormalize(ql);
	};

        //Function rotates a given glm::vec3 using a quaternion
        template<typename T>
        glm::vec3 quatRotVec3( quat<T> qi,glm::vec3 v3 )
        {
		quat<T> q = quatNormalize(qi);
                glm::vec4 v4 = glm::vec4(v3,1.0f);
		glm::mat4 m4 = quatUnittoMatrix(q);
		glm::vec3 rv = glm::vec3(m4 * v4);
                return rv;
        };

        /*inline glm::vec3 GetOrthogonal( glm::vec3 Norm )
        {
		glm::vec3 Nv = glm::normalize(Norm);
		glm::vec3 Ov;
		//Case 1: (1,0,0) -> (0,1,0)
		if (abs(Nv.x) >= 0.999) {Ov = glm::vec3(0,1,0);}
		//Case 2: (0,1,0) -> (1,0,0)
		else if (abs(Nv.y) >= 0.999) {Ov = glm::vec3(0,0,1);}
		//Case 3: (0,0,1) -> (0,1,0)
		else if (abs(Nv.z) >= 0.999) {Ov = glm::vec3(1,0,0);}

		//Case 4: (x,y,0) -> (y,-x,0)
		else if (abs(Nv.x) <= 0.001 && abs(Nv.y) > 0.001 && abs(Nv.z) > 0.001) {Ov = glm::vec3(0,-Nv.z,Nv.y);}
		//Case 5: (x,y,0) -> (y,-x,0)
		else if (abs(Nv.x) > 0.001 && abs(Nv.y) <= 0.001 && abs(Nv.z) > 0.001) {Ov = glm::vec3(-Nv.z,0,Nv.x);}
		//Case 6: (x,y,0) -> (y,-x,0)
		else if (abs(Nv.x) > 0.001 && abs(Nv.y) > 0.001 && abs(Nv.z) <= 0.001) {Ov = glm::vec3(-Nv.y,Nv.x,0);}

		//Case 6: (x,y,z) -> (y,-x,0)		
		else {Ov = glm::vec3(-Nv.y,Nv.x,0);}

		Ov = glm::normalize(Ov);

		return Ov;
	};*/

        inline glm::vec3 GetOrthogonal( glm::vec3 Norm )
        {
                glm::vec3 Nv = glm::normalize(Norm);
                glm::vec3 Ov;

		if (Nv.x < 1.0E-5)
			{Ov = glm::vec3(1,0,0);}
		else
		{
			float sum = Nv.y + Nv.z;

			Ov = glm::vec3(-sum/Nv.x,1.0f,1.0f);
		}

                Ov = glm::normalize(Ov);

		float dot = glm::dot(Ov,Nv);
		cout << "DOT TEST: " << dot << endl;

                return Ov;
        };
};

#endif
