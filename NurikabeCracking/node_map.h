#ifndef NODE_MAP_H
#define NODE_MAP_H
#include "pch.h"

namespace Nurikabe {
    class node_map
    {
    public:
        node_map(int matrix_order, row top_row, const vector<vector<vector<int>>>& connections);
        ~node_map();
        void update_map(row row);

        bool is_contiguous();
        void update_current_nodes(vector<node>& candidates);
        int get_node_count();
        int get_connection_count();
        vector<int> get_row_seeds();
    private:
        int order;
        vector<vector<vector<int>>> node_connections;
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

