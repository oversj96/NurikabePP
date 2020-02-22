#ifndef ROW_H
#define ROW_H
#include "pch.h"
#include "Segment.h"
class Row
{
public:
    Row(int seed, int length, std::vector<Segment> segments);
    ~Row();
    int seed;
    int length;
    std::vector<char> bits;
    std::vector<Segment> segments;
    static std::vector<char> generateBits(int seed, int length);

};
#endif // !ROW_H



