#include <iostream>
#include <experimental/filesystem>
		
#include "LSystem.h"


const char * paths[] = { "D:\\Projects\\LSystems\\input.a.txt",
"D:\\Projects\\LSystems\\input.b.txt",
"D:\\Projects\\LSystems\\input.c.txt",
"D:\\Projects\\LSystems\\input.d.txt",
"D:\\Projects\\LSystems\\input.e.txt",
"D:\\Projects\\LSystems\\input.f.txt"
};


int main(int argc,char ** argv)
{
	try
	{
		if (argc < 2)
		{
			throw std::runtime_error("no file input.");
		}
		

		for (int i = 0; i < 1; i++)
		{
			LSystem ls;
			std::experimental::filesystem::path inputPath(paths[i]);
			std::string inputFileName = inputPath.filename().string();


			ls.Read(paths[i]);
			Buffer<char> buffer(MAXSTR);

			ls.Derive(buffer);
			std::filebuf fb;


			//fb.open(R"(C:\Users\rickf\Documents\MATLAB\derived.txt)", std::ios::out);
			//std::ostream os(&fb);
//
			//std::string d(derived + 1);

			//os << d << std::endl;

			//fb.close();

			char * b1;
			char * b2;
			std::tie(b1, b2) = buffer.GetBasePointers();


			Box box;
			int inc = 1;
			ls.Draw(inputFileName + ".out.txt", b1, ls.properties, box, &inc, 0);


			Pixel start;
			start.h = 0;
			start.v = 0;

			ls.SetDrawParam(box, &inc, &start);
			ls.Draw(inputFileName + ".out.txt", b1, ls.properties, box, &inc, 1);
	

		}
	}
	catch (std::exception & ex)
	{
		std::cout << ex.what();
	}

    return 0;
}