// NurikabeCracking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

using namespace std;

int main()
{
    cout << "Counting patterns...\n" << endl;
    nurikabe::puzzle_factory factory(3);
    cout << "Counting complete. \n"
         << "Good Patterns: " << to_string(factory.get_good_patterns()) << "\n"
         << "Bad Patterns: "  << to_string(factory.get_bad_patterns()) << "\n"
         << endl;
    vector<int> seeds = factory.get_good_seeds();
    cout << "Listing seeds..." << endl;
    for (int i = 0; i < seeds.size(); i++) {
        cout << to_string(seeds[i]) << endl;
    }
    string input;
    cout << "Press any key to exit..." << endl;
    cin.get();

    return 0;
}