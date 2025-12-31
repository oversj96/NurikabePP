# Quick Fixes - Immediate Action Items
## Nurikabe++ Critical Bug Fixes

This document provides specific code changes to fix critical bugs identified in the codebase analysis.

---

## 1. Fix Integer Overflow in Power Calculations

### Problem
`std::pow()` returns `double` and causes precision loss and overflow for large exponents.

### Files to Update

#### Generator.cpp - Line 18
```cpp
// BEFORE
this->patternsPossible = (boost::multiprecision::cpp_int)std::pow(2, length * depth);

// AFTER
this->patternsPossible = boost::multiprecision::cpp_int(1) << (length * depth);
```

#### Generator.cpp - Line 21
```cpp
// BEFORE
for (int i = 0; i < std::pow(2, this->dimColumn); ++i)

// AFTER
int maxRows = 1 << this->dimColumn;
for (int i = 0; i < maxRows; ++i)
```

#### Generator.cpp - Line 81
```cpp
// BEFORE
double percent = this->rows[i].seed / std::pow(2, this->dimColumn) * 100;

// AFTER
double percent = this->rows[i].seed / static_cast<double>(1 << this->dimColumn) * 100;
```

#### Row.cpp - Line 24
```cpp
// BEFORE
int bitVal = std::pow(2, i);

// AFTER
int bitVal = 1 << i;
```

#### Node.cpp - Line 165
```cpp
// BEFORE
std::to_string(std::round(this->parentSeed / std::pow(2, this->parentLength) * 100))

// AFTER
std::to_string(std::round(this->parentSeed / static_cast<double>(1 << this->parentLength) * 100))
```

#### Node.cpp - Line 305
```cpp
// BEFORE
int bitVal = std::pow(2, i);

// AFTER
int bitVal = 1 << i;
```

#### Pattern.cpp - Line 44
```cpp
// BEFORE
if (seed < std::pow(2, rowLength * rowCount))

// AFTER
// For large boards, use multiprecision comparison
boost::multiprecision::cpp_int maxSeed = boost::multiprecision::cpp_int(1) << (rowLength * rowCount);
if (boost::multiprecision::cpp_int(seed) < maxSeed)
```

#### Pattern.cpp - Line 182
```cpp
// BEFORE
seed += std::pow(2, i);

// AFTER
seed += (1ULL << i);
```

#### Pattern.cpp - Line 196
```cpp
// BEFORE
seed += std::pow(2, i);

// AFTER
seed += (1u << i);
```

#### Pattern.cpp - Line 245
```cpp
// BEFORE
int bitVal = std::pow(2, i);

// AFTER
unsigned long long bitVal = 1ULL << i;
```

---

## 2. Fix NULL Comparison with char

### Row.cpp - Line 200
```cpp
// BEFORE
if (minVals[i] != NULL && minVals[i] > 0)

// AFTER
if (minVals[i] != -1 && minVals[i] > 0)
```

---

## 3. Fix bigpow Logic Error

### Pattern.cpp - Lines 202-213
```cpp
// BEFORE
boost::multiprecision::cpp_int Pattern::bigpow(boost::multiprecision::cpp_int lhs, boost::multiprecision::cpp_int rhs)
{
    if (rhs > 0)
    {
        return lhs * Pattern::bigpow(lhs, rhs - 1);
    }
    else
    {
        return lhs;  // WRONG!
    }
}

// AFTER - Option 1: Fix base case
boost::multiprecision::cpp_int Pattern::bigpow(boost::multiprecision::cpp_int lhs, boost::multiprecision::cpp_int rhs)
{
    if (rhs == 0)
    {
        return 1;
    }
    else if (rhs == 1)
    {
        return lhs;
    }
    else
    {
        return lhs * Pattern::bigpow(lhs, rhs - 1);
    }
}

// AFTER - Option 2: Use boost's built-in function (RECOMMENDED)
// Replace all calls to Pattern::bigpow with:
boost::multiprecision::pow(base, exponent);
// And remove the bigpow function entirely
```

---

## 4. Fix Platform-Specific Path Separators

### Generator.cpp - Line 294
```cpp
// BEFORE
fs::path p{"output\\" + fileName};

// AFTER
fs::path p{"output"};
p /= fileName;
```

### Generator.cpp - Line 318-319
```cpp
// BEFORE
fs::path filePath1{"output\\" + fileName1};
fs::path filePath2{"output\\" + fileName2};

// AFTER
fs::path filePath1{"output"};
filePath1 /= fileName1;
fs::path filePath2{"output"};
filePath2 /= fileName2;
```

---

## 5. Fix Division by Zero

### Generator.cpp - Lines 99-103
```cpp
// BEFORE
template <typename T>
T Generator::mean(const std::vector<T> &v)
{
    return std::accumulate(v.begin(), v.end(), 0) / v.size();
}

// AFTER
template <typename T>
T Generator::mean(const std::vector<T> &v)
{
    if (v.empty())
    {
        return T(0);
    }
    return std::accumulate(v.begin(), v.end(), T(0)) / v.size();
}
```

---

## 6. Remove Unused/Shadow Variable

### Generator.cpp - Line 7
```cpp
// BEFORE
std::vector<boost::multiprecision::cpp_int> rowCounts;

// AFTER
// Delete this line entirely - it shadows the member variable and is never used
```

---

## 7. Fix Unsafe Manual Mutex Locking

