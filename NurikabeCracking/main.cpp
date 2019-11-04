// NurikabeCracking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "shape_generator.h"

int main()
{
    shape_generator sg(2);
    std::cout << std::to_string(sg.shapes.size()) << std::endl;
    std::cin.get();

    return 0;
}