#include "pch.h"

namespace Nurikabe {
    node::node(int row_num, int node_num, int length, int leftmost_zero_col)
    {
        if (row_num >= 0 && node_num >= 0 && length > 0 && leftmost_zero_col >= 0) {
            node::row_number = row_num;
            node::node_number = node_num;
            node::length = length;
            node::leftmost_zero = leftmost_zero_col;
            node::contiguous = false;
        }
        else {
            std::cout << "Warning: node improperly initialized!" << std::endl;
            std::cout << "Row Number: " << std::to_string(row_num) << std::endl;
            std::cout << "Node Number: " << std::to_string(node_num) << std::endl;
            std::cout << "Zero Count in Node: " << std::to_string(length) << std::endl;
            std::cout << "Starting zero column position: " 
                << std::to_string(leftmost_zero_col) << std::endl;
        }
    }


    node::~node()
    {
        // Nothing to delete.
    }

    int node::get_row_number() {
        return node::row_number;
    }

    int node::get_node_number() {
        return node::node_number;
    }

    int node::get_length() {
        return node::length;
    }

    int node::get_leftmost_zero() {
        return node::leftmost_zero;
    }

}

