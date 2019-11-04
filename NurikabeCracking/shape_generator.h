#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H
#include "pch.h"
#include "shape.h"

class shape_generator
{
public:
    std::unordered_set<int> umap;
    std::vector<shape> shapes;
    shape_generator(int max_order);
    ~shape_generator();
    void run(std::vector<std::vector<int>> candidates, std::vector<std::vector<bool>> map, int tiles, int x_dim, int y_dim, int id);
    bool forms_pool(const std::vector<std::vector<bool>> &map, std::vector<int> point);
    bool in_bounds(const std::vector<std::vector<bool>> &map, std::vector<int> point);
    std::vector<std::vector<int>> determine_candidates(const std::vector<std::vector<bool>> &map,
        std::vector<int> point, std::vector<std::vector<int>> candidates);
    int distinct_count() const;
private:
    int max_dimension = 0;
    int shape_count = 0;
};

#endif // !SHAPE_GENERATOR_H


