#include "pch.h"

using namespace std;

namespace nurikabe {

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

    puzzle_factory::puzzle_factory(int passed_order)
    {
        if (passed_order > 1) {
            puzzle_factory::order = passed_order;

            // NOT YET FULLY FUNCTIONING

            generate_pool_table();
            generate_water_counts();
            generate_contiguous_row_list();
            generate_partially_contiguous_table();
            generate_columnally_contiguous_table();
            generate_all_columns_contiguous_table();
            generate_recontiguous_table();
            /*generate_noncontiguous_table();*/
            generate_decision_table();
            vector<int> empty_vec;
            pattern_generator(Control::Start, 0, empty_vec, false, false, Choices::Recontiguous, 0);
            /* puzzle_factory::order = passed_order;
             puzzle_factory::good_patterns = 0;
             puzzle_factory::bad_patterns = 0;
             puzzle_factory::generate_seed_table(order);
             if (order > 2) {
                 puzzle_factory::generate_database(order);
                 vector<int> seeds_container;
                 puzzle_factory::set_builder("start", seeds_container, 0, 0, 0);
             }
             else if (order > 2) {

                 puzzle_factory::test_all_patterns();
             }*/
        }
        else {
            cout << "Error: Cannot generate patterns for that integer." << endl;
        }

    }

    puzzle_factory::~puzzle_factory()
    {
        // No dynamically allocated memory to dispose of yet.
    }


    void puzzle_factory::generate_water_counts() {
        vector<int> water_count;
        for (int i = 0; i < pow(2, order); i++) {
            vector<char> row = puzzle_factory::gen_row(i, order);
            int water = 0;
            for (int j = 0; j < order; j++) {
                if (row[j] == 0) {
                    water++;
                }
            }
            puzzle_factory::water_count.push_back(water);
        }
    }

    void puzzle_factory::generate_pool_table() {
        vector<vector<bool>> table;

        for (int i = 0; i < pow(2, order); i++) {
            vector<bool> row;
            for (int j = 0; j < pow(2, order); j++) {
                vector<int> seeds = { i, j };
                if (puzzle_factory::has_pool(seeds, order)) {
                    row.push_back(true);
                }
                else {
                    row.push_back(false);
                }
            }
            table.push_back(row);
        }

        puzzle_factory::pool_table = table;
    }

    void puzzle_factory::generate_contiguous_row_list() {
        vector<bool> list;

        for (int i = 0; i < pow(2, order); i++) {
            vector<char> row = puzzle_factory::gen_row(i, order);

            vector<vector<int>> segments = puzzle_factory::get_contiguous_segments(row);

            if (segments.size() < 2) {
                list.push_back(true);
            }
            else {
                list.push_back(false);
            }
        }
        puzzle_factory::contiguous_row_list = list;
    }

