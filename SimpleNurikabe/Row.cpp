#include "pch.h"
#include "Row.h"


Row::Row(int seed, int length, std::vector<Segment> segments)
{
    Row::seed = seed;
    Row::length = length;
    Row::bits = Row::generateBits(Row::seed, Row::length);
    if (segments.size() > 0) {
        Row::segments = segments;
    }
    else {
        Row::segments = Segment::makeSegments(Row::bits);
    }
}


Row::~Row()
{
}


std::vector<char> Row::generateBits(int seed, int length) {
    std::vector<char> bits;
    bits.reserve(length);

    for (int i = length - 1; i >= 0; --i) {
        int bitVal = std::pow(2, i);
        if (seed >= bitVal) {
            seed -= bitVal;
            bits.push_back(1);
        }
        else {
            bits.push_back(0);
        }
    }

    return bits;
}