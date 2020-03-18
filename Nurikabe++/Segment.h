#ifndef SEGMENT_H
#define SEGMENT_H
#include "pch.h"

class Segment
{
public:
  int leftMostCol;
  int rightMostCol;
  std::vector<int> columns;
  Segment(std::vector<int> columns);
  ~Segment();
  bool connects(Segment &other);
};
#endif // !SEGMENT_H
