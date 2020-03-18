#include "pch.h"
#include "Generator.h"

boost::mutex Generator::mtx_;
std::vector<double> Generator::threadReports;
double Generator::threadMax;

Generator::Generator(int depth, int length, bool fastCount)
{
    this->partitionSets =
        std::make_shared<std::vector<std::vector<std::vector<short>>>>();
    this->dimColumn = length;
    this->dimRow = depth;
    this->fastCount = fastCount;
    this->patternsPossible =
        (boost::multiprecision::cpp_int)std::pow(2, length * depth);
    this->genSetOfPartSets();

    for (int i = 0; i < std::pow(2, this->dimColumn); ++i)
    {
        this->rows.push_back(Row(i, this->dimColumn, this->partitionSets));
    }

    this->goodPatterns = 0;

    // Begin multithreaded tree building.
    int concurentThreadsSupported = std::thread::hardware_concurrency();
    std::vector<std::shared_ptr<boost::thread>> threads;
    for (int i = 0; i < concurentThreadsSupported; ++i)
    {
        auto t = std::make_shared<boost::thread>(boost::thread(
            &Generator::buildTree, this, concurentThreadsSupported, i));
        threadReports.push_back(0);
        threads.push_back(t);
    }

    for (int i = 0; i < concurentThreadsSupported; ++i)
    {
        threads[i]->join();
    }

    if (this->fastCount)
    {
        this->countPatterns();
    }
}

void Generator::buildTree(int threadCount, int id)
{
    for (int i = id; i < this->rows.size(); i += threadCount)
    {
        double percent = this->rows[i].seed / std::pow(2, this->dimColumn) * 100;
        Generator::threadReports[id] = percent;
        Generator::mtx_.lock();
        if (percent > Generator::threadMax)
        {

            Generator::threadMax = percent;
            std::cout << "\rBuilding tree... " << std::setprecision(4) << percent
                      << " %                      ";
        }
        Generator::mtx_.unlock();
        for (int j = 0; j < this->rows.size(); ++j)
        {
            if (!this->rows[i].formsPool(this->rows[j]) &&
                this->rows[i].isPartiallyContiguous(this->rows[j]))
            {
                Row::mapNodes(this->rows[i], this->rows[j]);
            }
        }
    }
}

Generator::~Generator() {}

void Generator::insert_commas(std::string& num) {
    int insertPosition = num.length() - 3;
    while (insertPosition > 0) {
        num.insert(insertPosition, ",");
        insertPosition -= 3;
    }
}

void Generator::genSetOfPartSets()
{
    int partMax = 0;

    if (this->dimColumn % 2 == 0)
    {
        partMax = this->dimColumn / 2;
    }
    else
    {
        partMax = std::floor((this->dimColumn + 1) / 2);
    }

    for (int i = 0; i < (partMax + 1); ++i)
    {
        this->restrictedGrowthStrings(i);
    }
}

void Generator::restrictedGrowthStrings(int setLength)
{
    int n = setLength - 1;
    bool incrementable = true;
    std::vector<short> mainRow;
    std::vector<short> auxRow;
    std::vector<std::vector<short>> lexicoStrings;

    // Initialize rows and push back initial set
    for (int i = 0; i < setLength; ++i)
    {
        mainRow.push_back(0);
    }
    for (int i = 0; i < setLength - 1; ++i)
    {
        auxRow.push_back(1);
    }
    lexicoStrings.push_back(mainRow);

    while (incrementable)
    {
        incrementable = false;
        for (int index = n; index > 0; --index)
        {
            if (mainRow[index] < n && mainRow[index] < auxRow[index - 1])
            {
                incrementable = true;
                ++mainRow[index];
                this->setToZero(mainRow, index);
                this->updateAuxRow(mainRow, auxRow);
                lexicoStrings.push_back(mainRow);
                break;
            }
        }
    }
    this->partitionSets->push_back(lexicoStrings);
}

void Generator::updateAuxRow(const std::vector<short> &mainRow,
                             std::vector<short> &auxRow)
{
    for (int i = 1; i < mainRow.size(); ++i)
    {
        auxRow[i - 1] = *std::max_element(mainRow.begin(), mainRow.begin() + i) + 1;
    }
}

void Generator::setToZero(std::vector<short> &mainRow, int index)
{
    if (index != mainRow.size() - 1)
    {
        for (int i = index + 1; i < mainRow.size(); ++i)
        {
            mainRow[i] = 0;
        }
    }
}

void Generator::countPatterns()
{
    bool water = true;
    for (int i = 0; i < this->rows.size(); ++i)
    {
        for (int j = 0; j < this->rows[i].nodes.size(); ++j)
        {
            if (this->rows[i].nodes[j]->isStart)
            {
                if (this->rows[i].nodes[j]->segments.size() >= 1)
                {
                    water = true;
                }
                else
                {
                    water = false;
                }
                /*std::vector<std::vector<char>> rows;
        std::vector<char> row;
        for (size_t i = 0; i < this->dimColumn; ++i) {
            row.push_back(i);
        }

        for (size_t i = 0; i < this->dimRow; ++i) {
            rows.push_back(row);
        }*/
                this->goodPatterns += this->rows[i].nodes[j]->traverseAndMemoize(
                    water, false, 1, this->dimRow);
                /*this->goodPatterns += this->rows[i].nodes[j]->traverse(rows, water,
         * false, 1, this->dimRow);*/
            }
        }
    }
}
