#include "Phase06RotationFile.h"

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

// Phase 06
// file sink에 rotation_size와 파일명 패턴을 붙여서 로그가 일정 크기를 넘으면
// 다음 파일로 넘어가는 가장 작은 예제를 만든다.
// 이 단계에서 테스트하는 것은 다음 세 가지다.
// 1. %N 패턴이 회전 파일 번호로 바뀌는지
// 2. rotation_size를 넘기면 여러 파일이 생기는지
// 3. formatter는 그대로 두고 파일 수만 늘어나는지
void RunPhase06RotationFile()
{
    ResetLogging();
    SetGlobalTag("Phase06");

    boost::log::add_common_attributes();

    const auto format =
        expr::stream
        << "[" << expr::attr<std::string>("Tag") << "]"
        << "[#" << expr::attr<unsigned int>("LineID") << "] "
        << "[" << expr::attr<boost::posix_time::ptime>("TimeStamp") << "] "
        << "[" << expr::attr<Severity>("Severity") << "] "
        << "[" << expr::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID") << "] "
        << expr::smessage;

    boost::log::add_console_log(std::cout, keywords::format = format);

    boost::log::add_file_log(
        keywords::file_name = "logs/phase6_rotation_%3N.log",
        keywords::rotation_size = 1000,
        keywords::auto_flush = true,
        keywords::format = format);

    sources::severity_logger<Severity> logger;
    const std::string payload(100, 'x');

    BOOST_LOG_SEV(logger, Severity::Trace) << "Phase 06 started";

    for (int index = 1; index <= 12; ++index)
    {
        BOOST_LOG_SEV(logger, Severity::Info)
            << "Rotation sample " << index << " payload=" << payload;
    }

    std::cout
        << "Rotated files written using pattern logs/phase6_rotation_%3N.log"
        << std::endl;
}
