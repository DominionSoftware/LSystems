#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
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
	Turtle() : x(0),y(0),direction(0)
	{
		
	}

    virtual ~Turtle(){}

	void Print(std::ostream & os,MoveOrDraw md)
    {
		os << md << "," << x << "," << y << "," << direction << std::endl;
    }

	double x = 0;
	double y = 0;
	int direction;
};