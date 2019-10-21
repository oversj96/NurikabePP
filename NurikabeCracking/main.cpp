// NurikabeCracking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

using std::cin;
using std::cout;
using std::endl;
using std::to_string;
using std::string;

int main()
{
    cout << "\nCounting patterns...\n" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    nurikabe::puzzle_factory factory(3);
    auto stop = std::chrono::high_resolution_clock::now();
    cout << "Counting complete. \n"
        << "Good Patterns: " << to_string(factory.get_good_patterns()) << "\n"
        << "Bad Patterns: " << to_string(factory.get_bad_patterns()) << "\n"
        << endl;
    vector<int> seeds = factory.get_good_seeds();
    
    cin.get();
    
    // It's very important to properly convert seeds to strings unless you want
    // a wall of unicode characters. 
    if (seeds.size() > 0) {

        //factory.print_all_matrices(3);
        /*cout << " --  WARNING: Listing may take awhile! --" << endl;
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
        cout << "Press any key to exit..." << endl;*/
        cin.get();
    }
    else {
        cout << "No seeds to list." << endl;
    }
    

    return 0;
}