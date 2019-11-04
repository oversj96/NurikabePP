#ifndef NODE_H
#define NODE_H

namespace nurikabe {
    class node
    {
    public:
        node(int row_num, int node_num, int length, std::vector<int> columns);
        ~node();
        int get_row_number();
        bool connects_to(node* n);
        void set_contiguous();
        int get_node_number();
        int get_length();
        int get_leftmost_zero();
        std::vector<node*> get_connected_nodes();
        std::vector<int> get_columns();
        std::vector<int> connections;
        std::vector<node*> connected_nodes;
        bool is_contiguous();
        //vector<int> get_connected_nodes();
    private:
        bool contiguous;
        //vector<int> connected_node_ids;
        int row_number;
        std::vector<int> node_columns;      
        int node_number;
        int length;
        int leftmost_zero;       
        
    };
}
#endif
