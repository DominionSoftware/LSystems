#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
 
#include <iostream>     
#include <fstream> 
#include <vector>
#include <vtkQuaternion.h>
 

extern const double MAXANGLE;
extern const double MAXSCALE;
extern const double TWO_PI;

 
class TransformInfo
{
public:
	TransformInfo() 
	{
		
	}

    virtual ~TransformInfo(){}

	 

	
	std::vector<double> angle;
	 
 
	template<typename T, int R = 4, int C = 4>
	void MakeRotation(const char plane, T angleRadians, vnl_matrix_fixed<T, R, C> & M)
	{
		assert(R >= 3 && C >= 3);
		M.set_identity();
		T theta = angleRadians;
		float st = sin(theta);
		float ct = cos(theta);

		switch (plane)
		{
			case 'x':
			{
				M(0, 0) = 1;	M(0, 1) = 0;	M(0, 2) = 0;
				M(1, 0) = 0;	M(1, 1) = ct;	M(1, 2) = -st;
				M(2, 0) = 0;	M(2, 1) = st;	M(2, 2) = ct;
			}
			break;

			case 'y':
			{
				M(0, 0) = ct;	M(0, 1) = 0;	M(0, 2) = st;
				M(1, 0) = 0;	M(1, 1) = 1;	M(1, 2) = 0;
				M(2, 0) = -st;	M(2, 1) = 0;	M(2, 2) = ct;
			}
			break;
			case 'z':
			{
				M(0, 0) = ct;	M(0, 1) = -st;	M(0, 2) = 0;
				M(1, 0) = st;	M(1, 1) = ct;	M(1, 2) = 0;
				M(2, 0) = 0;	M(2, 1) = 0;	M(2, 2) = 1;
			}
			break;
		}
	}
};