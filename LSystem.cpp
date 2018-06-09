#include "LSystem.h"
#include "Production.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <algorithm>
#include <regex>
#include <stack>
#include <iostream>     
#include <fstream> 

#pragma optimize("",off)

LSystem::LSystem()
{

}

double LSystem::ParseNumber(std::string & s)
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
    printf("%s\n",*nextHandle);
    strcpy(*nextHandle, prodPtr->getSuccessor());
	printf("%s\n", *nextHandle);
    *curHandle += prodPtr->predecessorLength();
    *nextHandle += prodPtr->successorLength();
}
else
    {
  //  printf("%s\n",*nextHandle);
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

const int LEFT = 2;
const int TOP = 42;
const int RIGHT = 510;
const int BOTTOM = 340;


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
	char *curPtr;
	char *nextPtr;
	char *tempPtr;
	char *limPtr;
	limPtr = s2 + MAXSTR - MAXAXIOM;
    for (auto i = 1; i <= n; i++)
    {
        curPtr = s1;
        nextPtr = s2;
      
        while (*curPtr != 0) {
			printf("%s\n", curPtr);
            auto p = FindProd(curPtr);
            ApplyProduction(&curPtr, &nextPtr, p);
            if(nextPtr > limPtr) {
                printf("String too long");
                exit(1);
            }
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
            std::regex re("[0-9]<[0-9]>[0-9]-->");
			std::smatch match;
            foundmatch = std::regex_search(line, match,re);
			 
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
            properties.length = ParseNumber(temp);
            continue;
        }

        found = text.find("ANGLEFACTOR:");
        if (found!=std::string::npos)
        {
            std::string temp = text.substr(found + strlen("ANGLEFACTOR:"));
            properties.angleFactor = static_cast<int>(ParseNumber(temp));
            continue;
        }

        found = text.find("SCALEFACTOR:");
        if (found!=std::string::npos)
        {
            std::string temp = text.substr(found + strlen("SCALEFACTOR:"));
            properties.scale = ParseNumber(temp);
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


void LSystem::SetDrawParam(Box & boundBox, int * incPtr, Pixel * startPtr)
{
	
	double xscale, yscale, sc;
	xscale = (RIGHT - LEFT) / (boundBox.xmax - boundBox.xmin); 
	yscale = (BOTTOM - TOP) / (boundBox.ymax - boundBox.ymin);
	 
	if (xscale>yscale)
		sc = yscale; 
	else 
		sc = xscale;
	*incPtr = static_cast<int>(floor(static_cast<double>((sc * 100) / MAXSCALE)));
	startPtr->h = static_cast<int>(RIGHT - LEFT - *incPtr * (boundBox.xmin + boundBox.xmax - 1.0)) / 2; 
	startPtr->v = static_cast<int>(BOTTOM - TOP - *incPtr * (boundBox.ymin + boundBox.ymax - 1.0)) / 2;

}


void LSystem::BoxUpdate(Turtle & turtle, Box & box) const
{
	if (turtle.x < box.xmin)
		box.xmin = turtle.x;
	if (turtle.x > box.xmax)
		box.xmax = turtle.x;
	if (turtle.y < box.ymin)
		box.ymin = turtle.y;
	if (turtle.y > box.ymax)
		box.ymax = turtle.y;
}

void LSystem::Draw(char * instructions, Properties & properties,  Box & box,int * inc,int flag)
{


	std::filebuf fb;


	fb.open(R"(C:\Users\rickf\Documents\MATLAB\output.txt)", std::ios::out);
	std::ostream os(&fb);
	


	Turtle  turtle;
	std::stack<Turtle> stack;

	double ang = -TWO_PI /4;
 
	std::vector<double> sin;
	std::vector<double> cos;


	for (int i = 0; i < properties.angleFactor; i++)
	{
		sin.push_back(*inc * std::sin(ang));
		cos.push_back(*inc * std::cos(ang));
		ang += TWO_PI / properties.angleFactor;

	}
	auto halfangFac = properties.angleFactor / 2;

	turtle.direction = 0;

	char * str = instructions;
	char c;
	while ((c = *str++) != 0) {
		
		switch (c)
		{
		case '+':
		{
			turtle.direction = 1;
		}
		break;

		case '-':
		{
			turtle.direction = -1;
		}
		break;
		case'|':
		{
		}
			break;
		case '[':
		{
			stack.push(turtle);
		}
		case ']':
		{
			if (!stack.empty())
			{
				turtle = stack.top();
				stack.pop();
				turtle.Print(os,Move);
			}
			else
			{
			}
		}
		break;
		case 'F':
		{
			 

			turtle.x += turtle.direction * std::cos(45 * M_PI / 180.0);
			turtle.y += turtle.direction * std::sin( 45 * M_PI / 180.0);
		
			turtle.Print(os, MoveOrDraw::Draw);
			
			 
		}
		break;


		case 'f':
		{
			turtle.x += turtle.direction * std::cos(45 * M_PI / 180.0);
			turtle.y += turtle.direction * std::sin(45 * M_PI / 180.0);
			turtle.Print(os, MoveOrDraw::Move);
			
		}
		break;
		default:
			break;
		}
		
	}
	fb.close();
} 