#ifndef ROW_H
#define ROW_H
#include "pch.h"
#include "Node.h"
#include "Segment.h"

class Row
{

public:
    int seed;
    int length;
    std::shared_ptr<std::vector<std::vector<std::vector<short>>>>
        setOfPartitionSets;
    std::vector<char> bits;
    std::vector<Segment> segments;
    std::vector<std::shared_ptr<Node>> nodes;
    Row(int seed, int length,
        std::shared_ptr<std::vector<std::vector<std::vector<short>>>>
            setOfPartitionSets);
    ~Row();
    void generatebits();
    bool isPartitionless;
    void createSegments();
    void createNodes();
    bool formsPool(Row &other);
    bool isPartiallyContiguous(Row &other);
    static void mapNodes(Row &top, Row &bottom);
    static std::vector<short> mapRow(std::shared_ptr<Node> topNode,
                                     Row &bottomRow);
};
#endif // !ROW_H
