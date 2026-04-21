#include <iostream>
#include <string>

#include "phases/Phase01ParseSerialize.h"

namespace
{
void PrintMenu()
{
    std::cout << "\n=== Potato.Study.BoostJson ===\n";
    std::cout << "1. Phase 01 - Parse and serialize\n";
    std::cout << "q. Quit\n";
    std::cout << "> ";
}
}

int main()
{
    while (true)
    {
        PrintMenu();

        std::string input;
        if (!std::getline(std::cin, input))
        {
            break;
        }

        if (input == "1")
        {
            std::cout << "\n--- Phase 01 - Parse and Serialize ---\n\n";
            RunPhase01ParseSerialize();
        }
        else if (input == "q" || input == "Q")
        {
            break;
        }
        else
        {
            std::cout << "Unknown command. Select 1 or q.\n";
        }
    }

    return 0;
}