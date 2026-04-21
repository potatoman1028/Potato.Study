#include "Phase08AsyncSink.h"

#include "BoostLogShared.h"

#include <chrono>
#include <ios>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <boost/make_shared.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
namespace sources = boost::log::sources;

namespace
{
    void RunAsyncWorker(sources::severity_logger_mt<Severity>& logger, int workerIndex)
    {
        BOOST_LOG_SEV(logger, Severity::Info)
            << "Worker " << workerIndex << " queued record 1";

        std::this_thread::sleep_for(std::chrono::milliseconds(10 * workerIndex));

        BOOST_LOG_SEV(logger, Severity::Info)
            << "Worker " << workerIndex << " queued record 2";
    }
}

// Phase 08
// file backend 앞에 asynchronous_sink frontend를 붙여서
// "record 생성"과 "backend 처리"가 분리되는 가장 작은 예제를 만든다.
// 이 단계에서 테스트하는 것은 다음 세 가지다.
// 1. async sink가 내부 queue와 dedicated thread를 사용해 record를 넘기는지
// 2. file backend도 직접 조립해서 async frontend에 붙일 수 있는지
// 3. producer thread의 ThreadID가 async sink를 지나도 유지되는지
void RunPhase08AsyncSink()
{
    ResetLogging();
    SetGlobalTag("Phase08");

    boost::log::add_common_attributes();

    const auto format =
        expr::stream
        << "[" << expr::attr<std::string>("Tag") << "]"
        << "[AsyncFile]"
        << "[#" << expr::attr<unsigned int>("LineID") << "] "
        << "[" << expr::attr<boost::posix_time::ptime>("TimeStamp") << "] "
        << "[" << expr::attr<Severity>("Severity") << "] "
        << "[" << expr::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID") << "] "
        << expr::smessage;

    auto backend = boost::make_shared<sinks::text_file_backend>(
        keywords::file_name = "logs/phase8_async.log",
        keywords::open_mode = (std::ios_base::trunc | std::ios_base::out),
        keywords::auto_flush = true,
        keywords::enable_final_rotation = false);

    auto sink = boost::make_shared<sinks::asynchronous_sink<sinks::text_file_backend>>(backend);
    sink->set_formatter(format);

    boost::log::core::get()->add_sink(sink);

    sources::severity_logger_mt<Severity> logger;
    std::vector<std::thread> workers;

    BOOST_LOG_SEV(logger, Severity::Trace)
        << "Phase 08 started on the main thread";

    for (int workerIndex = 1; workerIndex <= 2; ++workerIndex)
    {
        workers.emplace_back(
            [&logger, workerIndex]()
            {
                RunAsyncWorker(logger, workerIndex);
            });
    }

    BOOST_LOG_SEV(logger, Severity::Info)
        << "Main thread queued record 1";
    BOOST_LOG_SEV(logger, Severity::Info)
        << "Main thread queued record 2";

    for (auto& worker : workers)
    {
        worker.join();
    }

    BOOST_LOG_SEV(logger, Severity::Warning)
        << "Flush will wait until queued records from every producer thread reach the backend";

    std::cout
        << "Queued records to the asynchronous sink. Calling flush()..."
        << std::endl;

    sink->flush();

    std::cout
        << "Flush completed. The file backend has processed queued records."
        << std::endl;

    PrintLogFileLocation("logs/phase8_async.log");
}
