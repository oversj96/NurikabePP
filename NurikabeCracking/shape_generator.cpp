#include "pch.h"
#include "shape_generator.h"


shape_generator::shape_generator(int max_order) {
    shape_generator::max_dimension = max_order;
    shape_generator::shape_count = 0;
    std::vector<std::vector<bool>> map;
    map.reserve(max_order);
    std::vector<bool> row;
    row.reserve(max_order);
    for (int i = 0; i < max_order; i++) {
        row.clear();
        for (int j = 0; j < max_order; j++) {
            row.push_back(false);
        }
        map.push_back(row);
    }
    std::vector<int> initial{ 0, 0 };
    std::vector<std::vector<int>> pos;
    pos.push_back(initial);
    int x = 0;
    int y = 0;
    shape_generator::run(pos, map, 0, x, y, 0);
}


shape_generator::~shape_generator()
{
}

void shape_generator::run(std::vector<std::vector<int>> candidates, std::vector<std::vector<bool>> map,
    int tiles, int x_dim, int y_dim, int id) {
    tiles++;
    for (int i = 0; i < candidates.size(); i++) {
        std::vector<int> point{ candidates[i][0], candidates[i][1] };
        int new_id = id + point[0] + point[1];
        map[point[0]][point[1]] = true;
        if (point[0] > x_dim) {
            x_dim = point[0];
        }
        if (point[1] > y_dim) {
            y_dim = point[1];
        }
        shape s(tiles, map, x_dim, y_dim, new_id);
        if (umap.find(new_id) == umap.end()) {
            shapes.push_back(s);
            shape_generator::shape_count++;
            shape_generator::umap.insert(new_id);
            std::vector<std::vector<int>> cand_copy = candidates;
            cand_copy.erase(cand_copy.begin() + i);
            auto n_candidates = shape_generator::determine_candidates(map, point, cand_copy);
            shape_generator::run(n_candidates, map, tiles, x_dim, y_dim, new_id);
        }
    }
}

std::vector<std::vector<int>> shape_generator::determine_candidates(const std::vector<std::vector<bool>> &map,
    std::vector<int> point, std::vector<std::vector<int>> candidates) {
    std::vector<int> top{ point[0] - 1, point[1] };     // top tile
    std::vector<int> bot{ point[0] + 1, point[1] };     // bottom tile
    std::vector<int> lft{ point[0],     point[1] - 1 }; // left tile
    std::vector<int> rht{ point[0],     point[1] + 1 }; // right tile
    std::vector<std::vector<int>> directions;
    directions.push_back(top);
    directions.push_back(bot);
    directions.push_back(lft);
    directions.push_back(rht);
    for (auto &direction : directions) {
        if (in_bounds(map, direction) && !forms_pool(map, direction)) {
            candidates.push_back(direction);
        }
    }
    return candidates;
    
}

bool shape_generator::forms_pool(const std::vector<std::vector<bool>> &map, std::vector<int> point) {
    std::vector<std::vector<char>> surrounding_tiles;
    /*
    This isn't very elegant (in fact, this is an antipattern) but I believe it's the most
    straight forward way of getting the eight surrounding tiles, and to test if each are
    out of bounds.  Will likely rework later if this program proves to be faster.
    */
    std::vector<int> cen{ point[0],     point[1] };     // center tile, had to be recopied
    std::vector<int> top{ point[0] - 1, point[1] };     // top tile
    std::vector<int> bot{ point[0] + 1, point[1] };     // bottom tile
    std::vector<int> lft{ point[0],     point[1] - 1 }; // left tile
    std::vector<int> rht{ point[0],     point[1] + 1 };  // right tile
    std::vector<int> tlf{ point[0] - 1, point[1] - 1 }; // top left tile
    std::vector<int> tpr{ point[0] - 1, point[1] + 1 };  // top right tile
    std::vector<int> btl{ point[0] + 1, point[1] - 1 }; // bottom left tile
    std::vector<int> btr{ point[0] + 1, point[1] + 1 }; // bottom right tile
    std::vector<std::vector<int>> surrounding_bits;
    surrounding_bits.reserve(9);
    surrounding_bits.push_back(tlf);
    surrounding_bits.push_back(top);
    surrounding_bits.push_back(tpr);
    surrounding_bits.push_back(lft);
    surrounding_bits.push_back(cen);
    surrounding_bits.push_back(rht);
    surrounding_bits.push_back(btl);
    surrounding_bits.push_back(bot);
    surrounding_bits.push_back(btr);

    std::vector<char> row;
    row.reserve(3);
    for (auto &this_point : surrounding_bits) {
        if (in_bounds(map, this_point)) {
            if (!map[this_point[0]][this_point[1]]) {
                row.push_back(1);
            }
            else {
                row.push_back(0);
            }
        }
        else {
            row.push_back(1);
        }
        if (row.size() == 3) {
            surrounding_tiles.push_back(row);
            row.clear();
        }
    }

    int length = surrounding_tiles[0].size();

    for (int i = 0; i < surrounding_tiles.size() - 1; i++) {
        char prev_res = 1;
        for (int j = 0; j < surrounding_tiles[1].size(); j++) {
            char res = (surrounding_tiles[i][j] | surrounding_tiles[i + 1][j]);

            if (prev_res == 0 && res == 0) {
                return true;
            }
            else {
                prev_res = res;
            }
        }
    }
    return false;
}

bool shape_generator::in_bounds(const std::vector<std::vector<bool>> &map, std::vector<int> point) {
    return (point[0] < map.size() && point[0] >= 0
        && point[1] < map.size() && point[1] >= 0);
}

int shape_generator::distinct_count() const { return shape_generator::shape_count; }

//namespace std
//{
//    template<typename T>
//    struct hash<vector<T>>
//    {
//        typedef vector<T> argument_type;
//        typedef std::size_t result_type;
//        result_type operator()(argument_type const& in) const
//        {
//            size_t size = in.size();
//            size_t seed = 0;
//            for (size_t i = 0; i < size; i++)
//                //Combine the hash of the current vector with the hashes of the previous ones
//                boost::hash_combine(seed, in[i]);
//            return seed;
//        }
//    };
//}
//
////using boost::hash_combine
//template <class T>
//inline void hash_combine(std::size_t& seed, T const& v)
//{
//    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//}