### Node.cpp - Lines 319-343
```cpp
// BEFORE
void Node::printPuzzle(std::vector<std::vector<char>> rows)
{
    int interval = 1;
    Pattern pat(rows[0].size(), rows, false);
    mtx_.lock();
    ++Node::debugCount;
    if (Node::debugCount % interval == 0)
    {
        // ... complex logic that could throw ...
    }
    mtx_.unlock();
}

// AFTER
void Node::printPuzzle(std::vector<std::vector<char>> rows)
{
    int interval = 1;
    Pattern pat(rows[0].size(), rows, false);
    
    {
        std::lock_guard<boost::mutex> lock(mtx_);
        ++Node::debugCount;
        
        if (Node::debugCount % interval == 0)
        {
            Node::patternSeeds.push_back(pat.patternSeed);
            if (!pat.isLegal || pat.patternSeed == 4465420545 || pat.patternSeed == 4465422466)
            {
                std::cout << pat.patternString();
                std::cin.get();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else
            {
                if (Node::debugCount % 10000 == 0)
                {
                    std::cout << Node::debugCount << "\n";
                    std::cout << pat.patternString() << std::endl;
                }
            }
        }
    }  // Lock automatically released here
}
```

### Generator.cpp - Lines 82-87
```cpp
// BEFORE
Generator::mtx_.lock();
Generator::threadReports[id] = percent;
double percentMean = Generator::mean(Generator::threadReports);
std::cout << "\rBuilding tree... " << std::setprecision(4) << percentMean
          << " %                 ";
Generator::mtx_.unlock();

// AFTER
{
    std::lock_guard<boost::mutex> lock(Generator::mtx_);
    Generator::threadReports[id] = percent;
    double percentMean = Generator::mean(Generator::threadReports);
    std::cout << "\rBuilding tree... " << std::setprecision(4) << percentMean
              << " %                 ";
}
```

---

## 8. Add Missing Include for std::lock_guard

### pch.h - Add after existing includes
```cpp
#include <mutex>  // For std::lock_guard if migrating from boost
```

Or continue using boost:
```cpp
#include <boost/thread/lock_guard.hpp>  // For boost::lock_guard
```

Then in Node.cpp and Generator.cpp, use:
```cpp
boost::lock_guard<boost::mutex> lock(mtx_);
```

---

## 9. Fix Inefficient Heap Operations

### Generator.cpp - Lines 300-302
```cpp
// BEFORE
std::make_heap(seeds.begin(), seeds.end());
std::cout << "\nSorting pattern seeds...\n";
std::sort_heap(seeds.begin(), seeds.end());

// AFTER
std::cout << "\nSorting pattern seeds...\n";
std::sort(seeds.begin(), seeds.end());
```

---

## 10. Add Input Validation

### Nurikabe++.cpp - After line 117-118
```cpp
// BEFORE
int dimRows = getInput("\nPlease enter a row count >1: ", false);
int dimLength = getInput("\nPlease enter a column count >1: ", false);

// AFTER
int dimRows = getInput("\nPlease enter a row count >1: ", false);
while (dimRows < 2)
{
    std::cout << "Row count must be at least 2. Please try again.\n";
    dimRows = getInput("\nPlease enter a row count >1: ", false);
}

int dimLength = getInput("\nPlease enter a column count >1: ", false);
while (dimLength < 2)
{
    std::cout << "Column count must be at least 2. Please try again.\n";
    dimLength = getInput("\nPlease enter a column count >1: ", false);
}
```

---

## 11. Remove Commented Code

### Generator.cpp - Line 47
```cpp
// BEFORE
if (this->fastCount)
{
    this->countPatterns();
    //this->sumRowMemos();
}

// AFTER
if (this->fastCount)
{
    this->countPatterns();
}
```

---

## 12. Add Documentation for Magic Numbers

### Node.cpp - Lines 328
```cpp
// BEFORE
if (!pat.isLegal || pat.patternSeed == 4465420545 || pat.patternSeed == 4465422466)

// AFTER
// Known problematic pattern seeds for debugging purposes:
// 4465420545 - Edge case with pool formation
// 4465422466 - Edge case with partition connectivity
constexpr unsigned long long DEBUG_SEED_1 = 4465420545;
constexpr unsigned long long DEBUG_SEED_2 = 4465422466;

if (!pat.isLegal || pat.patternSeed == DEBUG_SEED_1 || pat.patternSeed == DEBUG_SEED_2)
```

---

## Testing Checklist

After applying these fixes, test:

- [ ] Small boards (2x2, 3x3, 4x4) produce known correct counts
- [ ] Medium boards (5x5 through 8x8) match README values
- [ ] Large boards (9x9 through 13x13) match README values
- [ ] Pattern visualization works correctly
- [ ] File comparison functionality works
- [ ] No crashes with invalid input
- [ ] No deadlocks during multi-threaded execution
- [ ] Pattern seeds are correctly calculated
- [ ] Output files are created with correct format

---

## Build Command

After making changes, rebuild:

```bash
# If using Visual Studio:
# Open Nurikabe++.sln and build in Release mode

# If creating CMake build (recommended):
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

---

## Verification

Run the tool with known board sizes:

```bash
./Nurikabe++
# Select option 1
# Enter row count: 4
# Enter column count: 4
# Expected output: 3,756
```

Compare with README.md values to ensure calculations are correct.