    void puzzle_factory::generate_partially_contiguous_table() {
        vector<vector<bool>> table;

        for (int i = 0; i < pow(2, order); i++) {

            vector<bool> row;
            vector<char> top = puzzle_factory::gen_row(i, order);
            vector<vector<int>> segments_top = puzzle_factory::get_contiguous_segments(top);
            for (int j = 0; j < pow(2, order); j++) {
                bool partially = false;
                if (!puzzle_factory::pool_table[i][j]) {
                    vector<char> bot = puzzle_factory::gen_row(j, order);

                    vector<vector<char>> matrix = { top, bot };
                    // At least potentially contiguous, in this case.

                    vector<vector<int>> segments_bot = puzzle_factory::get_contiguous_segments(bot);


                    // Determine if all segments connect to the bottom.
                    // If no, its not contiguous at all.
                    // If yes, how many bottom segments are columnally connective?
                    // partially contiguous is < N bottom segments connective columnally.
                    // If all top segments connect to the only bottom segment, that is recontiguous.
                    // If all the top segments connect to some bottom segment, but not all bottom
                    // segments are columnally connective, that's partially contiguous.
                    // NOTE: Segments are by definition not contiguous, or else there 
                    //       is only one segment.
                    int top_segments = segments_top.size();
                    int bot_segments = segments_bot.size();
                    vector<int> col_cont_pos;

                    for (int k = 0; k < top.size(); k++) {
                        if (top[k] == 0 && bot[k] == 0) {
                            col_cont_pos.push_back(k);
                        }
                    }

                    // Having two for loops within another two for loops is disgusting, I know.
                    // I may try and speed this up later. For now, this is the simplest
                    // solution I could think of. N size shouldn't be very large for the first
                    // two dozen or so matrix orders.

                    if (col_cont_pos.size() > 0) {
                        // Loop through all top segments.
                        for (int k = 0; k < segments_top.size(); k++) {
                            // Loop through all column positions of each segment.
                            for (int m = 0; m < segments_top[k].size(); m++) {
                                // If verified column connections (col_cont_pos vector) 
                                // match column positions of a segment, the number of non-connected
                                // top segments is decremented.
                                // The point of this is to first verify these rows aren't
                                // non-contiguous in some manner. All top segments must have a path.
                                for (int col = 0; col < col_cont_pos.size(); col++) {
                                    if (segments_top[k][m] == col_cont_pos[col]) {
                                        top_segments--;
                                    }
                                }
                            }
                        }
                        if (top_segments == 0) {
                            for (int k = 0; k < segments_bot.size(); k++) {
                                // Loop through all column positions of each segment.
                                for (int m = 0; m < segments_bot[k].size(); m++) {
                                    // If verified column connections (col_cont_pos vector) 
                                    // match column positions of a segment, the number of non-connected
                                    // top segments is decremented.
                                    // The point of this is to first verify these rows aren't
                                    // non-contiguous in some manner. All top segments must have a path.
                                    for (int col = 0; col < col_cont_pos.size(); col++) {
                                        if (segments_bot[k][m] == col_cont_pos[col]) {
                                            bot_segments--;
                                        }
                                    }
                                }
                            }
                            if (bot_segments > 0) {
                                partially = true;
                            }
                        }
                    }
                }
                row.push_back(partially);
            }
            table.push_back(row);
        }
        puzzle_factory::partially_contiguous_table = table;
    }

    void puzzle_factory::generate_columnally_contiguous_table() {
        vector<vector<bool>> table;

        for (int i = 0; i < pow(2, order); i++) {

            vector<bool> row;
            vector<char> top = puzzle_factory::gen_row(i, order);
            vector<vector<int>> segments_top = puzzle_factory::get_contiguous_segments(top);
            for (int j = 0; j < pow(2, order); j++) {
                bool columnally = false;
                if (!puzzle_factory::pool_table[i][j] && !puzzle_factory::partially_contiguous_table[i][j]) {
                    vector<char> bot = puzzle_factory::gen_row(j, order);

                    vector<vector<char>> matrix = { top, bot };
                    // At least potentially contiguous, in this case.

                    vector<vector<int>> segments_bot = puzzle_factory::get_contiguous_segments(bot);


                    int top_segments = segments_top.size();
                    int bot_segments = segments_bot.size();
                    vector<int> col_cont_pos;

                    for (int k = 0; k < top.size(); k++) {
                        if (top[k] == 0 && bot[k] == 0) {
                            col_cont_pos.push_back(k);
                        }
                    }

                    for (int k = 0; k < segments_bot.size(); k++) {
                        for (int m = 0; m < col_cont_pos.size(); m++) {
                            for (int pos = 0; pos < segments_bot[k].size(); pos++) {
                                if (col_cont_pos[m] == segments_bot[k][pos]) {
                                    bot_segments--;
                                    break;
                                }
                            }
                        }
                    }

                    /* for (int k = 0; k < segments_top.size(); k++) {
                         for (int m = 0; m < col_cont_pos.size(); m++) {
                             for (int pos = 0; pos < segments_top[k].size(); pos++) {
                                 if (col_cont_pos[m] == segments_top[k][pos]) {
                                     top_segments--;
                                     break;
                                 }
                             }
                         }
                     }*/
                    columnally = (bot_segments == 0 && segments_bot.size() != 0);
                }
                row.push_back(columnally);
            }
            table.push_back(row);
        }
        puzzle_factory::columnally_contiguous_table = table;
    }

