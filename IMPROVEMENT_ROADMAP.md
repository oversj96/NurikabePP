# Nurikabe++ Improvement Roadmap

This document provides a structured, trackable roadmap for improving the codebase quality.

---

## Sprint 1: Critical Bug Fixes (Week 1)

**Goal:** Eliminate all critical bugs that could cause incorrect results or crashes

### Day 1-2: Integer Arithmetic Fixes
- [ ] Replace all `std::pow(2, x)` with bit shifting (`1 << x`)
- [ ] Update Generator.cpp (lines 18, 21, 81)
- [ ] Update Row.cpp (line 24)
- [ ] Update Node.cpp (lines 165, 305)
- [ ] Update Pattern.cpp (lines 44, 182, 196, 245)
- [ ] Test with small boards (2x2 through 4x4)
- [ ] Test with medium boards (5x5 through 8x8)
- [ ] Test with large boards (9x9 and above)

**Acceptance Criteria:**
- All pattern counts match known values in README.md
- No compilation warnings about integer overflow
- Pattern seeds calculated correctly for all board sizes

### Day 3: Logic and Type Fixes
- [ ] Fix NULL comparison in Row.cpp (line 200)
- [ ] Fix Pattern::bigpow base case (line 211) or replace with boost::multiprecision::pow
- [ ] Remove shadow variable in Generator.cpp (line 7)
- [ ] Add division by zero check in Generator::mean()

**Acceptance Criteria:**
- Code compiles without warnings
- Static analysis (cppcheck) passes
- All tests pass

### Day 4: Platform Compatibility
- [ ] Fix Windows-specific path separators in Generator.cpp (lines 294, 318-319)
- [ ] Test file output on Linux/macOS
- [ ] Test file comparison on Linux/macOS

**Acceptance Criteria:**
- Application runs on Windows, Linux, and macOS
- File paths work correctly on all platforms
- Output files created in correct location

### Day 5: Thread Safety
- [ ] Replace manual mutex lock/unlock with RAII guards
- [ ] Update Node.cpp printPuzzle() (lines 323-343)
- [ ] Update Generator.cpp buildTree() (lines 82-87)
- [ ] Add boost::lock_guard include to pch.h

**Acceptance Criteria:**
- No potential deadlocks (verified with ThreadSanitizer)
- Exception safety verified
- Multi-threaded execution stable

---

## Sprint 2: Code Quality Improvements (Week 2)

**Goal:** Improve code maintainability and readability

### Day 6-7: Naming Conventions
- [ ] Standardize method names (generateBits, not generatebits)
- [ ] Standardize dimension naming (columnCount, rowCount)
- [ ] Document naming conventions in CONTRIBUTING.md
- [ ] Apply clang-format with agreed style

**Files to Update:**
- Row.h/cpp: `generatebits()` → `generateBits()`
- Node.h: `generatebits()` → `generateBits()` (static version)
- All files: consistent parameter naming

### Day 8: Const Correctness
- [ ] Add const to Segment::connects()
- [ ] Add const to Row::formsPool()
- [ ] Add const to Row::isPartiallyContiguous()
- [ ] Add const to getter methods
- [ ] Mark read-only parameters as const references

**Acceptance Criteria:**
- Compile with `-Wcast-qual` warning enabled
- No const-related warnings

### Day 9: Input Validation
- [ ] Add range validation in getInput()
- [ ] Validate minimum board size (>1)
- [ ] Add maximum board size limit (prevent memory exhaustion)
- [ ] Improve error messages
- [ ] Add input validation unit tests

**Acceptance Criteria:**
- Invalid input gracefully rejected
- Clear error messages guide user
- No crashes from invalid input

### Day 10: Code Cleanup
- [ ] Remove commented code (Generator.cpp line 47)
- [ ] Extract magic numbers to named constants
- [ ] Document DEBUG_SEED values in Node.cpp
- [ ] Remove unused includes
- [ ] Fix TODO comments in pch.h

**Acceptance Criteria:**
- No commented-out code remains
- All magic numbers explained
- Clean static analysis report

---

## Sprint 3: Documentation (Week 3)

**Goal:** Make codebase understandable to new contributors

### Day 11-12: File and Class Documentation
- [ ] Add file header comments (purpose, author, license)
- [ ] Add class-level Doxygen comments
- [ ] Document class responsibilities
- [ ] Document class relationships

