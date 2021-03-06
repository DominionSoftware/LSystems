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
#include "TransformInfo.h"
#include "vtkQuaternion.h"
#include "Buffer.h"

#pragma optimize("",off)

LSystem::LSystem()
{
	

	std::string output3 = R"(C:\Users\rickf\Documents\MATLAB\debug.txt)";

	debug.open(output3.c_str(), std::ios::out);
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




Production *LSystem::FindProd(Buffer<char> & buffer)
{
    for (size_t i = 0; i < productions.size(); i++)
    {
        bool pf = productions[i].predecessorDifferent(buffer.curPtr);
        bool rc = productions[i].rightContextDifferent(this->properties.ignoreMap,
			buffer.curPtr + productions[i].predecessorLength());

        bool lc = productions[i].leftContextDifferent(this->properties.ignoreMap, buffer.curPtr -1);

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
void LSystem::ApplyProduction(Buffer<char> * buffer, Production *prodPtr)
{

	std::ostream os(&debug);
	if (prodPtr != NULL)
	{
		os << "production " << prodPtr->getLeftContext() << "," << prodPtr->getPredecessor() << "," << prodPtr->getRightContext() << "," << prodPtr->getSuccessor() << std::endl;
		os << "current " << std::string(buffer->curPtr) << std::endl;
		os << "left " << std::string(buffer->curPtr-1) << std::endl;
		os << "right " << std::string(buffer->curPtr + prodPtr->predecessorLength()) << std::endl;


		os << "next " << std::string(buffer->nextPtr-1) << std::endl;
		std::memcpy(buffer->nextPtr, prodPtr->getSuccessor(), prodPtr->successorLength());
		os << "next " << std::string(buffer->nextPtr-1) << std::endl;
		os << "full1 " << std::string(buffer->buffer1+1) << std::endl;
		os << "full2 " << std::string(buffer->buffer2 + 1) << std::endl;
		buffer->curPtr += prodPtr->predecessorLength();
		buffer->nextPtr += prodPtr->successorLength();
	}
	else
	{
		//os << std::string(buffer->nextPtr) << std::endl;
		*buffer->nextPtr = *buffer->curPtr;
		//os << std::string(buffer->nextPtr) << std::endl;
		buffer->nextPtr++;
		buffer->curPtr++;
	
	}


	if (buffer->nextPtr > buffer->limPtr)
	{
		throw std::runtime_error("String too long.");
	 
	}
	*buffer->nextPtr = 0;
}


const int LEFT = 2;
const int TOP = 42;
const int RIGHT = 510;
const int BOTTOM = 340;


void LSystem::Derive(Buffer<char> & buffer)
{
	auto n = static_cast<int>(this->properties.length);
    std::string curr = this->properties.axiom;
	char * s1;
	char * s2;
	std::tie(s1, s2) = buffer.GetBasePointers();
    std::memcpy(s1, properties.axiom.c_str(), properties.axiom.length());
	 
	
    for (auto i = 1; i <= n; i++)
    {
		{
			std::ostream os(&debug);
			os << "top " << std::endl;
			os << "full1 " << std::string(buffer.buffer1+1) << std::endl;
			os << "full2 " << std::string(buffer.buffer2 + 1) << std::endl;
		}

		buffer.curPtr = s1;
		buffer.nextPtr = s2;
      
        while (*buffer.curPtr != 0) {
            auto p = FindProd(buffer);
			ApplyProduction(&buffer, p);
			
        }
		

		char * tempPtr = s1;
		s1 = s2;
        s2 = tempPtr;
    }

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
            std::regex re("(\\*|[0-9])\\s*<\\s*(\\v|\\^|\\-|\\+|[0-9])\\s*>\\s*(\\*|[0-9])\\s*-->\\s*(\\[|\\]|F|\\-|\\+|\\^|\\v|[0-9])\\s*");
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
	if (turtle[0] < box.xmin)
		box.xmin = turtle[0];
	if (turtle[0]> box.xmax)
		box.xmax = turtle[0];
	if (turtle[1] < box.ymin)
		box.ymin = turtle[1];
	if (turtle[1] > box.ymax)
		box.ymax = turtle[1];
}

void LSystem::Draw(std::string & outputFileName,char * instructions, Properties & properties,  Box & box,int * inc,int flag)
{
	
	std::filebuf fb3;

	std::string output3 = R"(C:\Users\rickf\Documents\MATLAB\angles.txt)";

	fb3.open(output3.c_str(), std::ios::out);
	std::ostream os3(&fb3);

	
	
	std::filebuf fb2;

	std::string output2 = R"(C:\Users\rickf\Documents\MATLAB\math.txt)";

	fb2.open(output2.c_str(), std::ios::out);
	std::ostream os2(&fb2);

	std::filebuf fb;

	std::string output = R"(C:\Users\rickf\Documents\MATLAB\)" + outputFileName;

	fb.open(output.c_str(), std::ios::out);
	std::ostream os(&fb);
	


	Turtle  turtle;
	std::stack<Turtle> stack;

	double ang = -TWO_PI /4;
	TransformInfo tInfo;

	tInfo.angle.push_back(ang);


	for (int i = 0; i < properties.angleFactor; i++)
	{
		ang += TWO_PI / properties.angleFactor;
		tInfo.angle.push_back(ang);
	}

	turtle.xyplane = 4;
	turtle.zplane = 4;
	auto halfangFac = properties.angleFactor / 2;

 

	char * str = instructions;
	char c;
	while ((c = *str++) != 0) {
		
		switch (c)
		{
		case '+':
		{
			if (turtle.xyplane < properties.angleFactor - 2)
			{
				turtle.xyplane++;
			}
			else
			{
				turtle.xyplane = 4;
			}
		}
		break;

		case '-':
		{
			if (turtle.xyplane > 0)
			{
				turtle.xyplane--;
			}
			else
			{
				turtle.xyplane = properties.angleFactor - 1;
			}
		}
		break;
		case '^':
		{
			if (turtle.zplane < properties.angleFactor - 2)
			{
				turtle.zplane++;
			}
			else
			{
				turtle.zplane = 4;
			}
		}
		break;

		case 'v':
		{
			if (turtle.zplane > 0)
			{
				turtle.zplane--;
			}
			else
			{
				turtle.zplane = properties.angleFactor - 1;
			}
		}
		break;

		case'|':
		{
			if (turtle.xyplane >= halfangFac) 
				turtle.xyplane -= halfangFac; 
			else 
				turtle.xyplane += halfangFac;
		}
			break;
		case '[':
		{
			stack.push(turtle);
		}
		break;
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
			auto angleXY = tInfo.angle[turtle.xyplane];
			vtkQuaternion<double> qXY;
			qXY.SetRotationAngleAndAxis(angleXY, 0, 0, 1);
			auto qXYConj = qXY.Conjugated();

			auto angleZ= tInfo.angle[turtle.zplane];
			vtkQuaternion<double> qZ;
			qZ.SetRotationAngleAndAxis(angleZ, 0, 1, 0);
			auto qzConj = qZ.Conjugated();
			 
			vtkQuaternion<double> vec(0, 0, 1,0);
			 
			auto tVec = qZ * qXY * vec  *  qXYConj * qzConj;

			turtle[0] += tVec.GetX();
			turtle[1] += tVec.GetY();
			turtle[2] += tVec.GetZ();
		
			turtle.Print(os, MoveOrDraw::Draw);
		}
		break;


		case 'f':
		{
			auto angleXY = tInfo.angle[turtle.xyplane];
			vtkQuaternion<double> qXY;
			qXY.SetRotationAngleAndAxis(angleXY, 0, 0, 1);
			auto qXYConj = qXY.Conjugated();

			auto angleZ = tInfo.angle[turtle.zplane];
			vtkQuaternion<double> qZ;
			qZ.SetRotationAngleAndAxis(angleZ, 0, 1, 0);
			auto qzConj = qZ.Conjugated();

			vtkQuaternion<double> vec(0, 0, 1, 0);

			auto tVec = qZ * qXY * vec  *  qXYConj * qzConj;


			turtle.Print(os, MoveOrDraw::Move);
			
		}
		break;
		default:
			break;
		}
		
	}
	fb.close();
	fb2.close();

	for (size_t s = 0; s < tInfo.angle.size(); s++)
	{
		os3 << 1 << "," << tInfo.angle[s] << "," << 180 * tInfo.angle[s] / M_PI << ","  << "," << properties.angleFactor << std::endl;

	}
	fb3.close();
} 