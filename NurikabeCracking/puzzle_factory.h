#ifndef PUZZLE_FACTORY_H
#define PUZZLE_FACTORY_H
#include "pch.h"

using std::vector;
using std::string;

namespace nurikabe {

    class puzzle_factory {

    public:
        puzzle_factory(int passed_order);

        ~puzzle_factory();

        enum class Control { Start, Run, Pause, Stop };

        enum class PuzzleState {Contiguous, Noncontiguous};
      
        enum class Choices { Partial, Column, AllColumn, Recontiguous, Illegal, End };

        void generate_pool_table();

        void generate_water_counts();

        void generate_noncon_column_counts();

        void generate_partially_contiguous_table();

        void generate_columnally_contiguous_table();

        void generate_all_columns_contiguous_table();

        void generate_recontiguous_table();

        void generate_contiguous_row_list();

        void generate_decision_table();

        //void generate_upwardleading_table();

        //void generate_downwardleading_table();

        void generate_connections_data();

        //void generate_upwardcorrective_table();
        
        //void generate_downwardcorrective_table();

        //void generate_columncorrective_table();

        // A general table that keeps track of all possible combinations, good or bad,
        // and defines them by their enumerations for fast table lookup if needed.
        //vector<vector<Result>> general_table();

        // Generator

        void pattern_generator(Control status, int func_depth, vector<int> seeds, bool shore, bool is_partial_warning, Choices choice,
        int noncon_col_count);

        // Function to check for 2x2 pools of water by generating the rows from the passed seeds.
        bool has_pool(vector<int> row_seeds, int order);

        bool has_pool(vector<vector<char>> matrix, int order);
        // Determine if there are isolated water tiles. If a tile is
        
        // Given an int for a seed and the length of the string, generate the sequence of bits.
        vector<char> gen_row(int seed, int order);

        string gen_row(int seed);

        // Given a char vector of bits, convert it into the int it represents.
        int gen_seed(vector<char> bits);

        int count_water(vector<vector<char>> matrix);

        int traverse(vector<vector<char>> &matrix, int point[], int water, char path_char);
        // Retrives the game order, or in otherwords, the dimensions of the square matrix.
        int get_order();

        void set_builder(string flag, vector<int> seeds, int depth, int top_seed, int bottom_seed);

        bool pathable(vector<vector<char>> matrix);

        vector<vector<int>> get_contiguous_segments(vector<char> row);
       
        // Sets the good pattern count.
        void set_good_patterns(int patterns);
        // Gets the good pattern count.
        int get_good_patterns();
        // Sets the bad pattern count.
        void set_bad_patterns(int patterns);
        // Gets the bad pattern count.
        int get_bad_patterns();
        // Test all possible patterns and set the pattern counts.
        
        void print_all_matrices(int columns);

        void set_good_seeds(vector<int> seeds);
       
        string to_string_matrix(int seed);

        vector<int> get_column_seeds(vector<int> seeds);

        vector<int> get_good_seeds();


        //
        // POTENTIALLY DEPRECATED CODE
        //

        // Getter for retrieving the seed space after generation.
        vector<vector<vector<bool>>> get_middle_rows_database();

        vector<vector<bool>> get_table();

        void generate_seed_table(int order);

        void generate_database(int order);

        bool check_matrix(vector<int> seeds);

        void test_all_patterns();

        void test_small_patterns();

        // Getter function to retrieve game's total possible pattern size.
        int get_total_pattern_count();

        bool is_contiguous(vector<vector<char>> matrix);

    private:

        vector<vector<bool>> pool_table;

        //vector<vector<bool>> noncontiguous_edge_table;

        //vector<vector<bool>> noncontiguous_table;

        //vector<vector<bool>> legal_edge_table;

        //vector<vector<bool>> legal_middle_table;

        vector<vector<bool>> partially_contiguous_table;

        vector<vector<bool>> columnally_contiguous_table;

        vector<vector<bool>> all_columns_contiguous_table;

        vector<vector<bool>> recontiguous_table;

        // Serves as both contiguous and noncontiguous row lists.
        vector<bool> contiguous_row_list;

        //vector<vector<bool>> upward_leading_table;

        //vector<vector<bool>> downward_leading_table;

        //vector<vector<bool>> upward_corrective_table;

        //vector<vector<bool>> downward_corrective_table;

        //vector<vector<bool>> column_corrective_table;

        vector<bool> segmented_columns_list;

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

        vector<int> water_count;

        int call_count;

        // A three dimensional space of vectors that keeps track of what seeds can or cannot
        // be in a certain order.
        vector<vector<bool>> seed_table;

        vector<vector<State>> decision_table;

        vector<vector<vector<bool>>> middle_rows_database;
    };

}

#endif

