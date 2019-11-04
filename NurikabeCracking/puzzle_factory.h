#ifndef PUZZLE_FACTORY_H
#define PUZZLE_FACTORY_H
#include "row.h"
#include "node_map.h"

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

        void generate_partially_contiguous_table();

        void generate_columnally_contiguous_table();

        void generate_all_columns_contiguous_table();

        void generate_recontiguous_table();

        void generate_contiguous_row_list();

        void generate_decision_table();

        //void generate_upwardleading_table();

        //void generate_downwardleading_table();

        //void generate_connections_data();

        //void generate_upwardcorrective_table();
        
        //void generate_downwardcorrective_table();

        //void generate_columncorrective_table();

        // A general table that keeps track of all possible combinations, good or bad,
        // and defines them by their enumerations for fast table lookup if needed.
        //std::vector<std::vector<Result>> general_table();

        // Generator

        void pattern_generator(Control status, int func_depth, std::vector<int> seeds, bool shore, bool is_partial_warning, Choices choice, node_map map);

        void pattern_gen_with_mapping(Control status, int func_depth, node_map map);
        // Function to check for 2x2 pools of water by generating the rows from the passed seeds.
        bool has_pool(std::vector<int> row_seeds, int order);

        bool has_pool(std::vector<std::vector<char>> matrix, int order);
        // Determine if there are isolated water tiles. If a tile is
        
        // Given an int for a seed and the length of the string, generate the sequence of bits.

        int count_water(std::vector<std::vector<char>> matrix);

        int traverse(std::vector<std::vector<char>> &matrix, int point[], int water, char path_char);
        // Retrives the game order, or in otherwords, the dimensions of the square matrix.
        int get_order();
       
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

        void set_good_seeds(std::vector<int> seeds);
       
        std::string to_string_matrix(int seed);

        std::vector<int> get_good_seeds();


        //
        // POTENTIALLY DEPRECATED CODE
        //

        // Getter for retrieving the seed space after generation.
        void test_small_patterns();

        // Getter function to retrieve game's total possible pattern size.
        int get_total_pattern_count();


    private:

        std::vector<std::vector<bool>> pool_table;

        //std::vector<std::vector<bool>> noncontiguous_edge_table;

        //std::vector<std::vector<bool>> noncontiguous_table;

        //std::vector<std::vector<bool>> legal_edge_table;

        //std::vector<std::vector<bool>> legal_middle_table;

        std::vector<std::vector<bool>> partially_contiguous_table;

        std::vector<std::vector<bool>> columnally_contiguous_table;

        std::vector<std::vector<bool>> all_columns_contiguous_table;

        std::vector<std::vector<bool>> recontiguous_table;

        // Serves as both contiguous and noncontiguous row lists.
        std::vector<bool> contiguous_row_list;

        //std::vector<std::vector<bool>> upward_leading_table;

        //std::vector<std::vector<bool>> downward_leading_table;

        //std::vector<std::vector<bool>> upward_corrective_table;

        //std::vector<std::vector<bool>> downward_corrective_table;

        //std::vector<std::vector<bool>> column_corrective_table;

        std::vector<bool> segmented_columns_list;

        // Size of the matrix
        int order;
        // Total number of possible patterns given the matrix size.
        int total_patterns;
        // Total number of good patterns after solving.
        int good_patterns;
        // Total number of bad patterns after solving.
        int bad_patterns;
        // Vector for keeping track of all the good seeds generated.
        std::vector<int> good_seeds;

        std::vector<int> water_count;

        int call_count;

        // A three dimensional space of std::vectors that keeps track of what seeds can or cannot
        // be in a certain order.
        std::vector<std::vector<bool>> seed_table;

        std::vector<std::vector<Choices>> decision_table;

        std::vector<std::vector<std::vector<bool>>> middle_rows_database;
    };

}

#endif

