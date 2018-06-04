#pragma once

#include <vector>
#include "Properties.h"
#include "Production.h"


class LSystem
{
public:
    LSystem();

    virtual ~LSystem(){}

    virtual void Read(const char * path);
    double ParseFloat(std::string & s);
    char* Derive();

    Production GetProductions(std::string & s);

    Properties properties;
    std::vector<Production> productions;

    void ApplyProduction(char **curHandle, char **nextHandle, Production *prodPtr);



    Production *FindProd(char *curPtr);
};