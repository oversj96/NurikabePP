#ifndef GENERATOR_H
#define GENERATOR_H
#include "pch.h"
#include "Row.h"

class Generator
{
public:
    static boost::mutex mtx_;
    static std::vector<double> threadReports;
    static double threadMax;
    int dimColumn;
    int dimRow;
    bool fastCount;
    boost::multiprecision::cpp_int patternsPossible;
    boost::multiprecision::cpp_int goodPatterns;
    std::shared_ptr<std::vector<std::vector<std::vector<short>>>> partitionSets;
    std::vector<Row> rows;

    int nodeCount;
    Generator(int depth, int length, bool fastCount);
    ~Generator();
    void genSetOfPartSets();
    void printProgress();
    void restrictedGrowthStrings(int setLength);
    void buildTree(int threadcount, int id);
    void updateAuxRow(const std::vector<short> &mainRow,
                      std::vector<short> &auxRow);
    void setToZero(std::vector<short> &mainRow, int index);
    void countPatterns();
};
#endif // !GENERATOR_H