    void puzzle_factory::generate_all_columns_contiguous_table() {
        vector<vector<bool>> table;

        for (int i = 0; i < pow(2, order); i++) {

            vector<bool> row;
            vector<char> top = puzzle_factory::gen_row(i, order);
            vector<vector<int>> segments_top = puzzle_factory::get_contiguous_segments(top);
            for (int j = 0; j < pow(2, order); j++) {
                bool columnally = false;
                if (!puzzle_factory::pool_table[i][j] && !puzzle_factory::partially_contiguous_table[i][j]) {
                    vector<char> bot = puzzle_factory::gen_row(j, order);

                    vector<vector<char>> matrix = { top, bot };
                    // At least potentially contiguous, in this case.

                    vector<vector<int>> segments_bot = puzzle_factory::get_contiguous_segments(bot);


                    int top_segments = segments_top.size();
                    int bot_segments = segments_bot.size();
                    vector<int> col_cont_pos;

                    for (int k = 0; k < top.size(); k++) {
                        if (top[k] == 0 && bot[k] == 0) {
                            col_cont_pos.push_back(k);
                        }
                    }

                    for (int k = 0; k < segments_bot.size(); k++) {
                        for (int m = 0; m < col_cont_pos.size(); m++) {
                            for (int pos = 0; pos < segments_bot[k].size(); pos++) {
                                if (col_cont_pos[m] == segments_bot[k][pos]) {
                                    bot_segments--;
                                    break;
                                }
                            }
                        }
                    }

                    for (int k = 0; k < segments_top.size(); k++) {
                        for (int m = 0; m < col_cont_pos.size(); m++) {
                            for (int pos = 0; pos < segments_top[k].size(); pos++) {
                                if (col_cont_pos[m] == segments_top[k][pos]) {
                                    top_segments--;
                                    break;
                                }
                            }
                        }
                    }
                    columnally = (bot_segments == 0 && top_segments == 0);
                }
                row.push_back(columnally);
            }
            table.push_back(row);
        }
        puzzle_factory::all_columns_contiguous_table = table;
    }

    void puzzle_factory::generate_recontiguous_table() {
        vector<vector<bool>> table;

        for (int i = 0; i < pow(2, order); i++) {

            vector<bool> row;
            vector<char> top = puzzle_factory::gen_row(i, order);
            vector<vector<int>> segments_top = puzzle_factory::get_contiguous_segments(top);
            for (int j = 0; j < pow(2, order); j++) {
                bool recontiguous = false;
                if (!puzzle_factory::pool_table[i][j]) {
                    vector<char> bot = puzzle_factory::gen_row(j, order);
                    vector<vector<int>> segments_bot = puzzle_factory::get_contiguous_segments(bot);

                    int top_count = segments_top.size();
                    int bot_count = segments_bot.size();

                    if (top_count > 1) {
                        vector<int> col_cont_pos;
                        for (int k = 0; k < top.size(); k++) {
                            if (top[k] == 0 && bot[k] == 0) {
                                col_cont_pos.push_back(k);
                            }
                        }

                        // Iterate through the segments
                        for (int k = 0; k < segments_bot.size(); k++) {
                            int con_count = 0;
                            // Iterate through the positions in a segment
                            for (int m = 0; m < segments_bot[k].size(); m++) {
                                // Iterate by the connective columns
                                for (int pos = 0; pos < col_cont_pos.size(); pos++) {
                                    // If a segment position matches a known connective column
                                    if (segments_bot[k][m] == col_cont_pos[pos]) {
                                        con_count++;
                                    }
                                }
                            }
                            if (con_count == top_count) {
                                recontiguous = true;
                                break;
                            }
                        }
                    }
                }
                row.push_back(recontiguous);
            }
            table.push_back(row);
        }
        puzzle_factory::recontiguous_table = table;
    }

