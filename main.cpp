#include <iostream>

#include "LSystem.h"



int main(int argc,char ** argv)
{
	try
	{
		if (argc < 2)
		{
			throw std::runtime_error("no file input.");
		}
		LSystem ls;

		ls.Read(argv[1]);
		char * derived = ls.Derive(); 

		 
	}
	catch (std::exception & ex)
	{
		std::cout << ex.what();
	}

    return 0;
}