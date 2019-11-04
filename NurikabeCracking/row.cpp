#include "pch.h"
#include "row.h"
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
        std::vector<std::vector<int>> segments;
        std::vector<int> spots;
        bool segment = false;

        for (int i = 0; i < bits.size(); i++) {
            if (bits[i] == 0) {
                if (!segment) {
                    spots.clear();
                    segment = true;
                    spots.push_back(i);
                }
                else {
                    spots.push_back(i);
                }
            }
            else if (bits[i] == 1 && segment) {
                segment = false;
                segments.push_back(spots);
            }
        }
        if (segment) {
            segment = false;
            segments.push_back(spots);
        }

        std::vector<node> nodes;

        for (int i = 0; i < segments.size(); i++) {
            nodes.push_back(node(row::seed, i, row::length, segments[i]));
        }

        row::p_nodes = nodes;
    }

    std::vector<char> row::get_bits() {
        return row::bits;
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

    int row::get_contiguous_node_count() {
        return row::contiguous_node_count;
    }

    std::vector<char> row::generate_row_bits() {
        std::vector<char> row;
        // We know the length of the std::vector ahead of time, so if we reserve it 
        // we can make row generation extremely fast by eliminating the need for reallocation. 

        row.reserve(row::length);
        // If the seed is -1, this represents a std::vector beyond the scope of the matrix order, 
        // and will generate x's of the specified std::vector length. This is for bounds checking
        // during the seed space creation.
        for (int i = 0; i < row::length; i++) {
            row.push_back((row::seed >> i) & 1);
        }
        return row;
    }

    std::string row::gen_row_string() {
        std::vector<char> bin_vec = row::generate_row_bits();
        std::string row = "";

        for (int i = bin_vec.size() - 1; i > -1; i--) {
            row += (std::to_string(bin_vec[i]) + " ");
        }

        return row;
    }

    int row::get_seed() {
        return row::seed;
    }
}


