#include "pch.h"
#include "RowBrain.h"


RowBrain::RowBrain(int length)
{
    RowBrain::length = length;
    RowBrain::maxRowPatterns = std::pow(2, RowBrain::length);   
    RowBrain::rows.reserve(RowBrain::maxRowPatterns); 
    for (size_t i = 0; i < RowBrain::maxRowPatterns; ++i) {
        RowBrain::rows.push_back(
            std::make_unique<Row>(
                Row(i, length, Segment::makeSegments(Row::generateBits(i, length)))
            )
        );
    }

    std::vector<bool> row;
    RowBrain::triviallyLegalRows.reserve(RowBrain::maxRowPatterns);
    row.reserve(RowBrain::maxRowPatterns);
    for (size_t i = 0; i < RowBrain::maxRowPatterns; ++i) {
        row.clear();
        for (size_t j = 0; j < RowBrain::maxRowPatterns; ++j) {
            row.push_back(!formsPool(rows[i], rows[j]) && isContiguous(rows[i], rows[j]));
        }
        RowBrain::triviallyLegalRows.push_back(row);
    }
}


RowBrain::~RowBrain()
{
}

bool RowBrain::formsPool(std::shared_ptr<Row> r1, std::shared_ptr<Row> r2) {
    for (size_t i = 0; i < r1->bits.size() - 1; ++i) {
        if (r1->bits[i] == 0) {
            if ((r1->bits[i] | r2->bits[i] == 0) && (r1->bits[i + 1] | r2->bits[i + 1] == 0)) {
                return true;
            }
        }      
    }
    return false;
}

bool RowBrain::isContiguous(std::shared_ptr<Row> r1, std::shared_ptr<Row> r2) {
    if (r2->seed == RowBrain::maxRowPatterns - 1) {
        return true;
    }
    else {
        for (size_t i = 0; i < r1->bits.size(); ++i) {
            if (r1->bits[i] | r2->bits[i] == 0) {
                return true;
            }
        }
        return false;
    }
}

void RowBrain::determineSuccessorRows() {
    for (size_t i = 0; i < RowBrain::rows.size(); ++i) {

    }
}

void RowBrain::countPatterns(std::vector<std::shared_ptr<Row>> puzzle, int rowNum) {
    ++RowBrain::patterns;
    if (puzzle.size() < RowBrain::length) {
        for (size_t i = std::pow(2, length); i > 0; --i) {
            puzzle[rowNum] = RowBrain::rows[i];
        }
    }
    else {
        
    }

}