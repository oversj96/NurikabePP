#ifndef ROW_BRAIN_H
#define ROW_BRAIN_H
#include "pch.h"
#include "Row.h"
/*
Develop a class that will, given the length of the rows, determine what rows
are allowed to succeed a previous row. Rows will have varying segment combinations,
and all successor rows for a single segment combo must be generated.

Rows should be identifiable by the seed that generates the binary string. From there, the
count of nonconnected segments should be used as a sub-id for all rows that are
generated from a seed. Each row should have two vectors to choose from: a vector that allows
the next generation of the next legal puzzle, and a vector that ensures that there is only
one segment of water within the entire puzzle.
*/
class RowBrain
{
public:
    RowBrain(int length);
    ~RowBrain();
    std::vector<std::vector<bool>> triviallyLegalRows;
    std::vector<std::vector<std::shared_ptr<Row>>> successorRows;
    std::vector<std::shared_ptr<Row>> rows;
    void countPatterns(std::vector<std::shared_ptr<Row>> puzzle, int rowNum);
    void determineSuccessorRows();
    int patterns = 0;
    size_t maxRowPatterns;
private:
    int length;
    bool formsPool(std::shared_ptr<Row> r1, std::shared_ptr<Row> r2);
    bool isContiguous(std::shared_ptr<Row> r1, std::shared_ptr<Row> r2);
};
#endif // !ROW_BRAIN_H



