#pragma once

#include <vector>
#include "Properties.h"
#include "Production.h"
#include "Turtle.h"


class LSystem
{
public:
    LSystem();

    virtual ~LSystem(){}

    virtual void Read(const char * path);
    double ParseNumber(std::string & s);
    char * Derive();

    Production GetProductions(std::string & s);

    Properties properties;
    std::vector<Production> productions;

    void ApplyProduction(char **curHandle, char **nextHandle, Production *prodPtr);
    Production *FindProd(char *curPtr);

	void BoxUpdate(Turtle & turtle, Box & box) const;
	void SetDrawParam(Box & boundBox, int * incPtr, Pixel * startPtr);
	void Draw(std::string & outputFileName,char * instructions, Properties & properties, Box & box, int *  inc,int flag = 1);
};