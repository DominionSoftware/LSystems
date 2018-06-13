#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <vnl/vnl_vector_fixed.h>
#include <iostream>     
#include <fstream> 
#include <vector>
 
class Pixel
{
public:
	int h;
	int v;
};

class Box
{
public:
	double xmin = 0;
	double xmax = 0;
	double ymin = 0;
	double ymax = 0;
};


const double MAXANGLE = 40;
const double MAXSCALE = 100;
const double TWO_PI = M_PI * 2;

enum MoveOrDraw
{
	Move,
	Draw
};


class Turtle
{
public:
	Turtle() :xyzw{ 0,0,0,0 }, xyplane(0),zplane(0)
	{
		
	}

    virtual ~Turtle(){}

	double& operator[] (const int index)
	{
		return xyzw[index];
	}

	void Print(std::ostream & os,MoveOrDraw md)
    {
		os << md << "," << xyzw[0] << "," << xyzw[1] << "," << xyzw[2] << "," << xyplane << "," << zplane << std::endl;
    }

	vnl_vector_fixed<double, 4> xyzw;

	int xyplane;
	int zplane;
};