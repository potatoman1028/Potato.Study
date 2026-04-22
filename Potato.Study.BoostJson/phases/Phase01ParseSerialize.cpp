#include "Phase01ParseSerialize.h"

#include <iostream>
#include <string>

#include <boost/json.hpp>

namespace json = boost::json;

void RunPhase01ParseSerialize()
{
    const std::string source = R"({
  "project": "Potato.Study.BoostJson",
  "phase": 1,
  "features": ["parse", "serialize"],
  "enabled": false
})";

    const json::value parsed = json::parse(source);
    const json::object& root = parsed.as_object();
    const std::string serialized = json::serialize(parsed);

    std::cout << "Original JSON text:\n" << source << "\n\n";
    std::cout << "Parsed root kind: object\n";
    std::cout << "project = " << root.at("project").as_string().c_str() << "\n";
    std::cout << "phase = " << root.at("phase").as_int64() << "\n";
    std::cout << "enabled = " << (root.at("enabled").as_bool() ? "true" : "false") << "\n\n";
    std::cout << "Serialized JSON text:\n" << serialized << "\n";
}