#include "Phase02ReadJsonTypes.h"

#include <iostream>
#include <string>

#include <boost/json.hpp>

namespace json = boost::json;

namespace
{
const char* DescribeKind(const json::value& value)
{
    switch (value.kind())
    {
    case json::kind::object:
        return "object";
    case json::kind::array:
        return "array";
    case json::kind::string:
        return "string";
    case json::kind::int64:
        return "int64";
    case json::kind::uint64:
        return "uint64";
    case json::kind::double_:
        return "double";
    case json::kind::bool_:
        return "bool";
    case json::kind::null:
        return "null";
    default:
        return "unknown";
    }
}
}

void RunPhase02ReadJsonTypes()
{
    const std::string source = R"({
  "profile": {
    "name": "potato",
    "age": 3
  },
  "ratio": 0.75,
  "tags": ["boost", "json"],
  "enabled": "true",
  "nickname": null
})";

    const json::value parsed = json::parse(source);
    const json::object& root = parsed.as_object();
 
    const json::value& profileValue = root.at("profile");
    const json::object& profile = profileValue.as_object();
    const json::value& ratioValue = root.at("ratio");
    const json::value& tagsValue = root.at("tags");
    const json::array& tags = tagsValue.as_array();
    const json::value& enabledValue = root.at("enabled");
    const json::value& nicknameValue = root.at("nickname");

    std::cout << "Original JSON text:\n" << source << "\n\n";
    std::cout << "Root kind = " << DescribeKind(parsed) << "\n";
    std::cout << "profile kind = " << DescribeKind(profileValue) << "\n";
    std::cout << "profile.name kind = " << DescribeKind(profile.at("name"))
              << ", value = " << profile.at("name").as_string().c_str() << "\n";
    std::cout << "profile.age kind = " << DescribeKind(profile.at("age"))
              << ", value = " << profile.at("age").as_int64() << "\n";
    std::cout << "ratio kind = " << DescribeKind(ratioValue)
              << ", value = " << ratioValue.as_double() << "\n";
    std::cout << "tags kind = " << DescribeKind(tagsValue)
              << ", size = " << tags.size() << "\n";

    for (std::size_t index = 0; index < tags.size(); ++index)
    {
        const json::value& item = tags.at(index);
        std::cout << "tags[" << index << "] kind = " << DescribeKind(item)
                  << ", value = " << item.as_string().c_str() << "\n";
    }

    std::cout << "enabled kind = " << DescribeKind(enabledValue)
              << ", value = " << (enabledValue.as_bool() ? "true" : "false") << "\n";
    std::cout << "nickname kind = " << DescribeKind(nicknameValue)
              << ", is_null = " << (nicknameValue.is_null() ? "true" : "false") << "\n";
}
