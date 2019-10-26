#ifndef NODE_H
#define NODE_H
#include "pch.h"

namespace nurikabe {
    class node
    {
    public:
        node(int row_num, int node_num, int length, vector<int> columns);
        ~node();
        int get_row_number();
        bool connects_to(std::unique_ptr<node>& n);
        void set_contiguous();
        int get_node_number();
        int get_length();
        int get_leftmost_zero();
        vector<std::unique_ptr<node>> get_connected_nodes();
        vector<int> get_columns();
        vector<int> connections;
        vector<std::unique_ptr<node>> connected_nodes;
        bool is_contiguous();
        //vector<int> get_connected_nodes();
    private:
        bool contiguous;
        //vector<int> connected_node_ids;
        int row_number;
        vector<int> node_columns;      
        int node_number;
        int length;
        int leftmost_zero;       
        
    };
}
#endif
