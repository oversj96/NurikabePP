#ifndef PUZZLE_FACTORY_H
#define PUZZLE_FACTORY_H
#include "pch.h"

using namespace std;

namespace nurikabe {

    class puzzle_factory {

    public:
        puzzle_factory(int passed_order);

        ~puzzle_factory();
        // Getter for retrieving the seed space after generation.
        vector<vector<vector<bool>>> get_middle_rows_database();

        vector<vector<bool>> get_table();

        void generate_border_table(int order);

        // Generate the seed space (three dimensional table) given a matrix order with nurakabe
        // rules
        void generate_database(int order);
        // Function to check for 2x2 pools of water by generating the rows from the passed seeds.
        bool has_pool(vector<int> row_seeds, int order);

        bool has_pool(vector<vector<char>> matrix, int order);
        // Determine if there are isolated water tiles. If a tile is
        bool is_contiguous(vector<vector<char>> matrix);

        // Given an int for a seed and the length of the string, generate the sequence of bits.
        vector<char> gen_row(int seed, int order);

        // Given a char vector of bits, convert it into the int it represents.
        int gen_seed(vector<char> bits);

        int count_water(vector<vector<char>> matrix);

        int traverse(vector<vector<char>> &matrix, int point[], int water, char path_char);
        // Retrives the game order, or in otherwords, the dimensions of the square matrix.
        int get_order();
        // Getter function to retrieve game's total possible pattern size.
        int get_total_pattern_count();
        // Sets the good pattern count.
        void set_good_patterns(int patterns);
        // Gets the good pattern count.
        int get_good_patterns();
        // Sets the bad pattern count.
        void set_bad_patterns(int patterns);
        // Gets the bad pattern count.
        int get_bad_patterns();
        // Test all possible patterns and set the pattern counts.
        void test_all_patterns();

        void set_good_seeds(vector<int> seeds);

        bool check_matrix(vector<int> seeds);

        vector<int> get_column_seeds(vector<int> seeds);

        vector<int> get_good_seeds();

    private:
        // A three dimensional space of vectors that keeps track of what seeds can or cannot
        // be in a certain order.
        vector<vector<vector<bool>>> middle_rows_database;

        vector<vector<bool>> seed_table;

        // Size of the matrix
        int order;
        // Total number of possible patterns given the matrix size.
        int total_patterns;
        // Total number of good patterns after solving.
        int good_patterns;
        // Total number of bad patterns after solving.
        int bad_patterns;
        // Vector for keeping track of all the good seeds generated.
        vector<int> good_seeds;
    };

}

#endif

