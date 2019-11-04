#include "pch.h"
#include "node_map.h"
namespace nurikabe {

    node_map::node_map(row top_row, row sec_row)
    {
        if (top_row.get_bits().size() > 1) {
            node_map::order = top_row.get_bits().size();
            node_map::contiguous = false;
            node_map::row_count = 2;
        }
        else {
            std::cout
                << "Error: The matrix order given was less than 1."
                << " Cannot generate a node map with no rows."
                << std::endl;
        }

        node_map::predecessor_nodes = top_row.p_nodes;
        node_map::successor_nodes = sec_row.p_nodes;

        std::vector<node> temp = predecessor_nodes;
        for (int i = 0; i < temp.size(); i++) {
            for (int j = 0; j < successor_nodes.size(); j++) {
                temp[i].connects_to(&successor_nodes[j]);
            }
        }

        for (int i = 0; i < temp.size(); i++) {
            if (!temp[i].is_contiguous()) {
                node_map::contiguous = false;
                break;
            }
        }       
    }

    node_map::node_map(std::vector<node> predecessors, int row_count, int order) {
        node_map::predecessor_nodes = predecessors;
        node_map::row_count = row_count;
        node_map::order = order;
    }

    node_map::~node_map()
    {
    }

    int node_map::get_last_row_seed() {
        return node_map::successor_nodes[0].get_row_number();
    }

    bool node_map::is_contiguous() {
        return node_map::contiguous;
    }

    bool node_map::is_fully_contiguous() {
        return (node_map::pred_count == predecessor_nodes.size()
            && node_map::cand_count == successor_nodes.size());
    }

    std::vector<node> node_map::get_successor_nodes() {
        return node_map::successor_nodes;
    }

    int node_map::get_row_count() {
        return node_map::row_count;
    }

    void node_map::update_current_nodes(std::vector<node> candidates) {
        std::vector<node> temp = predecessor_nodes;
        for (int i = 0; i < candidates.size(); i++) {
            for (int j = 0; j < temp.size(); j++) {
                if (!candidates[i].is_contiguous()) {
                    if (candidates[i].connects_to(&temp[j])
                        && temp[j].is_contiguous()) {
                        candidates[i].set_contiguous();
                    }
                }
                else {
                    if (candidates[i].connects_to(&temp[j]) &&
                        !temp[j].is_contiguous()) {
                        temp[j].set_contiguous();
                    }
                }
            }
        }

        node_map::pred_count = 0;
        node_map::cand_count = 0;

        for (int i = 0; i < temp.size(); i++) {
            if (temp[i].is_contiguous()) {
                node_map::pred_count++;
            }
        }
        for (int i = 0; i < candidates.size(); i++) {
            if (candidates[i].is_contiguous()) {
                node_map::cand_count++;
            }
        }

        node_map::contiguous = true;
        for (int i = 0; i < temp.size(); i++) {
            if (!temp[i].is_contiguous()) {
                node_map::contiguous = false;
                break;
            }
        }

        node_map::successor_nodes = candidates;
        node_map::row_count++;
    }
}

