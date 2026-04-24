#include "Phase06ValidationAndErrorHandling.h"

#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <vector>

#include <boost/json.hpp>

namespace json = boost::json;

namespace
{
struct LessonConfig
{
    std::string title;
    int phase;
    bool enabled;
    std::vector<std::string> tags;
};

bool ReadStringField(const json::object& obj, const char* key, std::string& out, std::string& error)
{
    const auto it = obj.find(key);
    if (it == obj.end())
    {
        error = std::string("missing required string field: ") + key;
        return false;
    }

    const json::value& value = it->value();
    if (!value.is_string())
    {
        error = std::string("field must be string: ") + key;
        return false;
    }

    out = value.as_string().c_str();
    return true;
}

bool ReadIntField(const json::object& obj, const char* key, int& out, std::string& error)
{
    const auto it = obj.find(key);
    if (it == obj.end())
    {
        error = std::string("missing required int field: ") + key;
        return false;
    }

    const json::value& value = it->value();
    if (!value.is_int64())
    {
        error = std::string("field must be int64: ") + key;
        return false;
    }

    const std::int64_t number = value.as_int64();
    if (number < std::numeric_limits<int>::min() || number > std::numeric_limits<int>::max())
    {
        error = std::string("field is outside int range: ") + key;
        return false;
    }

    out = static_cast<int>(number);
    return true;
}

bool ReadOptionalBoolField(const json::object& obj, const char* key, bool defaultValue, bool& out, std::string& error)
{
    const auto it = obj.find(key);
    if (it == obj.end())
    {
        out = defaultValue;
        return true;
    }

    const json::value& value = it->value();
    if (!value.is_bool())
    {
        error = std::string("field must be bool: ") + key;
        return false;
    }

    out = value.as_bool();
    return true;
}

bool ReadStringArrayField(const json::object& obj, const char* key, std::vector<std::string>& out, std::string& error)
{
    const auto it = obj.find(key);
    if (it == obj.end())
    {
        error = std::string("missing required array field: ") + key;
        return false;
    }

    const json::value& value = it->value();
    if (!value.is_array())
    {
        error = std::string("field must be array: ") + key;
        return false;
    }

    const json::array& array = value.as_array();
    for (std::size_t index = 0; index < array.size(); ++index)
    {
        const json::value& item = array.at(index);
        if (!item.is_string())
        {
            error = std::string("array item must be string: ") + key + "[" + std::to_string(index) + "]";
            return false;
        }

        out.push_back(item.as_string().c_str());
    }

    return true;
}

std::optional<LessonConfig> TryReadLessonConfig(const json::value& input, std::string& error)
{
    if (!input.is_object())
    {
        error = "root value must be object";
        return std::nullopt;
    }

    const json::object& obj = input.as_object();
    LessonConfig config;

    if (!ReadStringField(obj, "title", config.title, error))
    {
        return std::nullopt;
    }

    if (!ReadIntField(obj, "phase", config.phase, error))
    {
        return std::nullopt;
    }

    if (!ReadOptionalBoolField(obj, "enabled", true, config.enabled, error))
    {
        return std::nullopt;
    }

    if (!ReadStringArrayField(obj, "tags", config.tags, error))
    {
        return std::nullopt;
    }

    return config;
}

void RunCase(const char* name, const std::string& source)
{
    std::cout << "\n[" << name << "]\n";

    boost::system::error_code ec;
    const json::value parsed = json::parse(source, ec);
    if (ec)
    {
        std::cout << "parse error: " << ec.message() << "\n";
        return;
    }

    std::string validationError;
    const std::optional<LessonConfig> config = TryReadLessonConfig(parsed, validationError);
    if (!config)
    {
        std::cout << "validation error: " << validationError << "\n";
        return;
    }

    std::cout << "valid config\n";
    std::cout << "title = " << config->title << "\n";
    std::cout << "phase = " << config->phase << "\n";
    std::cout << "enabled = " << (config->enabled ? "true" : "false") << "\n";
    std::cout << "tags size = " << config->tags.size() << "\n";
}
}

void RunPhase06ValidationAndErrorHandling()
{
    RunCase("valid input", R"({
  "title": "Validation success",
  "phase": 6,
  "enabled": true,
  "tags": ["parse", "validate"]
})");

    RunCase("parse failure", R"({
  "title": "Broken JSON",
  "phase": 6,
)");

    RunCase("missing field", R"({
  "phase": 6,
  "enabled": true,
  "tags": ["missing-title"]
})");

    RunCase("type mismatch", R"({
  "title": "Wrong phase type",
  "phase": "6",
  "enabled": true,
  "tags": ["type-mismatch"]
})");

    RunCase("default enabled", R"({
  "title": "Default enabled",
  "phase": 6,
  "tags": ["default-value"]
})");
}