**Template:**
```cpp
/**
 * @file Generator.h
 * @brief Pattern generation engine for Nurikabe puzzle counting
 * @author Justin Overstreet
 * @date 2025
 * 
 * This file implements the core algorithm for counting valid Nurikabe
 * patterns using a graph-based approach with memoization.
 */
```

### Day 13: Function Documentation
- [ ] Add Doxygen comments to all public methods
- [ ] Document parameters and return values
- [ ] Document preconditions and postconditions
- [ ] Add complexity notes where relevant

**Template:**
```cpp
/**
 * @brief Generates all partition sets up to maximum size
 * 
 * Creates restricted growth strings for partitioning segments.
 * Uses lexicographic ordering for enumeration.
 * 
 * @pre dimColumn must be > 0
 * @post partitionSets populated with all valid partitions
 * @complexity O(Bell(n)) where n is dimColumn/2
 */
void genSetOfPartSets();
```

### Day 14: Algorithm Documentation
- [ ] Document restricted growth string algorithm
- [ ] Explain partition mapping logic
- [ ] Document traversal algorithm
- [ ] Add diagrams for complex algorithms

Create `docs/ALGORITHMS.md` with:
- High-level algorithm overview
- Step-by-step breakdown
- Example walkthrough for small board
- References to papers/sources

### Day 15: README Enhancement
- [ ] Add project description
- [ ] Add build instructions (Visual Studio)
- [ ] Add build instructions (CMake)
- [ ] Document dependencies (Boost version, C++ standard)
- [ ] Add usage examples
- [ ] Add command-line interface documentation
- [ ] Add performance characteristics
- [ ] Add contributor guidelines

---

## Sprint 4: Testing Infrastructure (Week 4)

**Goal:** Establish automated testing

### Day 16-17: Unit Test Framework
- [ ] Choose test framework (Google Test recommended)
- [ ] Add Google Test to project
- [ ] Create test directory structure
- [ ] Write CMakeLists.txt for tests

Directory structure:
```
tests/
  unit/
    test_segment.cpp
    test_row.cpp
    test_node.cpp
    test_pattern.cpp
    test_generator.cpp
  integration/
    test_small_boards.cpp
    test_pattern_validation.cpp
  data/
    known_patterns_4x4.txt
    known_patterns_5x5.txt
```

### Day 18: Core Algorithm Tests
- [ ] Test Segment::connects()
- [ ] Test Row partition generation
- [ ] Test Pattern validation logic
- [ ] Test pool detection
- [ ] Test connectivity checking

**Minimum Coverage:**
- Segment: 100%
- Row: 80%
- Pattern: 80%
- Node: 60%
- Generator: 40% (integration tests better)

### Day 19: Integration Tests
- [ ] Test 2x2 board (known: 13 patterns)
- [ ] Test 3x3 board (known: 140 patterns)
- [ ] Test 4x4 board (known: 3,756 patterns)
- [ ] Test pattern visualization
- [ ] Test file I/O

### Day 20: CI/CD Setup
- [ ] Create GitHub Actions workflow
- [ ] Add build job (Linux, Windows, macOS)
- [ ] Add test job
- [ ] Add static analysis job (cppcheck)
- [ ] Add code coverage report
- [ ] Add badge to README

---

## Sprint 5: Performance Optimization (Week 5)

**Goal:** Improve runtime performance

### Day 21: Profiling
- [ ] Profile with gprof or perf
- [ ] Identify hot paths
- [ ] Measure memory usage
- [ ] Create baseline performance metrics

Document in `docs/PERFORMANCE.md`:
- Time per board size
- Memory usage per board size
- Thread scaling characteristics

### Day 22-23: Memory Optimizations
- [ ] Add vector reserves
- [ ] Eliminate unnecessary copies
- [ ] Review shared_ptr usage (consider unique_ptr)
- [ ] Pool allocations if beneficial

**Target:** 10-20% reduction in memory usage

### Day 24: Algorithm Optimizations
- [ ] Review memoization strategy
- [ ] Optimize hot loops
- [ ] Consider SIMD for bit operations
- [ ] Optimize partition generation

**Target:** 10-20% reduction in runtime

### Day 25: Threading Improvements
- [ ] Review thread pool size
- [ ] Optimize work distribution
- [ ] Reduce lock contention
- [ ] Consider lock-free data structures

**Target:** Better scaling to 16+ cores

---

## Sprint 6: Modern C++ Migration (Week 6)

**Goal:** Migrate from Boost to standard library where possible

### Day 26: Filesystem Migration
- [ ] Replace boost::filesystem with std::filesystem (C++17)
- [ ] Update CMakeLists.txt for C++17 requirement
- [ ] Test on all platforms