    void puzzle_factory::generate_noncon_column_counts() {
        vector<vector<int>> table;
        for (int i = 0; i < pow(2, order); i++) {
            for (int j = 0; j < pow(2, order); j++) {


            }
        }
    }

    void puzzle_factory::generate_decision_table() {
        vector<vector<Choices>> table;

        for (int i = 0; i < pow(2, order); i++) {
            vector<Choices> row;
            for (int j = 0; j < pow(2, order); j++) {
                if (partially_contiguous_table[i][j]) {
                    row.push_back(Choices::Partial);
                }
                else if (columnally_contiguous_table[i][j]) {
                    Choices state = Choices::Column;

                    if (all_columns_contiguous_table[i][j]) {
                        state = Choices::AllColumn;
                    }

                    row.push_back(state);
                }
                else if (all_columns_contiguous_table[i][j]) {
                    row.push_back(Choices::AllColumn);
                }
                else if (recontiguous_table[i][j]) {
                    row.push_back(Choices::Recontiguous);
                }
                else if (pool_table[i][j]) {
                    row.push_back(Choices::Illegal);
                }
                else if (j == pow(2, order) - 1) {
                    row.push_back(Choices::End);
                }
                else {
                    // Biggest potential for error here
                    row.push_back(Choices::Illegal);
                }
            }
            table.push_back(row);
        }
        puzzle_factory::decision_table = table;
    }

    void puzzle_factory::pattern_generator(Control status, int func_depth,
        vector<int> seeds, bool shore, bool is_partial_warning, Choices choice, int noncon_col_count) {
        func_depth++;
        puzzle_factory::call_count++;
        vector<Choices> choices;

        switch (status) {
        case Control::Start: {
            for (int i = 0; i < pow(2, order); i++) {
                for (int j = 0; j < pow(2, order); j++) {
                    vector<int> temp = seeds;
                    Choices state = decision_table[i][j];
                    shore = false;

                    if (i == (pow(2, order) - 1)) {
                        shore = true;
                        state = decision_table[0][j];
                    }

                    is_partial_warning = (state == Choices::Partial);
                    
                    if (state != Choices::Illegal) {
                        temp.push_back(i);
                        temp.push_back(j);
                        // Cannot be a noncontiguous row followed by a row of 1's
                        if ((!contiguous_row_list[i])) {
                            pattern_generator(Control::Run, 1, temp, shore, true, state);
                        }
                        else {
                            pattern_generator(Control::Run, 1, temp, shore, is_partial_warning, state);
                        }
                    }
                }
            }

            break;
        }
        case Control::Run: {
            int seed_one = seeds[seeds.size() - 2];
            int seed_two = seeds[seeds.size() - 1];

            switch (choice) {
            case Choices::Recontiguous: {
                choices = { Recontiguous, AllColumn, Column, Partial, End };
                is_partial_warning = false;
                break;
            }
            case Choices::AllColumn: {
                if (!is_partial_warning) {
                    choices = { Recontiguous, AllColumn, Column, Partial, End };
                }
                else {
                    choices = { Recontiguous, AllColumn, Column };
                }
                break;
            }
            case Choices::Column: {
                if (!is_partial_warning) {
                    choices = { Recontiguous, AllColumn, Column, Partial, End };
                }
                else {
                    choices = { Recontiguous, AllColumn, Column};
                }
                break;
            }
            case Choices::Partial: {
                choices = { Recontiguous, AllColumn, Column };
                is_partial_warning = true;
                break;
            }
            case Choices::End: {
                choices = { End };
                break;
            }
            }

            // If vector is full
            if (seeds.size() == order) {
                pattern_generator(Control::Stop, func_depth, seeds, shore, false, choice);
            }
            else {
                // If not, choose the next row.
                for (int i = 0; i < pow(2, order); i++) {
                    vector<int> temp;
                    bool shore_temp = shore;
                    temp.clear();
                    temp = seeds;
                    temp.push_back(i);
                    Choices new_choice = decision_table[seed_two][i];

                    if ((seed_two == (pow(2, order) - 1)) && shore) {
                        new_choice = decision_table[0][i];
                    }

                    if ((temp.size() == order) && is_partial_warning) {
                        choices = { Choices::Recontiguous };
                    }
                    else if ((temp.size() == order)) {
                        choices = { Choices::Recontiguous, Choices::AllColumn, Choices::Column, Choices::End };
                    }

                    if (find(choices.begin(), choices.end(), new_choice) != choices.end()) {
                        pattern_generator(Control::Run, func_depth, temp, shore, is_partial_warning, choice);
                    }
                }
            }           
            break;
        }
        case Control::Pause: {
            break;
        }
        case Control::Stop: {
            cout << "Matrix -- ";
            for (int pos = 0; pos < seeds.size(); pos++) {
                cout << to_string(seeds[pos]) << " -- ";
            }
            cout << endl;
            for (int j = 0; j < order; j++) {
                cout << puzzle_factory::gen_row(seeds[j]) << endl;
            }
            puzzle_factory::good_patterns++;
            break;
        }
        }
    }

