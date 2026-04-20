#include "BoostLogShared.h"

#include <iostream>

#include <boost/log/core.hpp>

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

void PrintLogFileLocation(std::string_view relativePath)
{
    std::cout << "Log file written to " << relativePath << std::endl;
}
