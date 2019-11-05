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
    std::vector<std::vector<int>> pos;

    for (int i = 0; i < max_order; i++) {
        for (int j = 0; j < max_order; j++) {
            pos.push_back({ i, j });
        }
    }
    std::seed_seq seq{ 1,2,3,4,5,6,7,8,9,10 };
    std::vector<std::uint32_t> seeds(max_order*max_order);
    seq.generate(seeds.begin(), seeds.end());
    int count = 0;
    for (int i = 0; i < max_order; i++) {
        std::vector<int> row;
        for (int j = 0; j < max_order; j++) {            
            row.push_back(seeds[count]);
            count++;
        }
        value_map.push_back(row);
    }

    /*std::vector<int> initial{ 0, 0 };
    std::vector<std::vector<int>> pos;
    pos.push_back(initial);*/
    int x = 0;
    int y = 0;
    shape_generator::run(pos, map, 0, x, y, 0);
}


shape_generator::~shape_generator()
{
}

void shape_generator::run(std::vector<std::vector<int>> candidates, std::vector<std::vector<bool>> map,
    int tiles, int x_dim, int y_dim, int id) {
    // Increment the tile count.
    tiles++;
    // Loop through the list of candidates available to this specific shape.
    for (size_t i = 0; i < candidates.size(); i++) {
        // For readability, assign the x and y value of the candidate to a point vector.
        std::vector<int> point{ candidates[i][0], candidates[i][1] };
        // Erase the chosen point from the list of candidates.
        // Generate the new shape ID based on the previous ID and the new point.
        int new_id = id + value_map[point[0]][point[1]];
        // Determine if the id has been used before.
        if (umap.find(new_id) == umap.end()) {
            // Set the point on the map to true to indicate water.
            auto n_map = map;
            n_map[point[0]][point[1]] = true;
            // Measure the shapes dimensions.
            /*if (point[0] > x_dim) {
                x_dim = point[0];
            }
            if (point[1] > y_dim) {
                y_dim = point[1];
            }*/
            // Construct a new shape 's' with these parameters.
            shape s(tiles, n_map, x_dim, y_dim, new_id);
            // Place the shape onto the end of the shapes list.
            shapes.push_back(s);
            // Superfluous variable to keep track of the count of shapes.
            shape_generator::shape_count++;
            // Hash the new id and add it to the unordered_set object.
            // This prevents multiple inclusion and searching a hash table is amortized O(1).
            shape_generator::umap.insert(new_id);
            // Copy the candidates that remain and update the list, removing any potentially
            // invalid points and adding new ones.
            // Passes cand_copy by reference.
            std::vector<std::vector<int>> n_cands = shape_generator::determine_candidates(n_map, point);

            /*std::cout << "Matrix " << std::to_string(id) << std::endl;
            for (auto const &cand : cand_copy) {
                std::cout << std::to_string(cand[0]) << "," << std::to_string(cand[1]);
                std::cout << std::endl;
            }*/
            // recurse!
            shape_generator::run(n_cands, n_map, tiles, x_dim, y_dim, new_id);
        }
        else {
            std::cout << std::to_string(new_id) << " already exists and tile count is: " << std::to_string(tiles) << std::endl;
        }
    }
}

std::vector<std::vector<int>> shape_generator::determine_candidates(const std::vector<std::vector<bool>> &map,
    std::vector<int> point) {
    std::vector<int> top{ point[0] - 1, point[1] };     // top tile
    std::vector<int> bot{ point[0] + 1, point[1] };     // bottom tile
    std::vector<int> lft{ point[0],     point[1] - 1 }; // left tile
    std::vector<int> rht{ point[0],     point[1] + 1 }; // right tile
    std::vector<std::vector<int>> directions;
    directions.push_back(top);
    directions.push_back(bot);
    directions.push_back(lft);
    directions.push_back(rht);
    std::vector<std::vector<int>> candidates;
    for (size_t i = 0; i < directions.size(); i++) {
        if (in_bounds(map, directions[i])) {
            if (!forms_pool(map, directions[i]) && !map[directions[i][0]][directions[i][1]]) {
                candidates.push_back(directions[i]);
            }            
        }      
    }
    return candidates;
}

bool shape_generator::forms_pool(const std::vector<std::vector<bool>> &map, std::vector<int> point) {
    std::vector<int> tlf{ point[0] - 1, point[1] - 1 }; // top left tile
    std::vector<int> tpr{ point[0] - 1, point[1] + 1 }; // top right tile
    std::vector<int> btl{ point[0] + 1, point[1] - 1 }; // bottom left tile
    std::vector<int> btr{ point[0] + 1, point[1] + 1 }; // bottom right tile
    std::vector<int> top{ point[0] - 1, point[1] };     // top tile
    std::vector<int> bot{ point[0] + 1, point[1] };     // bottom tile
    std::vector<int> lft{ point[0],     point[1] - 1 }; // left tile
    std::vector<int> rht{ point[0],     point[1] + 1 };  // right tile
    std::vector<std::vector<int>> checking_positions;
    if (in_bounds(map, tlf)) {
        checking_positions.push_back(tlf);
    }
    if (in_bounds(map, tpr)) {
        checking_positions.push_back(tpr);
    }
    if (in_bounds(map, btl)) {
        checking_positions.push_back(btl);
    }
    if (in_bounds(map, btr)) {
        checking_positions.push_back(btr);
    }
    for (int i = 0; i < checking_positions.size(); i++) {
        if (checking_positions[i] == tlf && map[tlf[0]][tlf[1]]) {
            if (map[lft[0]][lft[1]] && map[top[0]][top[1]]) {
                return true;
            }          
        }
        else if (checking_positions[i] == tpr && map[tpr[0]][tpr[1]]) {
            if (map[rht[0]][rht[1]] && map[top[0]][top[1]]) {
                return true;
            }
        }
        else if (checking_positions[i] == btl && map[btl[0]][btl[1]]) {
            if (map[lft[0]][lft[1]] && map[bot[0]][bot[1]]) {
                return true;
            }
        }
        else if (checking_positions[i] == btr && map[btr[0]][btr[1]]) {
            if (map[rht[0]][rht[1]] && map[bot[0]][bot[1]]) {
                return true;
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