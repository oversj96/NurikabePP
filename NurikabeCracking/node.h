#ifndef NODE_H
#define NODE_H
#include "pch.h"

namespace Nurikabe {
    class node
    {
    public:
        node(int row_num, int node_num, int length, int leftmost_zero_col);
        ~node();
        int get_row_number();
        int get_node_number();
        int get_length();
        int get_leftmost_zero();
        bool contiguous;
        //vector<int> get_connected_nodes();
    private:
        //vector<int> connected_node_ids;
        int row_number;
        int node_number;
        int length;
        int leftmost_zero;       
    };
}
#endif
