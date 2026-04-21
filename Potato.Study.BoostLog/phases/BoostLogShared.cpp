#include "BoostLogShared.h"

#include <iostream>
#include <string>

#include <boost/log/attributes/constant.hpp>
#include <boost/log/core.hpp>

namespace
{
    struct GlobalTagRegistration
    {
        boost::log::attribute_set::iterator iterator;
        bool isRegistered = false;
    };

    GlobalTagRegistration& GetGlobalTagRegistration()
    {
        static GlobalTagRegistration registration;
        return registration;
    }
}

std::ostream& operator<<(std::ostream& os, Severity severity)
{
    switch (severity)
    {
    case Severity::Trace:
        return os << "Trace";
    case Severity::Info:
        return os << "Info";
    case Severity::Warning:
        return os << "Warning";
    case Severity::Error:
        return os << "Error";
    default:
        return os << "Unknown";
    }
}

void ResetLogging()
{
    boost::log::core::get()->remove_all_sinks();
}

void SetGlobalTag(std::string_view tag)
{
    ClearGlobalTag();

    auto result = boost::log::core::get()->add_global_attribute(
        "Tag",
        boost::log::attributes::constant<std::string>(std::string(tag)));

    auto& registration = GetGlobalTagRegistration();
    registration.iterator = result.first;
    registration.isRegistered = true;
}

void ClearGlobalTag()
{
    auto& registration = GetGlobalTagRegistration();

    if (!registration.isRegistered)
    {
        return;
    }

    boost::log::core::get()->remove_global_attribute(registration.iterator);
    registration.isRegistered = false;
}

void PrintLogFileLocation(std::string_view relativePath)
{
    std::cout << "Log file written to " << relativePath << std::endl;
}
