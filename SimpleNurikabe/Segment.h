#ifndef SEGMENT_H
#define SEGMENT_H
#include "pch.h"
class Segment
{
public:
    Segment(std::vector<int> passed_columns);
    ~Segment();
    std::vector<int> columns;
    int segSize;
    int leftMostColumn;
    int rightMostColumn;
    bool isSingleColumn();
    static std::vector<Segment> makeSegments(const std::vector<char>& bits);
};
#endif // !SEGMENT_H



