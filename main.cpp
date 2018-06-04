#include <iostream>

#include "LSystem.h"



int main(int argc,char ** argv)
{

    LSystem ls;

    ls.Read(argv[1]);
    ls.Derive();

    return 0;
}