#ifndef NODE_MAP_H
#define NODE_MAP_H
#include "row.h"
namespace nurikabe {
    class node_map
    {
    public:
        node_map(row top_row, row sec_row);
        node_map(std::vector<node> predecessors, int row_count, int order);
        ~node_map();
        bool is_contiguous();
        bool is_fully_contiguous();
        int get_last_row_seed();
        int get_row_count();
        std::vector<node> get_successor_nodes();
        void update_current_nodes(std::vector<node> candidates);
    private:
        int order;
        int pred_count;
        int cand_count;
        int row_count;
        bool contiguous;
        std::vector<node> predecessor_nodes;
        std::vector<node> successor_nodes;
    };
}
#endif

