#include "Phase04SeverityFilter.h"

#include "BoostLogShared.h"

#include <iostream>
#include <string>

#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sources = boost::log::sources;

// Phase 04
// Phase 03의 sink 구성 위에 severity filter를 한 단계만 더 붙여서
// "같은 record라도 sink마다 통과 여부가 달라질 수 있다"는 점을 확인한다.
// 이 단계에서 테스트하는 것은 다음 세 가지다.
// 1. console sink는 모든 severity를 그대로 출력하는지
// 2. file sink는 Warning 이상만 남기도록 제한할 수 있는지
// 3. filter는 formatter와 별도로 sink 동작을 제어하는지
void RunPhase04SeverityFilter()
{
    ResetLogging();
    SetGlobalTag("Phase04");

    boost::log::add_common_attributes();

    const auto console_format =
        expr::stream
        << "[" << expr::attr<std::string>("Tag") << "]"
        << "[Console]"
        << "[#" << expr::attr<unsigned int>("LineID") << "] "
        << "[" << expr::attr<boost::posix_time::ptime>("TimeStamp") << "] "
        << "[" << expr::attr<Severity>("Severity") << "] "
        << "[" << expr::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID") << "] "
        << expr::smessage;

    const auto file_format =
        expr::stream
        << "[" << expr::attr<std::string>("Tag") << "]"
        << "[File][WarningOrHigher]"
        << "[#" << expr::attr<unsigned int>("LineID") << "] "
        << "[" << expr::attr<boost::posix_time::ptime>("TimeStamp") << "] "
        << "[" << expr::attr<Severity>("Severity") << "] "
        << "[" << expr::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID") << "] "
        << expr::smessage;

    const auto warning_or_higher =
        expr::attr<Severity>("Severity") >= Severity::Warning;

    boost::log::add_console_log(
        std::cout,
        keywords::format = console_format);

    boost::log::add_file_log(
        keywords::file_name = "logs/phase4_warning_or_higher.log",
        keywords::open_mode = std::ios_base::app,
        keywords::auto_flush = true,
        keywords::filter = warning_or_higher,
        keywords::format = file_format);

    sources::severity_logger<Severity> logger;

    BOOST_LOG_SEV(logger, Severity::Trace) << "Phase 04 started";
    BOOST_LOG_SEV(logger, Severity::Info) << "Console sink still receives every record";
    BOOST_LOG_SEV(logger, Severity::Warning) << "File sink starts recording from Warning";
    BOOST_LOG_SEV(logger, Severity::Error) << "File sink also records Error";

    PrintLogFileLocation("logs/phase4_warning_or_higher.log");
}
