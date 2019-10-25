#ifndef ROW_H
#define ROW_H
#include "pch.h"

namespace Nurikabe {
    class row
    {
    public:
        row(int seed_number, int row_length);
        ~row();
        vector<char> get_bits();
        vector<node> nodes;
        string gen_row_string();
        int get_seed();
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


