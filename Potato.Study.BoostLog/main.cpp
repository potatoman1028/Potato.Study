#include <iostream>
#include <string>

#include "phases/Phase01Basic.h"
#include "phases/Phase02Formatting.h"
#include "phases/Phase03Sinks.h"

namespace
{
    void PrintMenu()
    {
        std::cout
            << "Select a phase to run:\n"
            << "  1. Phase 01 - Basic severity logger\n"
            << "  2. Phase 02 - Formatting and common attributes\n"
            << "  3. Phase 03 - Console sink and file sink\n"
            << "  q. Quit\n"
            << "> ";
    }

    bool RunSelectedPhase(const std::string& input)
    {
        if (input == "1")
        {
            RunPhase01Basic();
            return true;
        }

        if (input == "2")
        {
            RunPhase02Formatting();
            return true;
        }

        if (input == "3")
        {
            RunPhase03Sinks();
            return true;
        }

        return false;
    }
}

int main()
{
    while (true)
    {
        PrintMenu();

        std::string input;
        std::getline(std::cin, input);

        if (input == "q" || input == "Q")
        {
            return 0;
        }

        if (RunSelectedPhase(input))
        {
            std::cout << "\nPhase completed. Returning to menu.\n\n";
            continue;
        }

        std::cout << "Invalid selection. Enter 1, 2, 3, or q.\n\n";
    }
}
