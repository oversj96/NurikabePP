#include "pch.h"
#include "Generator.h"

int main()
{
    while (true)
    {
        std::cout << "\nPlease enter a row length: ";
        int dimLength = 0;
        std::cin >> dimLength;
        std::cout << "\nPlease enter a row count: ";
        int dimRows = 0;
        std::cin >> dimRows;
        std::cout << "\nPlease wait.." << std::endl;
        Generator gen(dimLength, dimRows, true);
        std::cout << "\rTotal Good Patterns: " << gen.goodPatterns << std::endl;
        std::cin.get();
    }
}
