#include "Phase04CustomStructToJson.h"

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

	void tag_invoke(const json::value_from_tag&, json::value& jv, const Author& author)
	{
		jv = {
			{ "name", author.name },
			{ "active", author.active }
		};
	}

	void tag_invoke(const json::value_from_tag&, json::value& jv, const LessonPlan& plan)
	{
		jv = {
			{ "title", plan.title },
			{ "phase", plan.phase },
			{ "author", json::value_from(plan.author) },
			{ "tags", json::value_from(plan.tags) }
		};
	}
}

void RunPhase04CustomStructToJson()
{
	const study::LessonPlan plan{
		"Boost.JSON custom struct",
		4,
		{ "potato", true },
		{ "tag_invoke", "value_from", "custom-type" }
	};

	const json::value document = json::value_from(plan);
	const json::object& root = document.as_object();

	std::cout << "Root kind = object\n";
	std::cout << "title = " << root.at("title").as_string().c_str() << "\n";
	std::cout << "phase = " << root.at("phase").as_int64() << "\n";
	std::cout << "author.name = "
		<< root.at("author").as_object().at("name").as_string().c_str() << "\n";
	std::cout << "tags size = " << root.at("tags").as_array().size() << "\n\n";
	std::cout << "Serialized JSON text:\n" << json::serialize(document) << "\n";
}