    void node_map::update_map(row candidate) {
        int noncontiguous_nodes = predecessor_nodes.size();
        for (int i = 0; i < candidate.nodes.size(); i++) {
            for (int j = 0; j < candidate.nodes[i].get_columns.size(); j++) {

            }
        }
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

    bool puzzle_factory::pathable(vector<vector<char>> matrix) {
        vector<vector<int>> segments = puzzle_factory::get_contiguous_segments(matrix[0]);
        vector<bool> segment_pathability(segments.size(), false);

        if (segments.size() > 0) {
            for (int i = 0; i < segments.size(); i++) {
                bool way_down = false;
                for (int j = 0; j < segments[i].size(); j++) {
                    if (matrix[0][segments[i][j]] == 0 && matrix[1][segments[i][j]] == 0) {
                        way_down = true;
                    }
                    if (way_down) {
                        segment_pathability[i] = true;
                        break;
                    }
                }
            }
        }
        return std::all_of(std::begin(segment_pathability),
            std::end(segment_pathability), [](bool i) { return i; });
    }

    int puzzle_factory::count_water(vector<vector<char>> matrix) {
        int water = 0;

        for (int i = 0; i < matrix.size(); i++) {
            water += (matrix[i].size() - accumulate(matrix[i].begin(), matrix[i].end(), 0));
        }

        return water;
    }


    int puzzle_factory::traverse(vector<vector<char>>& matrix, int point[],
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

    void puzzle_factory::test_small_patterns() {

    }

    void puzzle_factory::set_builder(string flag, vector<int> seeds, int depth, int top_seed, int bottom_seed) {
        depth++;
        if (flag == "start") {
            for (int i = 0; i < pow(2, order); i++) {
                for (int j = 0; j < pow(2, order); j++) {
                    seeds.clear();
                    seeds.push_back(i);
                    seeds.push_back(j);
                    puzzle_factory::set_builder("", seeds, depth, i, j);
                }
            }
            cout << "Count: " << puzzle_factory::good_patterns << endl;
            cin.get();
        }
        else {
            if (order > 2) {
                for (int i = 0; i < pow(2, order); i++) {
                    vector<int> add_seeds = seeds;
                    if ((seed_table[top_seed][i] && seed_table[bottom_seed][i]) && middle_rows_database[top_seed][i][bottom_seed]) {
                        auto it_pos = add_seeds.end() - 1;
                        auto it = add_seeds.insert(it_pos, i);
                        if (add_seeds.size() < order) {
                            puzzle_factory::set_builder("", add_seeds, depth, i, bottom_seed);
                        }
                        else {
                            if (add_seeds[0] == add_seeds[add_seeds.size() - 1]) {
                                for (int j = 0; j < add_seeds.size(); j++) {
                                    if (add_seeds[j] != add_seeds[0]) {
                                        cout << "Matrix -- " << endl;
                                        for (int j = 0; j < order; j++) {
                                            cout << puzzle_factory::gen_row(add_seeds[j]) << endl;
                                        }
                                        puzzle_factory::good_patterns++;
                                        break;
                                    }
                                }
                            }
                            else {
                                cout << "Matrix -- " << endl;
                                for (int j = 0; j < order; j++) {
                                    cout << puzzle_factory::gen_row(add_seeds[j]) << endl;
                                }
                                puzzle_factory::good_patterns++;
                            }
                        }
                    }
                }
            }
            else {
                for (int i = 0; i < pow(2, order); i++) {
                    vector<int> add_seeds = seeds;
                    if (middle_rows_database[top_seed][i][bottom_seed]) {
                        auto it_pos = add_seeds.end() - 1;
                        auto it = add_seeds.insert(it_pos, i);
                        if (add_seeds.size() < order) {
                            puzzle_factory::set_builder("", add_seeds, depth, i, bottom_seed);
                        }
                        else {
                            cout << "Matrix -- " << endl;
                            for (int j = 0; j < order; j++) {
                                cout << puzzle_factory::gen_row(add_seeds[j]) << endl;
                            }
                            puzzle_factory::good_patterns++;
                        }
                    }
                }
            }
        }




        /*int count = 0;
        for (int i = 0; i < puzzle_factory::seed_table.size(); i++) {
            for (int j = 0; j < puzzle_factory::seed_table[i].size(); j++) {
                if (puzzle_factory::seed_table[i][j]) {
                    for (int k = 0; k < puzzle_factory::seed_table[i].size(); k++) {
                        if (puzzle_factory::seed_table[k][j] && puzzle_factory::middle_rows_database[i][k][j]) {


                            cout << "i: " << to_string(i) << " j: " << to_string(j)
                                << " k: " << to_string(k) << endl;
                            cout << puzzle_factory::gen_row(i) << endl;
                            cout << puzzle_factory::gen_row(j) << endl;
                            cout << puzzle_factory::gen_row(k) << endl;
                        }
                        count++;
                    }
                }
            }
        }
        cout << "Count: " << to_string(count) << endl;*/
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

    string puzzle_factory::to_string_matrix(int seed) {
        vector<char> bin_vec = puzzle_factory::gen_row(seed, order*order);
        string ret = "";

        for (int i = 0; i < order; i++) {
            if (i > 0) {
                ret += "\n";
            }
            for (int j = 0 + (i * order); j < order + (i * order); j++) {
                ret += (to_string(bin_vec[j]) + " ");
            }
        }
        return ret;
    }

    void puzzle_factory::print_all_matrices(int columns) {

        cout << "Matrix Order: " << order << "x" << order << endl;
        string row = "";
        for (int i = 0; i < good_seeds.size() - 1; i++) {
            cout << "seed: " << to_string(good_seeds[i]) << endl;
            cout << (puzzle_factory::to_string_matrix(good_seeds[i])) << endl;;
        }
    }

    

   

    //
    // POTENTIALLY DEPRECATED CODE
    //

    vector<vector<vector<bool>>> puzzle_factory::get_middle_rows_database() {
        return puzzle_factory::middle_rows_database;
    }

    vector<vector<bool>> puzzle_factory::get_table() {
        return puzzle_factory::seed_table;
    }

    bool puzzle_factory::is_contiguous(vector<vector<char>> matrix) {
        vector<vector<char>> temp = matrix;
        int water = puzzle_factory::count_water(temp);
        int second_row_water = temp[1].size() - accumulate(temp[1].begin(), temp[1].end(), 0);
        int traversed_water;

        // Check if the second row is a straight wall of water.
        if (temp[1].size() - second_row_water == 0) {
            return true;
        }

        if (temp[1].size() - second_row_water == temp[1].size()) {
            return false;
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
        else if (second_row_water == 0 && water > 0) {
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

    bool puzzle_factory::check_matrix(vector<int> seeds) {
        // Get the position for the bottom row of the vector, or the last element.
        int vec_manip_pos = seeds.size() - 1;
        // Check if the top two rows are legal, then check if the bottom 
        // two rows are legal.
        if (puzzle_factory::seed_table[seeds[0]][seeds[1]] &&
            puzzle_factory::seed_table[seeds[vec_manip_pos]][seeds[vec_manip_pos - 1]]) {
            // Check to make sure there are no middle isolated water tiles.
            if (!puzzle_factory::middle_rows_database[seeds[0]][seeds[1]][seeds[2]]) {
                // Passes the top and bottom row checks but fails
                // the middle rows check.
                return false;
            }
        }
        // Fails the top or bottom row checks.
        else {
            return false;
        }
        // Passes all tests.
        return true;
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

        for (int i = 0; i < pow(2, pow(vec_length, 2)); i++) {
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
        puzzle_factory::set_bad_patterns(pow(2, order*order)
            - puzzle_factory::get_good_patterns());
    }

    void puzzle_factory::generate_database(int order) {
        vector<vector<vector<bool>>> database;
        vector<vector<bool>> table;
        vector<bool> row;
        int high_num = pow(2, order);

        // Initialize a boolean database with all values set to false
        for (int i = 0; i < high_num; i++) {
            row.push_back(false);
        }

        for (int j = 0; j < high_num; j++) {
            table.push_back(row);
        }

        for (int k = 0; k < high_num; k++) {
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
        for (int i = 0; i < high_num; i++) {
            // Top row generation.
            first_row = puzzle_factory::gen_row(i, order);
            for (int j = 0; j < high_num; j++) {
                // Second row generation.
                second_row = puzzle_factory::gen_row(j, order);
                for (int k = 0; k < high_num; k++) {
                    // Third row generation.
                    third_row = puzzle_factory::gen_row(k, order);
                    // Construct a matrix with these rows.
                    matrix = { first_row, second_row, third_row };
                    // Get count of water in the matrix.
                    int water = puzzle_factory::count_water(matrix);
                    // If there is no pool.

                    if (water == 0) {
                        database[i][j][k] = true;
                    }
                    else if (!puzzle_factory::has_pool(matrix, order)) {

                        int top_water = matrix[0].size() -
                            accumulate(matrix[0].begin(), matrix[0].end(), 0);
                        int mid_water = matrix[1].size() -
                            accumulate(matrix[1].begin(), matrix[1].end(), 0);
                        int btm_water = matrix[2].size() -
                            accumulate(matrix[2].begin(), matrix[2].end(), 0);
                        vector<vector<char>> temp = matrix;

                        if (water == top_water) {
                            database[i][j][k] = true;
                            continue;
                        }
                        else if (water == btm_water) {
                            database[i][j][k] = true;
                            continue;
                        }
                        else if (mid_water > 0) {

                            if (mid_water == 1 && (matrix[1][0] == 0
                                || matrix[1][matrix.size() - 1] == 0)) {
                                database[i][j][k] = true;
                                continue;
                            }
                            else if (mid_water == matrix[1].size()) {
                                database[i][j][k] = true;
                                continue;
                            }
                            else {
                                database[i][j][k] = puzzle_factory::pathable(temp);
                            }
                        }

                        /*if (btm_water > 0 && top_water > 0 && mid_water == 0) {
                            database[i][j][k] = false;
                        }
                        else if (mid_water > 0 && top_water > 0 || btm_water > 0) {
                            for (int n = 0; n < matrix[0].size(); n++) {

                                if (matrix[0][n] == 0) {
                                    int point[2] = { 0, n };
                                    puzzle_factory::traverse(temp, point, 0, 1);
                                }

                                if (matrix[1][n] == 0) {
                                    int point[2] = { 1, n };
                                    puzzle_factory::traverse(temp, point, 0, 1);
                                }

                            }
                            int temp_second_row_water = temp[1].size() -
                                accumulate(temp[1].begin(), temp[1].end(), 0);

                            if (temp_second_row_water != 0) {
                                database[i][j][k] = false;
                            }
                        }
                        else if (water == mid_water) {
                            if (mid_water != matrix[1].size()) {
                                if (matrix[1][0] != 0 && matrix[1][matrix[1].size() - 1] != 0) {
                                    database[i][j][k] = false;
                                }
                                else {
                                    if (matrix[1][0] == 0) {
                                        int point[2] = { 1, 0 };
                                        puzzle_factory::traverse(temp, point, 0, 1);
                                    }
                                    else if (matrix[1][matrix[1].size() - 1] == 0) {
                                        int point[2] = { 1, matrix[1].size() - 1 };
                                        puzzle_factory::traverse(temp, point, 0, 1);
                                    }

                                    int temp_second_row_water = temp[1].size() -
                                        accumulate(temp[1].begin(), temp[1].end(), 0);

                                    if (temp_second_row_water != 0) {
                                        database[i][j][k] = false;
                                    }
                                }
                            }
                        }*/

                    }
                }
            }
        }

        // The resultant database should be a complete check against faulty patterns when
        // combined with the edge rows table and column checking.
        puzzle_factory::middle_rows_database = database;
    }

    void puzzle_factory::generate_seed_table(int order) {

        // Initialize the space for the table with all false.
        vector<vector<bool>> table;
        int bit_length = pow(2, order);

        for (int i = 0; i < bit_length; i++) {
            vector<bool> row;
            for (int j = 0; j < bit_length; j++) {
                row.push_back(false);
            }
            table.push_back(row);
        }

        // Initialize a two row matrix.
        vector<vector<char>> matrix(2);

        // 
        for (int i = 0; i < bit_length; i++) {
            matrix[0] = puzzle_factory::gen_row(i, order);
            for (int j = 0; j < bit_length; j++) {
                if (table[i][j] == false) {
                    matrix[1] = puzzle_factory::gen_row(j, order);
                    if (!puzzle_factory::has_pool(matrix, order)) {
                        vector<vector<char>> copy_matrix = matrix;
                        int water = puzzle_factory::count_water(copy_matrix);
                        int top_row_water = matrix[0].size() -
                            accumulate(copy_matrix[0].begin(), copy_matrix[0].end(), 0);

                        if (water > 0) {
                            if (water != top_row_water) {
                                for (int k = 0; k < matrix[1].size(); k++) {
                                    if (copy_matrix[1][k] == 0) {
                                        int point[2] = { 1, k };
                                        puzzle_factory::traverse(copy_matrix, point, 0, 1);
                                    }
                                }
                                if (matrix[0].size() -
                                    accumulate(copy_matrix[0].begin(),
                                        copy_matrix[0].end(), 0) == 0) {

                                    table[i][j] = true;
                                }
                            }
                            else {
                                for (int k = 0; k < matrix[0].size() - 1; k++) {
                                    if (matrix[0][k] == 0) {
                                        int point[2] = { 0, k };
                                        puzzle_factory::traverse(copy_matrix, point, 0, 1);
                                        break;
                                    }
                                }
                                if (accumulate(copy_matrix[0].begin(), copy_matrix[0].end(), 0)
                                    == copy_matrix[0].size()) {
                                    table[i][j] = true;
                                }
                            }
                        }
                        else {
                            table[i][j] = true;
                        }
                        /*for (int k = 0; k < matrix[0].size(); k++) {
                            if (copy_matrix[0][k] == 0) {
                                int point[2] = { 0, k };
                                puzzle_factory::traverse(copy_matrix, point, 0, 1);
                            }
                        }
                        if (accumulate(copy_matrix[0].begin(), copy_matrix[0].end(), 0)
                            == copy_matrix[0].size()) {
                            table[i][j] = true;
                        }*/
                        /*int water = puzzle_factory::count_water(matrix);
                        for (int k = 0; k < order; k++) {
                            int point[2] = { 1, k };
                            if (matrix[1][k] == 0) {
                                water -= puzzle_factory::traverse(copy_matrix, point, 0, 1);
                            }
                            if (water == 0) {
                                table[i][j] = true;
                                break;
                            }
                        }*/
                    }
                }
            }
        }
        puzzle_factory::seed_table = table;
    }
}