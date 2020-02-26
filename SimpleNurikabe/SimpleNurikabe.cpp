// SimpleNurikabe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "RowBrain.h"


int main()
{
    // Implementation of simple rows allows for the correct determining of a 2x2 only
    // Implemenation of ambiguous rows will allow for the rest (Not Yet Implemented)
    /*RowBrain brain(3);

    int count = 0;
    for (int i = 0; i < brain.triviallyLegalRows.size(); ++i) {
        for (int j = 0; j < brain.triviallyLegalRows[i].size(); ++j) {
            if (brain.triviallyLegalRows[i][j]) {
                ++count;
            }
        }
    }
    std::cout << std::to_string(count) << std::endl;
    std::cin.get();
*/

    RowBrain brain(20);
    brain.countPartitions();
    
    return 0;
}

