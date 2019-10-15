#include "pch.h"

using namespace std;

namespace nurikabe {
    puzzle_factory::puzzle_factory(int passed_order)
    {
        puzzle_factory::order = passed_order;
        puzzle_factory::generate_border_table(order);
        puzzle_factory::generate_database(order);
        puzzle_factory::test_all_patterns();       
    }

    puzzle_factory::~puzzle_factory()
    {
        // No dynamically allocated memory to dispose of yet.
    }

    vector<vector<vector<bool>>> puzzle_factory::get_middle_rows_database() {
        return puzzle_factory::middle_rows_database;
    }

    vector<vector<bool>> puzzle_factory::get_table() {
        return puzzle_factory::seed_table;
    }

    void puzzle_factory::generate_border_table(int order) {
        vector<vector<bool>> table;
        int bit_length = pow(2, order);

        for (int i = 0; i < bit_length; i++) {
            vector<bool> row;
            for (int j = 0; j < bit_length; j++) {
                row.push_back(false);
            }
            table.push_back(row);
        }

        vector<vector<char>> matrix(2);
        for (int i = 0; i < bit_length; i++) {
            matrix[0] = puzzle_factory::gen_row(i, order);
            for (int j = 0; j < bit_length; j++) {
                if (table[i][j] == false) {
                    matrix[1] = puzzle_factory::gen_row(j, order);
                    vector<vector<char>> copy_matrix = matrix;
                    if (!puzzle_factory::has_pool(matrix, order)) {                       
                        int water = puzzle_factory::count_water(matrix);
                        for (int k = 0; k < order; k++) {
                            int point[2] = { 1, k };
                            if (matrix[1][k] == 0) {                              
                                water -= puzzle_factory::traverse(copy_matrix, point, 0, 1);
                            }                                                    
                            if (water == 0) {
                                table[i][j] = true;
                                break;
                            }
                        }
                    }
                }              
            }
        }
        puzzle_factory::seed_table = table;
    }

    void puzzle_factory::generate_database(int order) {
        vector<vector<vector<bool>>> database;
        vector<vector<bool>> table;
        vector<bool> row; 
        int high_num = pow(2, order);

        // Initialize a boolean database with all values set to true
        for (int i = 0; i < high_num; i++) {
            row.push_back(false);
        }

        for (int j = 0; j < high_num; j++) {
                table.push_back(row);
        }

        for(int k = 0; k < high_num; k++){
            database.push_back(table);
        }
        
        // We want to test against all possible combinations of three rows that generate
        // pools, or any combinations that generate isolated water tiles in the second row.
        // Since we're testing all combinations, we don't have to check if the n x 3 matrix
        // is fully contiguous, just the second row.
        vector<vector<char>> matrix;
        matrix.reserve(3);
        vector<char> first_row;
        vector<char> second_row;
        vector<char> third_row;
        
        // Performing row generation only when necessary dramatically reduces wasted cycles.
        try {
            for (int i = 0; i < high_num; i++) {
                // Top row generation.
                first_row = puzzle_factory::gen_row(i, order);
                for (int j = 0; j < high_num; j++) {
                    // Second row generation.
                    second_row = puzzle_factory::gen_row(j, order);
                    for (int k = 0; k < high_num; k++) {
                        // Third row generation.
                        third_row = puzzle_factory::gen_row(k, order);
                        // Perform rule checking for this set of rows.
                        matrix = { first_row, second_row, third_row };
                        if (!puzzle_factory::has_pool(matrix, order)
                            && puzzle_factory::is_contiguous(matrix)) {
                            database[i][j][k] = true;
                        }
                    }
                }
            }
        }
        catch (const exception& oor){
            cout << oor.what();
        }
        
        // The resultant database should be a complete check against faulty patterns when
        // combined with the edge rows table and column checking.
        puzzle_factory::middle_rows_database = database;
    }

    // has_pool almost certainly can be made faster. can be done later.
    // Uses bitwise OR to immediately test for any consecutive 0's that may exist quickly.
    // 0010
    // 0001 bitwise OR operation
    // ----
    // 0011 -- two consecutive zeros indicate a pool. The algorithm does not generate the
    // entire vector if two consecutive zeros are found.
    bool puzzle_factory::has_pool(vector<int> row_seeds, int order) {
        int length = row_seeds.size();
        vector<char> prev_row = gen_row(row_seeds[0], order);

        for (int i = 1; i <= row_seeds.size() - 1; i++) {
            vector<char> row = gen_row(row_seeds[i], order);
            char prev_res = 1;

            for (int j = 0; j < row.size(); j++) {
                char res = (row[j] | prev_row[j]);

                if (prev_res == 0 && res == 0) {
                    return true;
                }
                else {
                    prev_res = res;
                }
            }
            prev_row = row;
        }

        return false;
    }

