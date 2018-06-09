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
		std::filebuf fb;


		fb.open(R"(C:\Users\rickf\Documents\MATLAB\derived.txt)", std::ios::out);
		std::ostream os(&fb);

		std::string d(derived + 1);

		os << d << std::endl;

		fb.close();


		Box box;
		int inc = 1;
		ls.Draw(derived+1, ls.properties,box,&inc,0);
	

		Pixel start;
		start.h = 0;
		start.v = 0;

		ls.SetDrawParam(box, &inc, &start);
		ls.Draw(derived + 1, ls.properties, box, &inc, 1);
		 
	}
	catch (std::exception & ex)
	{
		std::cout << ex.what();
	}

    return 0;
}