#include "pch.h"
#include "Segment.h"

Segment::Segment(std::vector<int> columns)
{
  if (columns.size() > 0)
  {
    this->columns = columns;
    this->leftMostCol = this->columns[0];
    this->rightMostCol = this->columns[this->columns.size() - 1];
  }
  else
  {
    throw std::logic_error("Critical Error: Segment columns cannot be size 0!");
  }
}

Segment::~Segment() {}

bool Segment::connects(Segment &other)
{
  for (int i = 0; i < other.columns.size(); ++i)
  {
    if (other.columns[i] >= this->leftMostCol &&
        other.columns[i] <= this->rightMostCol)
    {
      return true;
    }
  }
  return false;
}