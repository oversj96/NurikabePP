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

std::vector<std::vector<char>> RowBrain::partitionerStrings(int length) {
    int n = length - 1;
    bool incrementable = true;
    std::vector<char> mainRow;
    std::vector<char> auxRow;
    std::vector<std::vector<char>> lexicoStrings;

    // Initialize rows and push back initial set
    for (int i = 0; i < length; ++i) { mainRow.push_back(0); }
    for (int i = 0; i < length - 1; ++i) { auxRow.push_back(1); }   
    lexicoStrings.push_back(mainRow);

    while (incrementable) {
        incrementable = false;
        for (int index = n; index > 0; --index) {
            if (mainRow[index] < n && mainRow[index] < auxRow[index - 1]) {
                incrementable = true;
                ++mainRow[index];
                RowBrain::setToZero(mainRow, index);
                RowBrain::updateAuxRow(mainRow, auxRow);
                lexicoStrings.push_back(mainRow);
                break;
            }
        }
    }
    return lexicoStrings;
}

void RowBrain::updateAuxRow(const std::vector<char> &mainRow, std::vector<char> &auxRow) {
    for (int i = 1; i < mainRow.size(); ++i) {
        auxRow[i - 1] = *std::max_element(mainRow.begin(), mainRow.begin() + i) + 1;
    }
}

void RowBrain::setToZero(std::vector<char> &mainRow, int index) {
    if (index != mainRow.size() - 1) {
        for (int i = index + 1; i < mainRow.size(); ++i) { mainRow[i] = 0; }
    }
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