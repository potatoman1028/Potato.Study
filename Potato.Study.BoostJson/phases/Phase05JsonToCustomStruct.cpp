#include "Phase05JsonToCustomStruct.h"

#include <iostream>
#include <string>
#include <vector>

#include <boost/json.hpp>

namespace json = boost::json;

namespace study
{
struct Author
{
    std::string name;
    bool active;
};

struct LessonPlan
{
    std::string title;
    int phase;
    Author author;
    std::vector<std::string> tags;
};

Author tag_invoke(const json::value_to_tag<Author>&, const json::value& jv)
{
    const json::object& obj = jv.as_object();

    return {
        json::value_to<std::string>(obj.at("name")),
        obj.at("active").as_bool()
    };
}

LessonPlan tag_invoke(const json::value_to_tag<LessonPlan>&, const json::value& jv)
{
    const json::object& obj = jv.as_object();

    return {
        json::value_to<std::string>(obj.at("title")),
        static_cast<int>(obj.at("phase").as_int64()),
        json::value_to<Author>(obj.at("author")),
        json::value_to<std::vector<std::string>>(obj.at("tags"))
    };
}
}

void RunPhase05JsonToCustomStruct()
{
    const std::string source = R"({
  "title": "Boost.JSON value_to",
  "phase": 5,
  "author": {
    "name": "potato",
    "active": true
  },
  "tags": ["value_to", "tag_invoke", "custom-type"]
})";

    const json::value parsed = json::parse(source);
    const study::LessonPlan plan = json::value_to<study::LessonPlan>(parsed);

    std::cout << "Original JSON text:\n" << source << "\n\n";
    std::cout << "title = " << plan.title << "\n";
    std::cout << "phase = " << plan.phase << "\n";
    std::cout << "author.name = " << plan.author.name << "\n";
    std::cout << "author.active = " << (plan.author.active ? "true" : "false") << "\n";
    std::cout << "tags size = " << plan.tags.size() << "\n";

    if (!plan.tags.empty())
    {
        std::cout << "first tag = " << plan.tags.front() << "\n";
    }
}