### Day 27: Threading Migration
- [ ] Replace boost::thread with std::thread
- [ ] Replace boost::mutex with std::mutex
- [ ] Replace boost::lock_guard with std::lock_guard
- [ ] Test thread safety

### Day 28: Multiprecision Strategy
- [ ] Document why boost::multiprecision is needed
- [ ] Consider alternatives (GMP wrapper)
- [ ] Keep boost::multiprecision if best option

### Day 29-30: Build System Modernization
- [ ] Create CMakeLists.txt for cross-platform build
- [ ] Add find_package for dependencies
- [ ] Add installation targets
- [ ] Add package configuration
- [ ] Document build process

---

## Sprint 7: Architecture Refactoring (Weeks 7-8)

**Goal:** Improve separation of concerns

### Week 7: Reduce Coupling
- [ ] Remove static state from Row (setOfPartitionSets)
- [ ] Remove static state from Node (debugCount, patternSeeds)
- [ ] Pass dependencies explicitly
- [ ] Create PartitionSetManager class
- [ ] Create DebugLogger class

### Week 8: Split Responsibilities
- [ ] Split Node into GraphNode and PatternValidator
- [ ] Split Generator into PatternGenerator and ProgressReporter
- [ ] Create separate PatternPrinter class
- [ ] Document new architecture in docs/ARCHITECTURE.md

---

## Ongoing: Maintenance

### Monthly
- [ ] Review and address static analysis warnings
- [ ] Update dependencies
- [ ] Review performance metrics
- [ ] Address user-reported issues

### Quarterly
- [ ] Review test coverage
- [ ] Update documentation
- [ ] Refactor based on learnings
- [ ] Plan next features

---

## Success Metrics

### Code Quality
- **Current:** ~50 compiler warnings, multiple critical bugs
- **Target Sprint 2:** 0 warnings, 0 critical bugs
- **Target Sprint 4:** 80% test coverage
- **Target Sprint 7:** Clean architecture, low coupling

### Performance
- **Current:** Baseline TBD
- **Target Sprint 5:** 15% faster, 15% less memory
- **Target Sprint 7:** Better scaling to 16+ cores

### Maintainability
- **Current:** No documentation, unclear architecture
- **Target Sprint 3:** Full API documentation
- **Target Sprint 7:** Clear architecture, easy to modify

### Reliability
- **Current:** Known bugs, no tests
- **Target Sprint 1:** All critical bugs fixed
- **Target Sprint 4:** Automated testing, CI/CD

---

## Dependencies and Prerequisites

### Required Tools
- C++17 or later compiler (GCC 9+, Clang 10+, MSVC 2019+)
- CMake 3.15+
- Boost 1.70+ (will reduce in Sprint 6)
- Git

### Optional Tools
- Doxygen (for documentation)
- Google Test (for testing)
- clang-format (for formatting)
- cppcheck (for static analysis)
- AddressSanitizer (for memory checks)
- ThreadSanitizer (for race detection)

---

## Risk Management

### Technical Risks
| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|------------|
| Breaking changes in refactoring | High | High | Comprehensive tests before refactoring |
| Performance regression | Medium | High | Benchmark before/after changes |
| Platform compatibility issues | Low | Medium | Test on all platforms early |
| Boost migration issues | Medium | Medium | Migrate incrementally, one library at a time |

### Process Risks
| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|------------|
| Scope creep | Medium | Medium | Stick to sprint goals |
| Documentation lag | High | Low | Document as you code |
| Test maintenance burden | Medium | Medium | Keep tests simple and focused |

---

## Notes

- This roadmap is flexible - adjust based on actual progress
- Each sprint should end with working, tested code
- Document decisions and learnings in `docs/DECISIONS.md`
- Get code review for critical changes
- Celebrate completed sprints! 🎉

---

## Quick Reference

**Current Status:** Sprint 0 (Planning)  
**Next Milestone:** Complete Sprint 1 by [DATE]  
**Long-term Goal:** Production-ready, maintainable C++ application

**Quick Wins (Do First):**
1. Fix integer overflow bugs (Sprint 1, Day 1-2)
2. Add RAII lock guards (Sprint 1, Day 5)
3. Fix platform paths (Sprint 1, Day 4)
4. Add basic tests (Sprint 4, Day 16-17)

**Track Progress:**
- Create GitHub issues for each sprint
- Use project board for visualization
- Update this document with completion dates
- Add notes on challenges and solutions
