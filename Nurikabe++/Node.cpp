#include "pch.h"
#include "Node.h"
#include "Pattern.h"

unsigned long long Node::debugCount = 0;
std::vector<unsigned long long> Node::patternSeeds;
boost::mutex Node::mtx_;

Node::Node(std::vector<Segment> segments, int parentLength,
           unsigned int parentSeed, bool isStart, bool partitionless,
           std::shared_ptr<std::vector<char>> partitionSet)
{
    this->parentLength = parentLength;
    this->segments = segments;
    this->parentSeed = parentSeed;
    this->isStart = isStart;
    this->isPartitionless = partitionless;
    this->partitionSet = partitionSet;
    if (this->partitionSet->size() != 0)
    {
        this->maxElement =
            *std::max_element(partitionSet->begin(), partitionSet->end());
    }
    this->uniqueElements =
        std::set<char>(partitionSet->begin(), partitionSet->end());
}

Node::~Node() {}

bool Node::isInode(std::shared_ptr<Node> other)
{
    // If empty set.
    if (this->isPartitionless)
    {
        if (other->isPartitionless)
        {
            return true;
        }
        else if (other->maxElement == other->partitionSet->size() - 1)
        {
            return true;
        }
    }
    // If this is a set of one element.
    else if (this->uniqueElements.size() == 1)
    {
        if (other->isPartitionless)
        {
            return true;
        }
        else
        {
            return this->connectsAllPartitions(other);
        }
    }
    // If this is a multiple element set.
    else if (this->uniqueElements.size() > 1)
    {
        if (other->isPartitionless)
        {
            return false;
        }
        else
        {
            return this->connectsAllPartitions(other);
        }
    }
    else
    {
        return false;
    }
}

bool Node::isLeafNode(std::shared_ptr<Node> other)
{
    // If empty set.
    if (this->isPartitionless)
    {
        if (other->isPartitionless)
        {
            return true;
        }
        else if (other->segments.size() == 1)
        {
            return true;
        }
    }
    else if (this->uniqueElements.size() == 1)
    {
        if (other->isPartitionless)
        {
            return true;
        }
        else
        {
            if (other->uniqueElements.size() == 1)
            {
                return this->connectsAllPartitions(other);
            }
        }
    }
    else if (this->uniqueElements.size() > 1)
    {
        if (other->uniqueElements.size() == 1)
        {
            return this->connectsAllPartitions(other);
        }
    }
    return false;
}

bool Node::connectsAllPartitions(std::shared_ptr<Node> other)
{
    std::vector<bool> connections;
    for (int i = 0; i < this->partitionSet->size(); ++i)
    {
        connections.push_back(false);
    }

    for (int i = 0; i < this->partitionSet->size(); ++i)
    {
        for (int j = 0; j < other->partitionSet->size(); ++j)
        {
            if (!connections[i] && this->segments[i].connects(other->segments[j]))
            {
                connections[i] = true;
            }
        }
    }

    for (int i = 0; i < connections.size(); ++i)
    {
        if (connections[i])
        {
            char key = this->partitionSet->at(i);
            for (int j = 0; j < connections.size(); ++j)
            {
                if (this->partitionSet->at(j) == key)
                {
                    connections[j] = true;
                }
            }
        }
    }
    return std::all_of
    (
        connections.begin(), connections.end(), [](bool v) { return v; }
    );
}

