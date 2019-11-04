// NurikabeCracking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "shape_generator.h"

int main()
{
    shape_generator sg(2);
    std::cout << std::to_string(sg.shapes.size()) << std::endl;
    for (auto &shape : sg.shapes) {
        std::cout << "Matrix ID: " << std::to_string(shape.shape_id) << std::endl;
        for (auto &row : shape.shape_map) {
            for (auto const &val : row) {
                if (val) {
                    std::cout << " 0 ";
                }
                else {
                    std::cout << " 1 ";
                }
            }
            std::cout << std::endl;
        }
    }
    std::cin.get();

    return 0;
}