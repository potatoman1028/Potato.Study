#include "Phase05CustomAttribute.h"

#include "BoostLogShared.h"

#include <iostream>
#include <string>

#include <boost/log/attributes/constant.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sources = boost::log::sources;

// Phase 05
// 요청 ID 같은 custom attribute를 로그 record에 붙여서 formatter에서 읽는
// 가장 작은 예제를 만든다. scoped thread attribute를 사용해 현재 스코프 안의
// 로그에만 RequestID가 보이도록 구성한다.
// 이 단계에서 테스트하는 것은 다음 세 가지다.
// 1. custom attribute를 formatter에서 기본 attribute와 함께 출력할 수 있는지
// 2. scoped thread attribute가 스코프 안에서만 적용되는지
// 3. attribute가 없을 때 fallback 값을 둘 수 있는지
void RunPhase05CustomAttribute()
{
    ResetLogging();
    SetGlobalTag("Phase05");

    boost::log::add_common_attributes();

    const auto format =
        expr::stream
        << "[" << expr::attr<std::string>("Tag") << "] "
        << "[#" << expr::attr<unsigned int>("LineID") << "] "
        << "[" << expr::attr<boost::posix_time::ptime>("TimeStamp") << "] "
        << "[" << expr::attr<Severity>("Severity") << "] "
        << "[RequestID="
        << expr::attr<std::string>("RequestID").or_default(std::string("none"))
        << "] "
        << "[" << expr::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID") << "] "
        << expr::smessage;

    boost::log::add_console_log(std::cout, keywords::format = format);

    sources::severity_logger<Severity> logger;

    BOOST_LOG_SEV(logger, Severity::Trace) << "Phase 05 started";
    BOOST_LOG_SEV(logger, Severity::Info) << "No request context is attached yet";

    {
        BOOST_LOG_SCOPED_THREAD_ATTR("RequestID", attrs::constant<std::string>("REQ-1001"));

        BOOST_LOG_SEV(logger, Severity::Info) << "Request context is active";
        BOOST_LOG_SEV(logger, Severity::Warning) << "Custom attribute is printed with the log";
    }

    BOOST_LOG_SEV(logger, Severity::Info) << "Request context ended";
}
