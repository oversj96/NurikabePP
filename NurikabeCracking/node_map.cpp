#include "pch.h"

namespace Nurikabe {
    node_map::node_map(int matrix_order, row top_row, const vector<vector<vector<int>>>& connections)
    {
        if (matrix_order > 1) {
            node_map::node_connections = connections;
            node_map::order = matrix_order;
            node_map::node_count = 0;
            node_map::contiguous_node_count = 0;
            node_map::connections = 0;
            node_map::contiguous = true;
            node_map::associated_rows.reserve(node_map::order);
            node_map::row_count = 0;
        }
        else {
            std::cout
                << "Error: The matrix order given was less than 1."
                << " Cannot generate a node map with no rows."
                << std::endl;
        }
    }

    node_map::~node_map()
    {
    }

    bool node_map::is_contiguous() {
        return node_map::contiguous;
    }

    int node_map::get_node_count() {
        return node_map::node_count;
    }

    int node_map::get_connection_count() {
        return node_map::connections;
    }

    vector<int> node_map::get_row_seeds() {
        return node_map::associated_rows;
    }


    void node_map::update_current_nodes(vector<node>& candidates) {

    }
}

