#ifndef NODE_MAP_H
#define NODE_MAP_H
#include "pch.h"

namespace Nurikabe {
    class node_map
    {
    public:
        node_map(int matrix_order);
        ~node_map();
        void update(row row, vector<node> new_nodes, vector<vector<int>> node_connections);
        bool is_contiguous();
        int get_node_count();
        int get_connection_count();
        vector<int> get_row_seeds();
    private:
        int order;
        int row_count;
        int node_count;
        int contiguous_node_count;
        int connections;
        bool contiguous;
        vector<int> associated_rows;
        vector<node> predecessor_nodes;
    };
}

#endif

