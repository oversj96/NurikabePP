#include "pch.h"
#include "shape.h"

shape::shape(int tiles, std::vector<std::vector<bool>> value_map, int x, int y, int id)
{
    shape::tile_count  = tiles;
    shape::shape_map   = value_map;
    shape::x_dimension = x;
    shape::y_dimension = y;
    shape::shape_id = id;
}

shape::shape(const shape &s2) {
    shape::tile_count = s2.tile_count;
    shape::shape_map = s2.shape_map;
    shape::x_dimension = s2.x_dimension;
    shape::y_dimension = s2.y_dimension;
    shape::shape_id = s2.shape_id;
}

shape::shape() {
    std::vector<std::vector<bool>> empty;
    shape::tile_count = 0;
    shape::shape_map = empty;
    shape::x_dimension = 0;
    shape::y_dimension = 0;
    shape::shape_id = 0;
}


shape::~shape()
{
}



int shape::tiles() const { return tile_count; }

auto shape::get_shape_vec() const { return shape_map; }
