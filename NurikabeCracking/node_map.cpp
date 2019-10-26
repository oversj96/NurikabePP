#include "pch.h"

namespace nurikabe {
    node_map::node_map(row top_row)
    {
        if (top_row.get_bits().size() > 1) {
            node_map::order = top_row.get_bits().size();
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

    node_map::node_map(row top_row, row bot_row)
    {
        if (top_row.get_bits().size() > 1) {
            node_map::order = top_row.get_bits().size();
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

        node_map::predecessor_nodes = top_row.p_nodes;
        //node_map
    }

    node_map::node_map()
    {
       // default ctor means do nothing!
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


    void node_map::update_current_nodes(vector<std::unique_ptr<node>> candidates) {
        for (int i = 0; i < candidates.size(); i++) {
            for (int j = 0; j < node_map::predecessor_nodes.size(); j++) {
                if (!candidates[i]->is_contiguous()) {
                    if (candidates[i]->connects_to(node_map::predecessor_nodes[j])
                        && node_map::predecessor_nodes[j]->is_contiguous()) {
                        candidates[i]->set_contiguous();
                    }
                }
                else {
                    if (candidates[i]->connects_to(node_map::predecessor_nodes[j]) &&
                        !node_map::predecessor_nodes[j]->is_contiguous()) {
                        node_map::predecessor_nodes[j]->set_contiguous();
                    }
                }
            }
        }
    }
}

