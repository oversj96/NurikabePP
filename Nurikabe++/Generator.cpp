#include "pch.h"
#include "Generator.h"

boost::mutex Generator::mtx_;
std::vector<double> Generator::threadReports;
double Generator::threadVal;
std::vector<boost::multiprecision::cpp_int> rowCounts;

Generator::Generator(int length, int depth, bool fastCount)
{
    this->rowCounts;
    this->partitionSets =
        std::make_unique<std::vector<std::vector<std::shared_ptr<std::vector<char>>>>>();
    this->dimColumn = length;
    this->dimRow = depth;
    this->fastCount = fastCount;
    this->patternsPossible =
        (boost::multiprecision::cpp_int)std::pow(2, length * depth);
    this->genSetOfPartSets();
    Row::setOfPartitionSets = std::move(this->partitionSets);
    for (int i = 0; i < std::pow(2, this->dimColumn); ++i)
    {
        this->rows.push_back(Row(i, this->dimColumn));
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
        //this->sumRowMemos();
    }
    else
    {
        threads.clear();
        for (int i = 0; i < concurentThreadsSupported; ++i)
        {
            auto t = std::make_shared<boost::thread>(boost::thread(
                &Generator::countPatternsMulti, this, concurentThreadsSupported, i));
            threadReports.push_back(0);
            threads.push_back(t);
        }

        for (int i = 0; i < concurentThreadsSupported; ++i)
        {
            threads[i]->join();
        }
        // For printing seeds to file.
        std::stringstream ss;
        ss << "patterns[" << this->dimRow << "x" << this->dimColumn << "].txt";
        Generator::writeSeedsToFile(Node::patternSeeds, ss.str());
    }
}

Generator::~Generator()
{
    Node::patternSeeds.clear();
    Node::debugCount = 0;
}

void Generator::buildTree(int threadCount, int id)
{
    for (int i = id; i < this->rows.size(); i += threadCount)
    {
        double percent = this->rows[i].seed / std::pow(2, this->dimColumn) * 100;
        Generator::mtx_.lock();
        Generator::threadReports[id] = percent;
        double percentMean = Generator::mean(Generator::threadReports);
        std::cout << "\rBuilding tree... " << std::setprecision(4) << percentMean
                  << " %                 ";
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

template <typename T>
T Generator::mean(const std::vector<T> &v)
{
    return std::accumulate(v.begin(), v.end(), 0) / v.size();
}

void Generator::insert_commas(std::string &num)
{
    for (int insertPosition = num.length() - 3; insertPosition > 0; insertPosition -= 3)
    {
        num.insert(insertPosition, ",");
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
    std::vector<char> mainRow;
    std::vector<char> auxRow;
    std::vector<std::shared_ptr<std::vector<char>>> lexicoStrings;

    // Initialize rows and push back initial set
    for (int i = 0; i < setLength; ++i)
    {
        mainRow.push_back(0);
    }
    for (int i = 0; i < setLength - 1; ++i)
    {
        auxRow.push_back(1);
    }
    std::vector<char> copy = mainRow;
    lexicoStrings.push_back(std::make_shared<std::vector<char>>(copy));

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
                copy = mainRow;
                lexicoStrings.push_back(std::make_shared<std::vector<char>>(copy));
                break;
            }
        }
    }
    this->partitionSets->push_back(lexicoStrings);
}

void Generator::updateAuxRow(std::vector<char> &mainRow,
                             std::vector<char> &auxRow)
{
    for (int i = 1; i < mainRow.size(); ++i)
    {
        auxRow[i - 1] = *std::max_element(mainRow.begin(), mainRow.begin() + i) + 1;
    }
}

void Generator::setToZero(std::vector<char> &mainRow, int index)
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
                this->goodPatterns += this->rows[i].nodes[j]->traverseAndMemoize(
                    water, false, 1, this->dimRow);
            }
        }
    }
}

void Generator::countPatternsMulti(int threadCount, int id)
{
    bool water = true;
    for (int i = id; i < this->rows.size(); i += threadCount)
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
                std::vector<std::vector<char>> rows;
                std::vector<char> row;
                for (size_t i = 0; i < this->dimColumn; ++i)
                {
                    row.push_back(i);
                }

                for (size_t i = 0; i < this->dimRow; ++i)
                {
                    rows.push_back(row);
                }
                this->goodPatterns += this->rows[i].nodes[j]->traverse(rows, water,
                                                                       false, 1, this->dimRow);
            }
        }
    }
}

void Generator::sumRowMemos()
{
    for (size_t i = 0; i < this->rows.size(); ++i)
    {
        this->rowCounts.push_back(0);
    }

    size_t pos = 0;
    for (auto &row : this->rows)
    {
        for (auto &node : row.nodes)
        {
            for (auto &memo : node->memos)
            {
                rowCounts[pos] += memo;
            }
        }
        ++pos;
    }

    this->rowCounts = rowCounts;
}

void Generator::rowDetails()
{
    for (size_t i = 0; i < this->rows.size(); ++i)
    {
        std::cout << "Row Seed: " << this->rows[i].seed << "\n"
                  << "Row Segment Count: " << this->rows[i].segments.size() << "\n"
                  << "Row Shape: ";
        for (auto &bit : this->rows[i].bits)
        {
            std::cout << std::to_string(bit);
        }
        std::cout << "\n"
                  << "Total Good Patterns Associated: " << this->rowCounts[i] << "\n"
                  << std::endl;
    }
}

void Generator::writeSeedsToFile(std::vector<unsigned long long> &seeds, std::string fileName)
{
    namespace fs = boost::filesystem;
    fs::path p{"output\\" + fileName};
    if (!fs::exists(fs::path("output")))
    {
        fs::create_directory(fs::path("output"));
    }
    fs::ofstream file(p);
    std::make_heap(seeds.begin(), seeds.end());
    std::cout << "\nSorting pattern seeds...\n";
    std::sort_heap(seeds.begin(), seeds.end());
    std::cout << "Sorting complete!" << std::endl;
    std::cout << "Writing file...\n";
    for (size_t i = 0; i < seeds.size(); ++i)
    {
        file << seeds[i] << "\n";
    }
    file << std::endl;
    std::cout << "File written to: " << p.relative_path() << std::endl;
    file.close();
}

void Generator::compareFiles(std::string fileName1, std::string fileName2)
{
    namespace fs = boost::filesystem;

    fs::path filePath1{"output\\" + fileName1};
    fs::path filePath2{"output\\" + fileName2};

    if (fs::exists(filePath1) || fs::exists(filePath2))
    {
        fs::ifstream file1(filePath1);
        fs::ifstream file2(filePath2);
        std::string lineFile1 = "";
        std::string lineFile2 = "";
        std::cout << "\nReading Files..." << std::endl;
        unsigned long long count = 0;
        while (!file1.eof() && !file2.eof())
        {
            ++count;
            std::getline(file1, lineFile1);
            std::getline(file2, lineFile2);
            if (count % 10000 == 0)
            {
                std::cout << count << std::endl;
            }
            if (lineFile1 != lineFile2)
            {
                std::cout << "\n!! Mismatch Detected !!\n"
                          << filePath1.relative_path() << ": " << lineFile1 << "\n"
                          << filePath2.relative_path() << ": " << lineFile2 << "\n"
                          << std::endl;
                break;
            }
        }
    }
    else
    {
        std::cerr << "\nOne or both of the specified files does not exist!" << std::endl;
    }
}