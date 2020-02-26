#include "pch.h"
#include "Segment.h"


Segment::Segment(std::vector<int> passed_columns)
{
    Segment::columns = passed_columns;
    Segment::segSize = passed_columns.size();
    Segment::leftMostColumn = passed_columns[0];
    Segment::rightMostColumn = passed_columns[Segment::segSize - 1];
}

Segment::~Segment()
{
}

bool Segment::operator ==(const Segment &s) {
    auto other = dynamic_cast<const Segment*>(&s);
    return other != 0;
}

bool Segment::isSingleColumn() {
    return Segment::leftMostColumn == Segment::rightMostColumn;
}

std::vector<Segment> Segment::makeSegments(const std::vector<char>& bits) {
    std::vector<int> seg;
    std::vector<Segment> segments;

    for (size_t i = 0; i < bits.size(); ++i) {
        if (bits[i] == 0) {
            seg.push_back(i);
            if (i == bits.size() - 1) {
                segments.push_back(seg);
            }
        }
        else if (bits[i] == 1 && seg.size() > 0) {
            segments.push_back(seg);
            seg.clear();
        }
    }

    return segments;
}

void Segment::addSegmentColumns(const Segment &s2) {
    for (int key : s2.columns) {
        Segment::columns.push_back(key);
    }
}
