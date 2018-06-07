#pragma once


#include <string>
#include <map>

class Properties
{
public:

    Properties() : length(0),angleFactor(0),scale(1.0)
    {

    }

    double length;
    int angleFactor;
    double scale;
    std::string axiom;
    std::string ignore;
    std::map<char,uint8_t > ignoreMap;
    std::string leftContext;
    std::string rightContext;
    std::string predecessor;
    std::string successor;

};