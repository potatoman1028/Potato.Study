#include "Phase03BuildJsonProgrammatically.h"

#include <iostream>

#include <boost/json.hpp>

namespace json = boost::json;

void RunPhase03BuildJsonProgrammatically()
{
    json::object profile;
    profile["name"] = "potato";
    profile["age"] = 3;
    profile["active"] = true;
    profile["nickname"] = "king";

    json::array features;
    features.push_back("build-object");
    features.push_back("build-array");
    features.push_back("serialize");
    features.push_back("striiiiing");

    json::array checkpoints;

    json::object firstCheckpoint;
    firstCheckpoint["step"] = "create root object";
    firstCheckpoint["done"] = true;
    checkpoints.push_back(std::move(firstCheckpoint));

    json::object secondCheckpoint;
    secondCheckpoint["step"] = "append nested array";
    secondCheckpoint["done"] = true;

    json::object secondCheckpoint2;
    secondCheckpoint2["step"] = "append nested array";
    secondCheckpoint2["done"] = true;

    checkpoints.push_back(std::move(secondCheckpoint));
    checkpoints.push_back(std::move(secondCheckpoint2));

    json::object root;
    root["project"] = "Potato.Study.BoostJson";
    root["phase"] = 3;
    root["profile"] = std::move(profile);
    root["features"] = std::move(features);
    root["checkpoints"] = std::move(checkpoints);
    root["enabled"] = false;

    const json::value document = std::move(root);
    const json::object& built = document.as_object();
    const json::array& builtFeatures = built.at("features").as_array();
    const json::array& builtCheckpoints = built.at("checkpoints").as_array();

    std::cout << "Built root kind: object\n";
    std::cout << "project = " << built.at("project").as_string().c_str() << "\n";
    std::cout << "phase = " << built.at("phase").as_int64() << "\n";
    std::cout << "profile.name = "
              << built.at("profile").as_object().at("name").as_string().c_str() << "\n";
    std::cout << "features size = " << builtFeatures.size() << "\n";
    std::cout << "first feature = " << builtFeatures.at(0).as_string().c_str() << "\n";
    std::cout << "checkpoints size = " << builtCheckpoints.size() << "\n";
    std::cout << "first checkpoint step = "
              << builtCheckpoints.at(0).as_object().at("step").as_string().c_str() << "\n\n";
    std::cout << "Serialized JSON text:\n" << json::serialize(document) << "\n";
}
