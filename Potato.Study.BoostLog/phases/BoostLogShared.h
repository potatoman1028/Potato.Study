#pragma once

#include <iosfwd>
#include <string_view>

enum class Severity
{
    Trace,
    Info,
    Warning,
    Error
};

std::ostream& operator<<(std::ostream& os, Severity severity);

void ResetLogging();
void SetGlobalTag(std::string_view tag);
void ClearGlobalTag();
void PrintLogFileLocation(std::string_view relativePath);
