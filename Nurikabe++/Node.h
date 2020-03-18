#ifndef NODE_H
#define NODE_H
#include "pch.h"
#include "Segment.h"

class Node
{
public:
    static unsigned long long debugCount;
    static unsigned long long recursionCalls;
    static unsigned int currentSeed;
    int parentLength;
    unsigned int parentSeed;
    std::vector<Segment> segments;
    bool isStart;
    std::vector<short> partitionSet;
    std::vector<bool> depthMemoized;
    std::vector<boost::multiprecision::cpp_int> memos;
    std::vector<std::shared_ptr<Node>> inodes;
    std::vector<std::shared_ptr<Node>> leafNodes;
    short maxElement;
    std::set<short> uniqueElements;
    bool isPartitionless;
    Node(std::vector<Segment> segments, int parentLength, unsigned int parentSeed,
         bool isStart, bool partitionless, std::vector<short> partitionSet);
    ~Node();
    bool isInode(std::shared_ptr<Node> other);
    bool isLeafNode(std::shared_ptr<Node> other);
    bool connectsAllPartitions(std::shared_ptr<Node> other);
    boost::multiprecision::cpp_int traverseAndMemoize(bool water, bool end,
                                                      int depth, int baseDepth);
    unsigned long long traverse(std::vector<std::vector<char>> puzzleRows,
                                bool water, bool end, int depth, int base_depth);
    void printPuzzle(unsigned long long patCount,
                     std::vector<std::vector<char>> puzzleRows);
    static std::vector<char> generatebits(int seed, int length);
};

#endif // !NODE_H
