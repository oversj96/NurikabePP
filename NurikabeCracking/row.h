#ifndef ROW_H
#define ROW_H
#include "pch.h"

namespace nurikabe {
    class row
    {
    public:
        row(int seed_number, int row_length);
        ~row();
        vector<char> get_bits();
        vector<std::unique_ptr<node>> p_nodes;
        string gen_row_string();
        int get_seed();
        node_map map_out(row other);
    protected:
        int length;
        int seed;
        vector<char> bits;
    private:
        void create_nodes();
        vector<char> generate_row_bits();
        int gen_seed();
        bool store_row;      
    };
}
#endif


