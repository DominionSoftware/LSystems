#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
 
#include <iostream>     
#include <fstream> 
#include <vector>
 
 

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

	 

	std::vector<double> sin;
	std::vector<double> cos;
	std::vector<double> angle;

 
};