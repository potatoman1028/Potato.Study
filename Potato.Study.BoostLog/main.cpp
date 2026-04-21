#include <iostream>
#include <string>

#include "phases/Phase01Basic.h"
#include "phases/Phase02Formatting.h"
#include "phases/Phase03Sinks.h"
#include "phases/Phase04SeverityFilter.h"
#include "phases/Phase05CustomAttribute.h"
#include "phases/Phase06RotationFile.h"
#include "phases/Phase07MultiThread.h"
#include "phases/Phase08AsyncSink.h"

namespace
{
    void PrintMenu()
    {
        std::cout
            << "Select a phase to run:\n"
            << "  1. Phase 01 - Basic severity logger\n"
            << "  2. Phase 02 - Formatting and common attributes\n"
            << "  3. Phase 03 - Console sink and file sink\n"
            << "  4. Phase 04 - Severity filter on file sink\n"
            << "  5. Phase 05 - Custom request attribute\n"
            << "  6. Phase 06 - Rotation file sink\n"
            << "  7. Phase 07 - Multi-thread logging\n"
            << "  8. Phase 08 - Async sink\n"
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

        if (input == "4")
        {
            RunPhase04SeverityFilter();
            return true;
        }

        if (input == "5")
        {
            RunPhase05CustomAttribute();
            return true;
        }

        if (input == "6")
        {
            RunPhase06RotationFile();
            return true;
        }

        if (input == "7")
        {
            RunPhase07MultiThread();
            return true;
        }

        if (input == "8")
        {
            RunPhase08AsyncSink();
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

        std::cout << "Invalid selection. Enter 1, 2, 3, 4, 5, 6, 7, 8, or q.\n\n";
    }
}
