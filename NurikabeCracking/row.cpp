#include "pch.h"

namespace nurikabe {
    row::row(int seed_number, int row_length)
    {
        if (seed_number >= 0 && row_length > 0) {
            row::seed = seed_number;
            row::length = row_length;
            row::store_row = store_row;
            row::bits = row::generate_row_bits();
            row::create_nodes();
        }
    }


    row::~row()
    {
        // Nothing to delete.
    }

    void row::create_nodes() {
        vector<vector<int>> segments;
        vector<int> spots;
        bool segment = false;

        for (int i = 0; i < bits.size(); i++) {
            if (bits[i] == 0) {
                if (!segment) {
                    spots.clear();
                    segment = true;
                    spots.emplace_back(i);
                }
                else {
                    spots.emplace_back(i);
                }
            }
            else if (bits[i] == 1 && segment) {
                segment = false;
                segments.emplace_back(spots);
            }
        }
        if (segment) {
            segment = false;
            segments.emplace_back(spots);
        }

        vector<std::unique_ptr<node>> nodes;

        for (int i = 0; i < segments.size(); i++) {
            node n(row::seed, i, row::length, segments[i]);
            auto node(std::make_unique<node>(n));
            nodes.emplace_back(node);
        }

        row::p_nodes = nodes;
    }

    vector<char> row::get_bits() {
        return row::bits;

    }

    node_map row::map_out(row other) {
        node_map map(length, this);

    }

    int row::gen_seed() {
        int value = 0;
        int out_seed = 0;

        for (int i = 0; i < row::bits.size(); i++) {
            if (row::bits[i] == 1) {
                value = std::pow(2, (row::bits.size() - i - 1));
            }
            else {
                value = 0;
            }
            out_seed += value;
        }
        return out_seed;
    }



    vector<char> row::generate_row_bits() {
        vector<char> row;
        // We know the length of the vector ahead of time, so if we reserve it and use
        // the [] subscript operator, we can make row generation extremely fast. On order of
        // 1000x faster than using push_back for large rows.
        row.reserve(row::length);
        // If the seed is -1, this represents a vector beyond the scope of the matrix order, 
        // and will generate x's of the specified vector length. This is for bounds checking
        // during the seed space creation.
        for (int i = 0; i < row::length; i++) {
            row[(row::seed >> i) & 1];
        }
        return row;
    }

    string row::gen_row_string() {
        vector<char> bin_vec = row::generate_row_bits();
        string row = "";

        for (int i = bin_vec.size() - 1; i > -1; i--) {
            row += (std::to_string(bin_vec[i]) + " ");
        }

        return row;
    }

    int row::get_seed() {
        return row::seed;
    }
}


