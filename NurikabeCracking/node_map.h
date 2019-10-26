#ifndef NODE_MAP_H
#define NODE_MAP_H
#include "pch.h"

namespace nurikabe {
    class node_map
    {
    public:
        node_map(row top_row);
        node_map(row top_row, row bot_row);
        node_map();
        ~node_map();
        bool is_contiguous();
        void update_current_nodes(vector<std::unique_ptr<node>> candidates);
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
        vector<std::unique_ptr<node>> predecessor_nodes;
    };
}
#endif

