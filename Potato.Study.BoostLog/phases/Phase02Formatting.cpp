#include "Phase02Formatting.h"

#include "BoostLogShared.h"

#include <iostream>

#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sources = boost::log::sources;

// Phase 02
// Boost.Log tutorial의 "Adding more information to log: Attributes"와
// "Log record formatting"에 맞춰 공통 attribute와 formatter를 확인한다.
// 이 단계에서 테스트하는 것은 다음 세 가지다.
// 1. add_common_attributes() 후 LineID, TimeStamp, ThreadID를 사용할 수 있는지
// 2. Severity attribute를 formatter에서 함께 출력할 수 있는지
// 3. boost::posix_time::ptime를 포맷에 쓰기 위해 date_time 지원이 필요한지
void RunPhase02Formatting()
{
    ResetLogging();
    boost::log::add_common_attributes();

    const auto format =
        expr::stream
        << "[#" << expr::attr<unsigned int>("LineID") << "] "
        << "[" << expr::attr<boost::posix_time::ptime>("TimeStamp") << "] "
        << "[" << expr::attr<Severity>("Severity") << "] "
        << "[" << expr::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID") << "] "
        << expr::smessage;

    boost::log::add_console_log(std::cout, keywords::format = format);

    sources::severity_logger<Severity> logger;

    BOOST_LOG_SEV(logger, Severity::Trace) << "Phase 02 started";
    BOOST_LOG_SEV(logger, Severity::Info) << "Formatting and common attributes are active";
    BOOST_LOG_SEV(logger, Severity::Warning) << "ThreadID, TimeStamp, and LineID are printed";
}
