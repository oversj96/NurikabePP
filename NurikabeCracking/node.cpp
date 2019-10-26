#include "pch.h"

namespace nurikabe {
    node::node(int row_num, int node_num, int length, vector<int> columns)
    {
        if (row_num >= 0 && node_num >= 0 && length > 0 && columns.size() >= 0) {
            node::row_number = row_num;
            node::node_number = node_num;
            node::length = length;
            node::node_columns = columns;
            node::leftmost_zero = columns[0];
            node::contiguous = false;
        }
        else {
            std::cout << "Warning: node improperly initialized!" << std::endl;
            std::cout << "Row Number: " << std::to_string(row_num) << std::endl;
            std::cout << "Node Number: " << std::to_string(node_num) << std::endl;
            std::cout << "Zero Count in Node: " << std::to_string(length) << std::endl;
            std::cout << "Column size: "
                << std::to_string(columns.size()) << std::endl;
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

    vector<int> node::get_columns() {
        return node::node_columns;
    }

    bool node::connects_to(std::unique_ptr<node>& n) {
        if (std::find(node::connected_nodes.begin(), node::connected_nodes.end(), n) 
            != node::connected_nodes.end()) {
            return true;
        }
        else {
            for (int i = 0; i < node::node_columns.size(); i++) {
                for (int j = 0; j < n->node_columns.size(); j++) {
                    if (node::node_columns[i] == n->node_columns[j]) {
                        node::connected_nodes.emplace_back(n);
                        return true;
                    }
                }
            }
        }      
        return false;
    }

    vector<std::unique_ptr<node>> node::get_connected_nodes() {
        return node::connected_nodes;
    }

    bool node::is_contiguous() {
        return node::contiguous;
    }

    void node::set_contiguous() {
        if (!contiguous) {
            node::contiguous = true;
            for (int i = 0; i < connected_nodes.size(); i++) {
                node::connected_nodes[i]->set_contiguous();
            }
        }
    }
}

