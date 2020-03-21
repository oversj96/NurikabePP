#ifndef PATTERN_H
#define PATTERN_H
#include "pch.h"

class Pattern
{
public:
    const static std::vector<std::vector<int>> points;
    unsigned int rowLength;
    unsigned int rowCount;
    bool seedOutOfBounds;
    bool isLegal;
    bool hasPool;
    std::vector<std::vector<char>> rows;
    std::vector<int> firstWaterPoint;
    std::vector<std::vector<bool>> travelMap;
    unsigned int waterCount;
    unsigned int waterReachable;
    unsigned long long patternSeed;

    Pattern(unsigned int rowLength, std::vector<std::vector<char>> rows, bool test);
    Pattern(unsigned int rowLength, unsigned int rowCount, unsigned long long seed);
    ~Pattern();
    std::string patternString();
    bool canMove(int dim, int pos, int dir);
    void testPattern(int posX, int posY, int call);
    void countWater();
    void transpose();
    static unsigned long long determinePatternSeed(std::vector<std::vector<char>> &rows);
    static unsigned int rowSeed(std::vector<char> &row);

    static std::vector<char> generateBits(unsigned long long seed, int length);
    void shapeBits(std::vector<char> &binVec);
    bool formsPool(std::vector<char> r1, std::vector<char> r2);
    static boost::multiprecision::cpp_int bigpow(boost::multiprecision::cpp_int lhs, boost::multiprecision::cpp_int rhs);
};
#endif // !PATTERN_H
