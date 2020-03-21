#include "pch.h"
#include "Row.h"

std::unique_ptr<std::vector<std::vector<std::shared_ptr<std::vector<char>>>>>
    Row::setOfPartitionSets;

Row::Row(int seed, int length)
{
    this->seed = seed;
    this->length = length;
    this->generatebits();
    this->createSegments();
    this->isPartitionless = (this->segments.size() == 0);
    this->createNodes();
}

Row::~Row() {}

void Row::generatebits()
{
    int sCopy = this->seed;
    for (int i = this->length - 1; i >= 0; --i)
    {
        int bitVal = std::pow(2, i);
        if (sCopy >= bitVal)
        {
            sCopy -= bitVal;
            this->bits.push_back(1);
        }
        else
        {
            this->bits.push_back(0);
        }
    }
}

void Row::createSegments()
{
    std::vector<int> columns;

    for (int i = 0; i < this->bits.size(); ++i)
    {
        if (this->bits[i] == 0)
        {
            columns.push_back(i);
            if (i == this->bits.size() - 1)
            {
                this->segments.push_back(Segment(columns));
            }
        }
        else if (this->bits[i] == 1 && columns.size() > 0)
        {
            this->segments.push_back(Segment(columns));
            columns.clear();
        }
    }
}

void Row::createNodes()
{
    if (!this->isPartitionless)
    {
        std::vector<char>::iterator it;
        auto partSets = Row::setOfPartitionSets->at(this->segments.size());
        for (int i = 0; i < partSets.size(); ++i)
        {
            char maxElement =
                *std::max_element(partSets[i]->begin(), partSets[i]->end());

            if (maxElement == partSets[i]->size() - 1)
            {
                this->nodes.push_back(
                    std::make_shared<Node>(Node(this->segments, this->length,
                                                this->seed, true, false, partSets[i])));
            }
            else
            {
                this->nodes.push_back(std::make_shared<Node>(
                    Node(this->segments, this->length, this->seed, false, false,
                         partSets[i])));
            }
        }
    }
    else
    {
        this->nodes.push_back(
            std::make_shared<Node>(Node(this->segments, this->length, this->seed,
                                        true, true, std::make_shared<std::vector<char>>(std::vector<char>{}))));
    }
}

bool Row::isPartiallyContiguous(Row &other)
{
    if (this->isPartitionless || other.isPartitionless)
    {
        return true;
    }
    for (int i = 0; i < this->bits.size(); ++i)
    {
        if (this->bits[i] == 0 && other.bits[i] == 0)
        {
            return true;
        }
    }
    return false;
}

bool Row::formsPool(Row &other)
{
    for (int i = 0; i < this->bits.size() - 1; ++i)
    {
        if (this->bits[i] == 0)
        {
            if ((this->bits[i] == 0 && other.bits[i] == 0) &&
                (this->bits[i + 1] == 0 && other.bits[i + 1] == 0))
            {
                return true;
            }
        }
    }
    return false;
}

void Row::mapNodes(Row &top, Row &bottom)
{
    for (int i = 0; i < top.nodes.size(); ++i)
    {
        std::vector<char> legalSet = Row::mapRow(top.nodes[i], bottom);

        for (int j = 0; j < bottom.nodes.size(); ++j)
        {
            if (legalSet == *bottom.nodes[j]->partitionSet)
            {
                if (top.nodes[i]->isInode(bottom.nodes[j]))
                {
                    top.nodes[i]->inodes.push_back(bottom.nodes[j]);
                }

                if (top.nodes[i]->isLeafNode(bottom.nodes[j]))
                {
                    top.nodes[i]->leafNodes.push_back(bottom.nodes[j]);
                }

                break;
            }
        }
    }
}

std::vector<char> Row::mapRow(std::shared_ptr<Node> topNode, Row &bottomRow)
{
    if (bottomRow.segments.size() == 0)
    {
        return std::vector<char>{};
    }

    else if (topNode->segments.size() == 0)
    {
        std::vector<char> set;
        for (int i = 0; i < bottomRow.segments.size(); ++i)
        {
            set.push_back(i);
        }
        return set;
    }

    else
    {
        std::vector<std::vector<char>> connectiveList;
        for (char i = 0; i < (char)bottomRow.segments.size(); ++i)
        {
            std::vector<char> connections;
            for (char j = 0; j < (char)topNode->partitionSet->size(); ++j)
            {
                if (bottomRow.segments[i].connects(topNode->segments[j]))
                {
                    connections.push_back(topNode->partitionSet->at(j));
                }
            }
            connectiveList.push_back(connections);
            connections.clear();
        }

        std::vector<char> minVals;
        for (int i = 0; i < connectiveList.size(); ++i)
        {
            if (connectiveList[i].size() != 0)
            {
                minVals.push_back(*std::min_element(connectiveList[i].begin(),
                                                    connectiveList[i].end()));
            }
            else
            {
                minVals.push_back(-1);
            }
        }

        for (int i = 0; i < minVals.size(); ++i)
        {
            if (minVals[i] != NULL && minVals[i] > 0)
            {
                int j = 0;
                while (j < connectiveList.size())
                {
                    if (j != i)
                    {
                        if (std::find(connectiveList[j].begin(), connectiveList[j].end(),
                                      minVals[i]) != connectiveList[j].end())
                        {
                            char contextMin = *std::min_element(connectiveList[j].begin(),
                                                                connectiveList[j].end());
                            if (contextMin != minVals[i])
                            {
                                minVals[i] = contextMin;
                                j = 0;
                            }
                            else
                            {
                                ++j;
                            }
                        }
                        else
                        {
                            ++j;
                        }
                    }
                    else
                    {
                        ++j;
                    }
                }
            }
        }

        for (int i = 0; i < minVals.size(); ++i)
        {
            if (minVals[i] == -1)
            {
                minVals[i] = *std::max_element(minVals.begin(), minVals.end()) + 1;
            }
        }

        std::vector<char> seen;
        std::vector<std::vector<char>> locsOfSeen;
        for (int i = 0; i < minVals.size(); ++i)
        {
            if (std::find(seen.begin(), seen.end(), minVals[i]) == seen.end())
            {
                seen.push_back(minVals[i]);
                std::vector<char> locs;
                for (int j = 0; j < minVals.size(); ++j)
                {
                    if (minVals[j] == minVals[i])
                    {
                        locs.push_back(j);
                    }
                }
                locsOfSeen.push_back(locs);
                locs.clear();
            }
        }

        for (int i = 0; i < locsOfSeen.size(); ++i)
        {
            for (auto &loc : locsOfSeen[i])
            {
                minVals[loc] = i;
            }
        }

        return minVals;
    }
}
