#ifndef TILE_H
#define TILE_H
#include "pch.h"
class tile
{
public:
    tile(int x_pos, int y_pos);
    ~tile();
    int x() const;
    int y() const;
private:
    int x_coordinate;
    int y_coordinate;
};
#endif // !TILE_H



