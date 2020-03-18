#include "pch.h"
#include "Generator.h"


int main()
{
    using clock_type = std::chrono::high_resolution_clock;
    
    while (true)
    {
        std::cout << "\nPlease enter a row length: ";
        int dimLength = 0;
        std::cin >> dimLength;
        std::cout << "\nPlease enter a row count: ";
        int dimRows = 0;
        std::cin >> dimRows;
        std::cout << "\nPlease wait..\n";
        auto start = clock_type::now();
        Generator gen(dimLength, dimRows, true);
        auto stop = clock_type::now();
        std::stringstream ss;
        ss << gen.goodPatterns;
        std::string num = ss.str(); 
        gen.insert_commas(num);
        std::cout << std::setprecision(2) << std::fixed << "\rTotal Good Patterns: " << num << "\n";
        auto total = stop - start;
        std::cout << "\nTime elapsed (seconds): " 
            << std::chrono::duration_cast<std::chrono::seconds>(total).count() << "\n";
        std::cout << "\nTime elapsed (milliseconds): " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(total).count() << std::endl;
        std::cin.get();
    }
}



