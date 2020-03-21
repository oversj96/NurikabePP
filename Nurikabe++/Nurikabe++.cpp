#include "pch.h"
#include "Generator.h"
#include "Pattern.h"

/* Start Console File Prototypes */
enum class Choice;
int getInput(std::string message, bool errorChecking);
std::string getInput(std::string message);
/* End Console File Prototypes */

enum class Choice
{
    generate = 1,
    display,
    compare,
    exit
};

int getInput(std::string message, bool errorChecking)
{
    bool input = true;
    int userInput;
    while (input)
    {
        try
        {
            std::cout << message;
            std::cin >> userInput;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("\n!!! Please enter one of the listed numbers. Try Again. !!!\n\n\n\n\n");
            }
            else
            {
                input = false;
            }
            if (errorChecking)
            {
                input = true;
                if (!std::cin.fail() && (userInput > 4 || userInput < 1))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    throw std::invalid_argument("\n!!! The selection entered was not listed. Try Again. !!!\n\n\n\n\n");
                }
                else
                {
                    input = false;
                }
            }
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << e.what();
        }
    }
    return userInput;
}

std::string getInput(std::string message)
{
    bool input = true;
    std::string userInput;
    while (input)
    {
        try
        {
            std::cout << message;
            std::cin >> userInput;
            input = false;
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << e.what();
        }
    }
    return userInput;
}

int main()
{
    using clock_type = std::chrono::high_resolution_clock;

    Choice userChoice = Choice::generate;
    int userInput;
    std::stringstream ss;
    ss << std::setw(10);
    ss << "                                                                     \n"
       << "                                                                     \n"
       << "      * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"
       << "      *                                                             *\n"
       << "      *                 Nurikabe++ Puzzle Tool                      *\n"
       << "      *                Author: Justin Overstreet                    *\n"
       << "      *                                                             *\n"
       << "      *                Please select a function:                    *\n"
       << "      *                                                             *\n"
       << "      *    1. Run Pattern Counter For A Specific Board Size         *\n"
       << "      *    2. Visualize A Pattern/Seed For A Specific Board Size    *\n"
       << "      *    3. Compare Seed Files (For Debugging)                    *\n"
       << "      *    4. Exit                                                  *\n"
       << "      *                                                             *\n"
       << "      * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"
       << "\n         Selection: ";

    while (userChoice != Choice::exit)
    {
        userInput = getInput(ss.str(), true);
        userChoice = static_cast<Choice>(userInput);

        switch (userChoice)
        {
        case Choice::generate:
        {
            int dimRows = getInput("\nPlease enter a row count >1: ", false);
            int dimLength = getInput("\nPlease enter a column count >1: ", false);
            bool fastCount = false;
            std::string answer = "";
            answer = getInput("\nDo you wish to use the debugger? (This will dramatically slow counting) [ y / n ]: ");
            char letter = std::tolower(answer[0]);
            if (letter == 'n')
            {
                fastCount = true;
            }
            std::cout << "\n\nPlease wait..\n\n";
            auto start = clock_type::now();
            Generator gen(dimLength, dimRows, fastCount);
            auto stop = clock_type::now();
            std::stringstream ss;
            ss << gen.goodPatterns;
            std::string num = ss.str();
            gen.insert_commas(num);
            std::cout << std::setprecision(2) << std::fixed << "\n\nTotal Good Patterns: " << num << "\n\n";
            auto total = stop - start;
            std::cout << "\nTime elapsed (seconds): "
                      << std::chrono::duration_cast<std::chrono::seconds>(total).count();
            std::cout << "\nTime elapsed (milliseconds): "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(total).count() << std::endl;
            std::cout << "Press any key to continue...\n\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
        case Choice::display:
        {
            unsigned int colCount = getInput("\nPlease enter a column count: ", false);
            unsigned int rowCount = getInput("\nPlease enter a row count: ", false);
            unsigned long long seed = std::stoull(getInput("Please enter a pattern seed: "));
            Pattern pat(colCount, rowCount, seed);
            if (pat.isLegal)
            {
                std::cout << "\n\nPuzzle Status: Valid" << std::endl;
            }
            else
            {
                std::cout << "\n\nPuzzle Status: Invalid" << std::endl;
            }

            std::cout << pat.patternString();
            break;
        }
        case Choice::compare:
        {
            std::string fileName1 = getInput("Please enter the name of file one: ");
            std::string fileName2 = getInput("Please enter the name of file two: ");
            Generator::compareFiles(fileName1, fileName2);
            break;
        }
        default:
            std::cerr << "A user choice not on the list was somehow selected!" << std::endl;
        }
    }

    return 0;
}
