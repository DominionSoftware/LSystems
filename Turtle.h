#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

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
	double xmin;
	double xmax;
	double ymin;
	double ymax;
};


const double MAXANGLE = 40;
const double MAXSCALE = 100;
const double TWO_PI = M_PI * 2;

class Turtle
{
public:
	Turtle();

    virtual ~Turtle(){}

	double x = 0;
	double y = 0;
	int direction;
};