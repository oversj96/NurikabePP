// NurikabeCracking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

using namespace std;

int main()
{
    cout << "\nCounting patterns...\n" << endl;
    nurikabe::puzzle_factory factory(3);
    cout << "Counting complete. \n"
        << "Good Patterns: " << to_string(factory.get_good_patterns()) << "\n"
        << "Bad Patterns: " << to_string(factory.get_bad_patterns()) << "\n"
        << endl;
    vector<int> seeds = factory.get_good_seeds();
    
    cin.get();
    
    // It's very important to properly convert seeds to strings unless you want
    // a wall of unicode characters. 
    if (seeds.size() > 0) {
        cout << " --  WARNING: Listing may take awhile! --" << endl;
        cout << " -- Press any button to list the seeds --" << endl;
        string seed_start = to_string(seeds[0]);
        string seed_end;
        for (int i = 0; i < seeds.size() - 1; i++) {

            if (seeds[i] != (seeds[i + 1] - 1) || (i + 1) == (seeds.size() - 1)) {
                seed_end = to_string(seeds[i]);
                cout << seed_start << " -> " << seed_end << endl;
                seed_start = to_string(seeds[i + 1]);
                seed_end = to_string(seeds[i + 1]);
            }
            else {
                seed_end = to_string(seeds[i]);
            }

        }
        cout << "Press any key to exit..." << endl;
        cin.get();
    }
    else {
        cout << "No seeds to list." << endl;
    }
    

    return 0;
}