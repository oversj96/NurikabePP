#ifndef ROW_H
#define ROW_H
#include "node.h"
namespace nurikabe {
    class row
    {
    public:
        row(int seed_number, int row_length);
        ~row();
        std::vector<char> get_bits();
        std::vector<node> p_nodes;
        std::string gen_row_string();
        int get_seed();
        int get_contiguous_node_count();
    protected:
        /*std::unique_ptr<row> above_row;
        std::unique_ptr<row> below_row;
        bool is_top;
        bool is_bottom;*/
        int length;
        int seed;
        std::vector<char> bits;
    private:
        int contiguous_node_count;
        void create_nodes();
        std::vector<char> generate_row_bits();
        int gen_seed();
        bool store_row;      
    };
}
#endif


