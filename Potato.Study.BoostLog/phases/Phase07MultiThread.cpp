#include "Phase07MultiThread.h"

#include "BoostLogShared.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sources = boost::log::sources;

namespace
{
    void RunWorker(sources::severity_logger_mt<Severity>& logger, int workerIndex)
    {
        BOOST_LOG_SEV(logger, Severity::Info)
            << "Worker " << workerIndex << " started";

        for (int step = 1; step <= 4; ++step)
        {
            BOOST_LOG_SEV(logger, Severity::Info)
                << "Worker " << workerIndex << " step " << step;

            std::this_thread::sleep_for(
                std::chrono::milliseconds(10 * workerIndex));
        }

        BOOST_LOG_SEV(logger, Severity::Warning)
            << "Worker " << workerIndex << " finished";
    }
}

// Phase 07
// 여러 스레드가 같은 logger와 console sink로 동시에 로그를 남길 때
// ThreadID와 출력 순서가 어떻게 보이는지 확인하는 가장 작은 예제를 만든다.
// 이 단계에서 테스트하는 것은 다음 세 가지다.
// 1. severity_logger_mt를 여러 스레드가 공유해서 써도 되는지
// 2. 각 로그 레코드에 서로 다른 ThreadID가 찍히는지
// 3. 로그가 스레드별로 묶이지 않고 자연스럽게 섞여 보이는지
void RunPhase07MultiThread()
{
    ResetLogging();
    SetGlobalTag("Phase07");

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

    sources::severity_logger_mt<Severity> logger;
    std::vector<std::thread> workers;

    BOOST_LOG_SEV(logger, Severity::Trace)
        << "Phase 07 started on the main thread";

    for (int workerIndex = 1; workerIndex <= 5; ++workerIndex)
    {
        workers.emplace_back(
            [&logger, workerIndex]()
            {
                RunWorker(logger, workerIndex);
            });
    }

    for (auto& worker : workers)
    {
        worker.join();
    }

    BOOST_LOG_SEV(logger, Severity::Info)
        << "All worker threads joined";
}
