#include "Phase01Basic.h"

#include "BoostLogShared.h"

#include <iostream>

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace sources = boost::log::sources;

// Phase 01
// Boost.Log tutorial의 "Creating loggers and writing logs"에 맞춰
// severity_logger와 BOOST_LOG_SEV로 최소 로그 기록 흐름을 확인한다.
// 이 단계에서 테스트하는 것은 다음 두 가지다.
// 1. severity_logger<Severity>가 source-specific "Severity" attribute를 제공하는지
// 2. 별도 formatter 없이도 메시지가 콘솔 sink로 바로 출력되는지
void RunPhase01Basic()
{
    ResetLogging();
    boost::log::add_console_log(std::cout);

    sources::severity_logger<Severity> logger;

    BOOST_LOG_SEV(logger, Severity::Trace) << "Phase 01 started";
    BOOST_LOG_SEV(logger, Severity::Info) << "Basic severity logger is active";
    BOOST_LOG_SEV(logger, Severity::Warning) << "Phase 01 shows the minimum working example";
}
