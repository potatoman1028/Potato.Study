#include "Phase03Sinks.h"

#include "BoostLogShared.h"

#include <iostream>

#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sources = boost::log::sources;

// Phase 03
// Boost.Log tutorial의 "Setting up sinks"와 utility helper 문서의
// add_console_log / add_file_log 설명에 맞춰 sink 구성을 확인한다.
// 이 단계에서 테스트하는 것은 다음 네 가지다.
// 1. 같은 formatter를 console sink와 file sink에 함께 적용할 수 있는지
// 2. add_file_log의 file_name, open_mode, auto_flush가 의도대로 동작하는지
// 3. 하나의 로그 레코드가 여러 sink로 동시에 전달되는지
// 4. 상대 경로 로그 파일이 현재 작업 디렉터리 기준으로 생성되는지
void RunPhase03Sinks()
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
    boost::log::add_file_log(
        keywords::file_name = "logs/phase3_sample.log",
        keywords::open_mode = std::ios_base::app,
        keywords::auto_flush = true,
        keywords::format = format);

    sources::severity_logger<Severity> logger;

    BOOST_LOG_SEV(logger, Severity::Trace) << "Phase 03 started";
    BOOST_LOG_SEV(logger, Severity::Info) << "Console sink and file sink are active";
    BOOST_LOG_SEV(logger, Severity::Warning) << "This phase writes to console and file";
    BOOST_LOG_SEV(logger, Severity::Error) << "Sample error message for sink verification";

    PrintLogFileLocation("logs/phase3_sample.log");
}
