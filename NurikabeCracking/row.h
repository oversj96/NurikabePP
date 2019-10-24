#ifndef ROW_H
#define ROW_H
#include "pch.h"

namespace Nurikabe {
    class row
    {
    public:
        row(int seed_number, int row_length, bool store_row);
        ~row();
        vector<char> get_row_bits();
        int gen_seed();
        vector<char> generate_row_bits();
        string gen_row_string();
        int get_seed();
    protected:
        int length;
        int seed;
        vector<char> bits;
    private:
        bool store_row;
    };
}
#endif


