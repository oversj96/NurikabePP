#ifndef SHAPE_H
#define SHAPE_H
#include "pch.h"

class shape
{
public:  
    shape(int tiles, std::vector<std::vector<bool>> map, int x, int y, int id);
    shape(const shape &s2);
    shape();
    ~shape();
    int tiles() const;
    auto get_shape_vec() const;
    int shape_id;
private:
    int tile_count = 0;
    std::vector<std::vector<bool>> shape_map;
    int x_dimension = 0;
    int y_dimension = 0;

}; 
#endif // !SHAPE_H



