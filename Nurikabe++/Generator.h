#ifndef GENERATOR_H
#define GENERATOR_H
#include "pch.h"
#include "Row.h"
#include "Node.h"

class Generator
{
public:
    static boost::mutex mtx_;
    static std::vector<double> threadReports;
    static double threadVal;
    int dimColumn;
    int dimRow;
    bool fastCount;
    std::vector<boost::multiprecision::cpp_int> rowCounts;
    boost::multiprecision::cpp_int patternsPossible;
    boost::multiprecision::cpp_int goodPatterns;
    std::unique_ptr<std::vector<std::vector<std::shared_ptr<std::vector<char>>>>>
        partitionSets;
    std::vector<Row> rows;

    int nodeCount; 
    Generator(int depth, int length, bool fastCount);
    ~Generator();
    void genSetOfPartSets();
    
    void restrictedGrowthStrings(int setLength);
    void buildTree(int threadcount, int id);
    void updateAuxRow(std::vector<char> &mainRow,
                      std::vector<char> &auxRow);
    void setToZero(std::vector<char> &mainRow, int index);
    void countPatterns();
    void countPatternsMulti(int threadCount, int id);
    static void insert_commas(std::string& num);

    static void writeSeedsToFile(std::vector<unsigned long long> &seeds, std::string fileName);
    void rowDetails();
    void sumRowMemos();

    static void compareFiles(std::string fileName1, std::string fileName2);

    template <typename T>
    static T mean(const std::vector<T>& v);
};
#endif // !GENERATOR_H
