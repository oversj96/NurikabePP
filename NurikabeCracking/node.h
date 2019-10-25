#ifndef NODE_H
#define NODE_H
#include "pch.h"

namespace Nurikabe {
    class node
    {
    public:
        node(int row_num, int node_num, int length, vector<int> columns);
        ~node();
        int get_row_number();
        int get_node_number();
        int get_length();
        int get_leftmost_zero();
        vector<int> get_columns();
        vector<int> connections;
        bool contiguous;
        //vector<int> get_connected_nodes();
    private:
        //vector<int> connected_node_ids;
        int row_number;
        vector<int> node_columns;
        int node_number;
        int length;
        int leftmost_zero;       
        
    };
}
#endif
