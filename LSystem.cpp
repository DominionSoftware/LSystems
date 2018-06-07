#include "LSystem.h"
#include "Production.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <algorithm>
#include <regex>

LSystem::LSystem()
{

}

double LSystem::ParseFloat(std::string & s)
{
    std::regex base_regex{"[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)"};
    std::smatch results;
    if (std::regex_match(s, results, base_regex))
    {
        std::vector<std::string> sv;
        auto itr = results.begin();

        auto s =  itr->str();
        return std::stod(s);

    }

    return std::numeric_limits<double>::quiet_NaN();

};

Production LSystem::GetProductions(std::string & line)
{
    Production production;

    std::stringstream ss(line);
   std::string token;

    if (std::getline(ss, token, '<'))
    {

        production.setLeftContext(token.c_str());

        if (std::getline(ss, token, '>'))
        {
            production.setPredecessor(token.c_str());

            if (std::getline(ss, token, '-'))
            {
                production.setRightContext(token.c_str());


                std::getline(ss, token, '-');
                std::getline(ss, token, '>');
                if (std::getline(ss, token))
                {
                    production.setSuccessor(token.c_str());
                }
            }
        }

    }

    return production;

};




Production *LSystem::FindProd(char *curPtr)
{
    for (size_t i = 0; i < productions.size(); i++)
    {
        bool pf = productions[i].predecessorDifferent(curPtr);
        bool rc = productions[i].rightContextDifferent(this->properties.ignoreMap,
                curPtr + productions[i].predecessorLength());

        bool lc = productions[i].leftContextDifferent(this->properties.ignoreMap,curPtr-1);

        if (pf || rc || lc)
        {

        }
        else
        {
            return (&productions[i]);
        }
    }
    return (nullptr);
}


void LSystem::ApplyProduction(char **curHandle, char **nextHandle, Production *prodPtr)
{
if (prodPtr != NULL)
{
    strcpy(*nextHandle, prodPtr->getSuccessor());
    printf("%s\n",*nextHandle);
    *curHandle += prodPtr->predecessorLength();
    *nextHandle += prodPtr->successorLength();
}
else
    {
    printf("%s\n",*nextHandle);
    **nextHandle = **curHandle;
    printf("%s\n",*nextHandle);
    ++(*nextHandle);
    ++(*curHandle);
    printf("%s\n",*nextHandle);
    printf("%s\n",*curHandle);
    }
}
#define MAXSTR 30000
#define MAXAXIOM 100;

char * LSystem::Derive()
{
    int n = (int)this->properties.length;
    std::string curr = this->properties.axiom;
	char * string1 = new char[MAXSTR];
	char * string2 = new char[MAXSTR];
 
	 
    
	auto s1 = string1;
	auto s2 = string2;

    for (int i=0; i < MAXSTR; i++)
        *(s1+i) = *(s2+i) = 0;

    ++s1;
    ++s2;
    strcpy(s1, properties.axiom.c_str());
    char *curPtr, *nextPtr, *tempPtr, *limPtr;
    limPtr = s2 + MAXSTR - MAXAXIOM;
    for (auto i = 1; i <= n; i++)
    {
        curPtr = s1;
        nextPtr = s2;
        //printf("%s\n",s1);
       // printf("%s\n",s2);
        while (*curPtr != 0) {
            auto p = FindProd(curPtr);
            ApplyProduction(&curPtr, &nextPtr, p);
            if(nextPtr > limPtr) {
                printf("String too long");
                exit(1);
            }
          //  printf("%s\n",s1);
          //  printf("%s\n",s2);
            *nextPtr = 0;
        }
        tempPtr = s1;
		s1 = s2;
        s2 = tempPtr;
    }
	delete[] string2;

    return(string1);
}
void LSystem::Read(const char *path)
{

    std::ifstream input(path);
    if (!input.is_open())
    {
        throw std::runtime_error("Unable to open input file.");
    }

    std::string text;


    auto isProduction = [](std::string line)
    {
        bool foundmatch = false;
        try {
            std::regex re("[0-9|*|+|-|F]+<[0-9|*|+|-|F]+>[0-9|*|+|-|F]-->[0-9|*|+|-|F]+");
            foundmatch = std::regex_search(line, re);

        } catch (std::regex_error& e) {
            std::cout << e.what() << std::endl;
        }
        return foundmatch;
    };




    while(std::getline(input,text))
    {
        text.erase( std::remove_if(text.begin(), text.end(),
                               [](char c) -> bool
                               {
                                   return std::isspace<char>(c, std::locale::classic());
                               }),
                text.end());
        std::transform(text.begin(), text.end(),text.begin(), ::toupper);

        if (isProduction(text))
        {
            std::cout << "found production" << std::endl;
            auto production = GetProductions(text);
            this->productions.push_back(production);
            continue;
        }
        std::size_t found = text.find("DERIVATIONLENGTH:");
        if (found!=std::string::npos)
        {
            std::string temp = text.substr(found + strlen("DERIVATIONLENGTH:"));
            properties.length = ParseFloat(temp);
            continue;
        }

        found = text.find("ANGLEFACTOR:");
        if (found!=std::string::npos)
        {
            std::string temp = text.substr(found + strlen("ANGLEFACTOR:"));
            properties.angle = ParseFloat(temp);
            continue;
        }

        found = text.find("SCALEFACTOR:");
        if (found!=std::string::npos)
        {
            std::string temp = text.substr(found + strlen("SCALEFACTOR:"));
            properties.scale = ParseFloat(temp);
        }
        found = text.find("AXIOM:");
        if (found!=std::string::npos)
        {
            properties.axiom = text.substr(found + strlen("AXIOM:"));
            continue;
        }
        found = text.find("IGNORE:");
        if (found!=std::string::npos)
        {
            properties.ignore = text.substr(found = strlen("IGNORE:"));
            for(auto c : properties.ignore)
            {
                properties.ignoreMap[c] = 1;
            }
            continue;
        }
    }



    input.close();


}