#include <iostream>
#include <string>

#include "phases/Phase01ParseSerialize.h"
#include "phases/Phase02ReadJsonTypes.h"
#include "phases/Phase03BuildJsonProgrammatically.h"
#include "phases/Phase04CustomStructToJson.h"
#include "phases/Phase05JsonToCustomStruct.h"
#include "phases/Phase06ValidationAndErrorHandling.h"

namespace
{
void PrintMenu()
{
    std::cout << "\n=== Potato.Study.BoostJson ===\n";
    std::cout << "1. Phase 01 - Parse and serialize\n";
    std::cout << "2. Phase 02 - Read JSON types\n";
    std::cout << "3. Phase 03 - Build JSON programmatically\n";
    std::cout << "4. Phase 04 - Custom struct to JSON\n";
    std::cout << "5. Phase 05 - JSON to custom struct\n";
    std::cout << "6. Phase 06 - Validation and error handling\n";
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
        else if (input == "2")
        {
            std::cout << "\n--- Phase 02 - Read JSON Types ---\n\n";
            RunPhase02ReadJsonTypes();
        }
        else if (input == "3")
        {
            std::cout << "\n--- Phase 03 - Build JSON Programmatically ---\n\n";
            RunPhase03BuildJsonProgrammatically();
        }
        else if (input == "4")
        {
            std::cout << "\n--- Phase 04 - Custom Struct to JSON ---\n\n";
            RunPhase04CustomStructToJson();
        }
        else if (input == "5")
        {
            std::cout << "\n--- Phase 05 - JSON to Custom Struct ---\n\n";
            RunPhase05JsonToCustomStruct();
        }
        else if (input == "6")
        {
            std::cout << "\n--- Phase 06 - Validation and Error Handling ---\n";
            RunPhase06ValidationAndErrorHandling();
        }
        else if (input == "q" || input == "Q")
        {
            break;
        }
        else
        {
            std::cout << "Unknown command. Select 1, 2, 3, 4, 5, 6, or q.\n";
        }
    }

    return 0;
}
