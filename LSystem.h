#pragma once

#include <vector>
#include "Properties.h"
#include "Production.h"
#include "Turtle.h"
#include "Buffer.h"


class LSystem
{
public:
    LSystem();

    virtual ~LSystem()
    {
		if (debug.is_open())
		{
			debug.close();
		}

    }

    virtual void Read(const char * path);
    double ParseNumber(std::string & s);
	void Derive(Buffer<char> & buffer);

    Production GetProductions(std::string & s);

    Properties properties;
    std::vector<Production> productions;
	void ApplyProduction(Buffer<char> * buffer, Production *prodPtr);
    
    Production *FindProd(Buffer<char> & buffer);

	void BoxUpdate(Turtle & turtle, Box & box) const;
	void SetDrawParam(Box & boundBox, int * incPtr, Pixel * startPtr);
	void Draw(std::string & outputFileName,char * instructions, Properties & properties, Box & box, int *  inc,int flag = 1);


	std::filebuf debug;


};
