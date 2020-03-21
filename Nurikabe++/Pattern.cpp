#include "pch.h"
#include "Pattern.h"

const std::vector<std::vector<int>> Pattern::points = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

Pattern::Pattern(unsigned int rowLength, std::vector<std::vector<char>> rows, bool test)
{
    this->hasPool = false;
    this->rowLength = rowLength;
    this->rows = rows;

    if (this->rows.size() > rowLength) {
        this->transpose();
    }

    this->patternSeed = Pattern::determinePatternSeed(this->rows);
    this->firstWaterPoint = std::vector<int>{ -1, -1 };

    std::vector<bool> row;
    for (size_t i = 0; i < this->rowLength; ++i) {
        row.push_back(true);
    }
    for (size_t i = 0; i < this->rows.size(); ++i) {
        this->travelMap.push_back(row);
    }

    this->waterCount = 0;
    this->countWater();
    this->waterReachable = 0;
    this->isLegal = true;
    if (this->waterCount > 0 && test) {
        this->testPattern(this->firstWaterPoint[0], this->firstWaterPoint[1], 1);
    }
}

Pattern::Pattern(unsigned int rowLength, unsigned int rowCount, unsigned long long seed) { 
    this->patternSeed = seed;
    this->firstWaterPoint = std::vector<int>{ -1, -1 };
    if (seed < std::pow(2, rowLength * rowCount)) {
        this->seedOutOfBounds = false;
        this->rowLength = rowLength;
        this->rowCount = rowCount;  
        std::vector<char> binVec = Pattern::generateBits(this->patternSeed, this->rowLength*this->rowCount);
        this->shapeBits(binVec);
        std::vector<bool> row;
        for (size_t i = 0; i < this->rowLength; ++i) {
            row.push_back(true);
        }
        for (size_t i = 0; i < this->rows.size(); ++i) {
            this->travelMap.push_back(row);
        }

        this->waterCount = 0;
        this->countWater();
        this->waterReachable = 0;

        this->hasPool = false;
        for (int i = 0; i < this->rows.size() - 1; ++i) {
            if (this->formsPool(this->rows[i], this->rows[i + 1])) {
                this->hasPool = true;
                break;
            }
        }

        this->isLegal = true;
        if (this->waterCount > 0) {
            this->testPattern(this->firstWaterPoint[0], this->firstWaterPoint[1], 1);
        }
    }
    else {
        this->seedOutOfBounds = true;
        std::cerr << "\nError: Seed was too large to accurately display for this board size." << std::endl;
    }
}

Pattern::~Pattern()
{
}

std::string Pattern::patternString() {
    std::stringstream output;
    output << "\n";
    output << "Pattern Seed: " << this->patternSeed << "\n";
    for (auto &row : this->rows) {
        for (auto &bit : row) {
            output << std::to_string(bit) << " ";
        }
        output << "   " << Pattern::rowSeed(row) << std::endl;
    }
    return output.str();
}


bool Pattern::canMove(int dim, int pos, int dir) {
    if (dim == 0) {
        return pos + dir <= this->rows.size() - 1
            && pos + dir >= 0;
    }
    else {
        return pos + dir <= this->rowLength - 1
            && pos + dir >= 0;
    }

}

void Pattern::testPattern(int posX, int posY, int call) {
    this->travelMap[posX][posY] = false;
    ++this->waterReachable;
    for (const auto &point : Pattern::points) {
        int x = point[0];
        int y = point[1];
        if (this->canMove(0, posX, x)
            && this->canMove(1, posY, y)
            && this->travelMap[posX + x][posY + y]) {
            this->testPattern(posX + x, posY + y, call + 1);
        }
    }
    if (call == 1) {
        this->isLegal = this->waterReachable == this->waterCount && !this->hasPool;
    }
}

void Pattern::countWater() {
    for (size_t i = 0; i < this->rows.size(); ++i) {
        for (size_t j = 0; j < this->rows[i].size(); ++j) {
            if (this->rows[i][j] == 0) {
                if (this->firstWaterPoint == std::vector<int>{-1, -1}) {
                    this->firstWaterPoint = std::vector<int>{ (int)i, (int)j };
                }
                ++this->waterCount;
            }
            else {
                this->travelMap[i][j] = false;
            }
        }
    }
}

unsigned long long
Pattern::determinePatternSeed(std::vector<std::vector<char>> &rows) {
    using bignum = boost::multiprecision::cpp_int;
    unsigned long long seed = 0;
    std::vector<char> binary;
    binary.reserve(rows.size() * rows[0].size());

    // Flatten 2D Vector.
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            binary.push_back(rows[i][j]);
        }
    }

    for (int i = binary.size() - 1; i >= 0; --i) {
        if (binary[i] == 1) {
            seed += std::pow(2, i);
        }
    }

    return seed;
}

unsigned int Pattern::rowSeed(std::vector<char> &row) {
    unsigned int seed = 0;
    for (int i = row.size() - 1; i >= 0; --i) {
        if (row[i] == 1) {
            seed += std::pow(2, i);
        }
    }
    return seed;
}

boost::multiprecision::cpp_int
Pattern::bigpow(boost::multiprecision::cpp_int lhs, boost::multiprecision::cpp_int rhs) {
    if (rhs > 0) {
        return lhs * Pattern::bigpow(lhs, rhs - 1);
    }
    else {
        return lhs;
    }
}

void Pattern::transpose() {
    int xDim = this->rowLength;
    int yDim = this->rows.size();
    std::vector<char> xRow;
    std::vector<std::vector<char>> rows;
    xRow.reserve(yDim);
    rows.reserve(xDim);

    for (int i = 0; i < this->rows[0].size(); ++i) {
        for (int j = 0; j < this->rows.size(); ++j) {
            xRow.push_back(this->rows[j][i]);
        }
        rows.push_back(xRow);
        xRow.clear();
    }

    this->rows = rows;
    this->rowLength = yDim;
}

std::vector<char> Pattern::generateBits(unsigned long long seed, int length)
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

void Pattern::shapeBits(std::vector<char> &binVec) {
    std::vector<char> row;
    unsigned int cursor = 0;
    for (unsigned int i = 0; i < this->rowCount; ++i) {
        for (unsigned int j = 0; j < this->rowLength; ++j) {
            row.push_back(binVec[cursor]);
            ++cursor;
        }
        this->rows.push_back(row);
        row.clear();
    }
}

bool Pattern::formsPool(std::vector<char> r1, std::vector<char> r2) {
    for (int i = 0; i < r1.size() - 1; ++i)
    {
        if (r1[i] == 0)
        {
            if ((r1[i] == 0 && r2[i] == 0) && (r1[i + 1] == 0 && r2[i + 1] == 0))
            {
                return true;
            }
        }
    }
    return false;
}