    // Overloaded has_pool, takes a char matrix as a parameter instead of seeds.
    bool puzzle_factory::has_pool(vector<vector<char>> matrix, int order) {
        int length = matrix[0].size();
      
        for (int i = 0; i < matrix.size() - 1; i++) {
            char prev_res = 1;
            for (int j = 0; j < matrix[1].size(); j++) {
                char res = (matrix[i][j] | matrix[i + 1][j]);

                if (prev_res == 0 && res == 0) {
                    return true;
                }
                else {
                    prev_res = res;
                }
            }
        }
        return false;
    }

    int puzzle_factory::count_water(vector<vector<char>> matrix) {
        int water = 0;

        for (int i = 0; i < matrix.size(); i++) {
            water +=  (matrix[i].size() - accumulate(matrix[i].begin(), matrix[i].end(), 0));
        }

        return water;
    }


    int puzzle_factory::traverse(vector<vector<char>> &matrix, int point[], 
        int water, char path_char) {
        // Path character indicates what you want to replace a water tile with to show
        // you've been there before. Highly recommended to not make this '0'.
        matrix[point[0]][point[1]] = path_char;
        water += 1;
        int pos[2] = { 0, 0 };

        // Right
        if (point[1] != matrix[0].size() - 1 && matrix[point[0]][point[1] + 1] == 0) {
            pos[0] = point[0];
            pos[1] = point[1] + 1;
            water = puzzle_factory::traverse(matrix, pos, water, path_char);
        }
        // Down
        if (point[0] != matrix.size() - 1 && matrix[point[0] + 1][point[1]] == 0) {
            pos[0] = point[0] + 1;
            pos[1] = point[1];
            water = puzzle_factory::traverse(matrix, pos, water, path_char);
        }
        // Left
        if (point[1] != 0 && matrix[point[0]][point[1] - 1] == 0) {
            pos[0] = point[0];
            pos[1] = point[1] - 1;
            water = puzzle_factory::traverse(matrix, pos, water, path_char);
        }
        // Up
        if (point[0] != 0 && matrix[point[0] - 1][point[1]] == 0) {
            pos[0] = point[0] - 1;
            pos[1] = point[1];
            water = puzzle_factory::traverse(matrix, pos, water, path_char);
        }

        return water;
    }

    int puzzle_factory::get_order()
    {
        return puzzle_factory::order;
    }

    int puzzle_factory::get_total_pattern_count()
    {
        return puzzle_factory::total_patterns;
    }

    void puzzle_factory::set_good_patterns(int good_patterns)
    {
        puzzle_factory::good_patterns = good_patterns;
    }

    int puzzle_factory::get_good_patterns()
    {
        return puzzle_factory::good_patterns;
    }

    void puzzle_factory::set_bad_patterns(int patterns)
    {
        puzzle_factory::bad_patterns = patterns;
    }

    int puzzle_factory::get_bad_patterns()
    {
        return puzzle_factory::bad_patterns;
    }

    void puzzle_factory::set_good_seeds(vector<int> seeds) {
        puzzle_factory::good_seeds = seeds;
    }

    vector<int> puzzle_factory::get_good_seeds() {
        return puzzle_factory::good_seeds;
    }

    void puzzle_factory::test_all_patterns()
    {
        int max_val = pow(2, order) - 1;
        vector<int> row_seeds;
        int vec_length = order;
        vector<int> vec_pos_values;

        // Calculate the bit value at every position in the vector now so we only have to
        // do this once. Reduces wasted cycles.
        /*for (int vec_pos = 1; vec_pos < vec_length; vec_pos++) {
            vec_pos_values.push_back(pow(2, vec_pos));
        }*/
        
        // Initialize row seeds to all 0's of the correct length.
        for (int i = 0; i < vec_length; i++) {
            row_seeds.push_back(0);
        }

        int row_range = row_seeds.size() - 1;
        
        for (int i = 0; i < pow(2, pow(2, vec_length)); i++) {
            // Get the position for the bottom row of the vector, or the last element.
            // Check if the top two rows are legal, then check if the bottom 
            // two rows are legal.
            
            if (puzzle_factory::check_matrix(row_seeds) 
                && puzzle_factory::check_matrix(puzzle_factory::get_column_seeds(row_seeds))) {
                good_seeds.push_back(i);
            }
          
            row_seeds[row_range]++;

            if (row_seeds[row_range] > max_val) {
                for (int n = row_range; n > 0; n--) {
                    if (row_seeds[n] > max_val) {
                        row_seeds[n - 1]++;
                        row_seeds[n] = 0;
                    }
                }
            }

                       
            /*int seed = i;
            for (int vec_pos = vec_length - 1; vec_pos >= 0; vec_pos--) {
                if (seed >= vec_pos_values[vec_pos]) {
                    seed -= vec_pos_values[vec_pos];
                }
            }*/
        }
        puzzle_factory::set_good_patterns(puzzle_factory::good_seeds.size());
        puzzle_factory::set_bad_patterns(pow(2, order*order) - puzzle_factory::get_good_patterns());
    }