boost::multiprecision::cpp_int
Node::traverseAndMemoize(bool water, bool end, int depth, int base_depth)
{
    if (this->depthMemoized.size() == 0)
    {
        for (int i = 0; i < base_depth; ++i)
        {
            this->depthMemoized.push_back(false);
            this->memos.push_back(0);
        }
    }

    if (depth == 1)
    {
        std::cout << "\rCounting...  "
                  << std::to_string(std::round(
                         this->parentSeed / std::pow(2, this->parentLength) * 100))
                  << " %";
    }

    boost::multiprecision::cpp_int count = 0;

    if (end)
    {
        if (depth == base_depth - 1)
        {
            ++count;
        }
        else
        {
            count += this->traverseAndMemoize(water, end, depth + 1, base_depth);
        }
    }

    else if (!this->depthMemoized[depth - 1])
    {
        if (depth == base_depth - 1)
        {
            count = this->leafNodes.size();
        }

        else if (depth != base_depth - 1)
        {
            if (!water)
            {
                for (int i = 0; i < this->inodes.size(); ++i)
                {
                    if (this->inodes[i]->isStart)
                    {
                        bool hasWater = this->inodes[i]->segments.size() >= 1;
                        count += this->inodes[i]->traverseAndMemoize(hasWater, end,
                                                                     depth + 1, base_depth);
                    }
                }
            }
            else
            {
                for (int i = 0; i < this->inodes.size(); ++i)
                {
                    this->inodes[i]->isPartitionless ? end = true : end = false;
                    count += this->inodes[i]->traverseAndMemoize(water, end, depth + 1,
                                                                 base_depth);
                }
            }
        }
        else
        {
            std::cout << "\n Warning: conditions not met in memoized traverse."
                      << std::endl;
        }

        this->depthMemoized[depth - 1] = true;
        this->memos[depth - 1] = count;
    }
    else
    {
        count = this->memos[depth - 1];
    }
    return count;
}

unsigned long long Node::traverse(std::vector<std::vector<char>> puzzleRows,
                                  bool water, bool end, int depth,
                                  int base_depth)
{

    puzzleRows[depth - 1] =
        Node::generatebits(this->parentSeed, this->parentLength);

    unsigned long long count = 0;

    if (end)
    {
        if (depth == base_depth - 1)
        {
            puzzleRows[depth] =
                Node::generatebits(this->parentSeed, this->parentLength);
            ++count;
            Node::printPuzzle(puzzleRows);
        }
        else
        {
            count += this->traverse(puzzleRows, water, end, depth + 1, base_depth);
        }
    }

    else if (depth == base_depth - 1)
    {
        for (auto &node : this->leafNodes)
        {
            puzzleRows[depth] =
                Node::generatebits(node->parentSeed, node->parentLength);
            Node::printPuzzle(puzzleRows);
            ++count;
        }
    }

    else if (depth != base_depth - 1)
    {
        if (!water)
        {
            for (int i = 0; i < this->inodes.size(); ++i)
            {
                if (this->inodes[i]->isStart)
                {
                    bool hasWater = this->inodes[i]->segments.size() >= 1;
                    count += this->inodes[i]->traverse(puzzleRows, hasWater, end,
                                                       depth + 1, base_depth);
                }
            }
        }
        else
        {
            for (int i = 0; i < this->inodes.size(); ++i)
            {
                this->inodes[i]->isPartitionless ? end = true : end = false;
                count += this->inodes[i]->traverse(puzzleRows, water, end, depth + 1,
                                                   base_depth);
            }
        }
    }
    else
    {
        std::cout << "\n Warning: conditions not met in memoized traverse."
                  << std::endl;
    }
    return count;
}

// Static version for debug purposes only.
std::vector<char> Node::generatebits(int seed, int length)
{
    std::vector<char> bits;
    bits.reserve(length);
    for (int i = length - 1; i >= 0; --i)
    {
        int bitVal = std::pow(2, i);
        if (seed >= bitVal)
        {
            seed -= bitVal;
            bits.push_back(1);
        }
        else
        {
            bits.push_back(0);
        }
    }
    return bits;
}

void Node::printPuzzle(std::vector<std::vector<char>> rows)
{
    int interval = 1;
    Pattern pat(rows[0].size(), rows, false);
    mtx_.lock();
    ++Node::debugCount;
    if (Node::debugCount % interval == 0) {
        Node::patternSeeds.push_back(pat.patternSeed);
        if (!pat.isLegal || pat.patternSeed == 4465420545 || pat.patternSeed == 4465422466) {
            std::cout << pat.patternString();
            std::cin.get();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            if (Node::debugCount % 10000 == 0) {
                std::cout << Node::debugCount << "\n";
                std::cout << pat.patternString() << std::endl;
            }
        }
    }
    mtx_.unlock();
}

template<typename T>
typename std::vector<T>::iterator
Node::insert_sorted(std::vector<T> & vec, T const& item)
{
    return vec.insert
    (
        std::upper_bound(vec.begin(), vec.end(), item),
        item
    );
}