    bool puzzle_factory::check_matrix(vector<int> seeds) {
        // Get the position for the bottom row of the vector, or the last element.
        int vec_manip_pos = seeds.size() - 1;
        // Check if the top two rows are legal, then check if the bottom 
        // two rows are legal.
        if (puzzle_factory::seed_table[seeds[0]][seeds[1]] &&
            puzzle_factory::seed_table[seeds[vec_manip_pos]][seeds[vec_manip_pos - 1]]) {
            // Check to make sure there are no middle isolated water tiles.
            for (int j = 0; j < puzzle_factory::order - 2; j++) {
                if (puzzle_factory::middle_rows_database[j][j + 1][j + 2]) {
                    // Passes the top and bottom row checks but fails
                    // the middle rows check.
                    return false;
                }
            }
        }
        // Fails the top or bottom row checks.
        else {
            return false;
        }
        // Passes all tests.
        return true;
    }

    vector<int> puzzle_factory::get_column_seeds(vector<int> seeds) {
        vector<int> column_seeds;
        vector<vector<char>> rows;
        vector<char> column;

        for (int i = 0; i < seeds.size(); i++) {
            rows.push_back(puzzle_factory::gen_row(seeds[i], puzzle_factory::order));
        }

        // Iterate from 0,0 -- Moving to the right, column by column.
        // Emplace the newly minted seeds at the back of the list to ensure a proper ordering.
        for (int j = 0; j < rows[0].size(); j++) {
            column.clear();
            for (int i = 0; i < rows.size(); i++) {
                column.push_back(rows[i][j]);
            }
            column_seeds.push_back(puzzle_factory::gen_seed(column));
        }
        return column_seeds;
    }


    // This function is for contiguousness checking when there are three rows.
    // We are trying to determine if a water tile sitting in the second row can escape.
    // That is all that is required to i
    bool puzzle_factory::is_contiguous(vector<vector<char>> matrix) {
        vector<vector<char>> temp = matrix;
        int water = puzzle_factory::count_water(temp);
        int second_row_water = accumulate(temp[1].begin(), temp[1].end(), 0);
        int traversed_water;

        // Check if the second row is a straight wall of water.
        if (temp[1].size() - second_row_water == temp[1].size()) {
            return true;
        }
        
        // Check if all water in the temp is in the second row
        if (water == second_row_water) {
            // If they are equivalent determine if all water can be reached from one side.
            // 0 0 0 1 0 is not contiguous, but 0 0 0 1 1 will be for our purposes.
            for (int i = 0; i < temp[1].size(); i++) {
                if (temp[1][i] == 0) {
                    int pos[2] = { 1, i };
                    traversed_water = puzzle_factory::traverse(temp, pos, 0, 1);
                    // If all water can be traversed, it's contiguous.
                    if (traversed_water == water) {
                        return true;
                    }
                }
            }
        }
        // If there exists water, but none of it is in the second row, the pattern
        // is not contiguous.
        else if (second_row_water == 0 && water > 0){
            return false;
        }
        // For all other cases, if not all of the water is in the second row,
        // we must determine if the water that is in the second row leads out from the
        // top or the bottom. Sides do not matter.
        else {
            // Examine the top row for water leading in.
            for (int i = 0; i < temp[0].size(); i++) {
                // If water is found, traverse it.
                if (temp[0][i] == 0) {
                    int point[2] = { 0, i };
                    puzzle_factory::traverse(temp, point, 0, 1);
                }
            }

            for (int i = 0; i < temp[2].size(); i++) {
                // If water is found, traverse it.
                if (temp[2][i] == 0) {
                    int point[2] = { 2, i };
                    puzzle_factory::traverse(temp, point, 0, 1);
                }
            }

            // If there's leftover water, it's not contiguous.
            if (accumulate(temp[1].begin(), temp[1].end(), 0) != temp[1].size()) {
                return false;
            }
        }

        // If no cases trip falsehood, it must be true.
        return true;
    }

    vector<char> puzzle_factory::gen_row(int seed, int desired_length) {
        vector<char> bit_vector;

        // If the seed is -1, this represents a vector beyond the scope of the matrix order, 
        // and will generate x's of the specified vector length. This is for bounds checking
        // during the seed space creation.
        for (int i = 0; i < desired_length; i++) {
            bit_vector.push_back((seed >> i) & 1);
        }

        return bit_vector;
    }

    int puzzle_factory::gen_seed(vector<char> bits) {
        int value = 0;
        int seed = 0;

        for (int i = 0; i < bits.size(); i++) {
            if (bits[i] == 1) {
                value = pow(2, bits.size() - i - 1);
            }
            else {
                value = 0;
            }           
            seed += value;
        }
        return seed;
    }
    
}