#include "PerfProfiler.h"
#include <chrono>
#include <algorithm>
#include <thread>



PerProfiler& PerProfiler::Instance()
{
    static PerProfiler instance;
    return instance;
}

PerProfiler::~PerProfiler()
{
    endSession();
}

void PerProfiler::beginSession(const std::string& name, const std::string& filepath)
{
    if (m_activeSession) { endSession(); }
    m_activeSession = true;
    m_outputStream.open(filepath);
    writeHeader();
    m_sessionName = name;
}

void PerProfiler::endSession()
{
    if (!m_activeSession) { return; }
    m_activeSession = false;
    writeFooter();
    m_outputStream.close();
    m_profileCount = 0;
}


void PerProfiler::writeHeader()
{
    m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
}


void PerProfiler::writeProfile(const ProfileResult& result)
{
    std::lock_guard<std::mutex> lock(m_lock);

    if (m_profileCount++ > 0) { m_outputStream << ","; }

    std::string name = result.name;
    std::replace(name.begin(), name.end(), '"', '\'');

    std::string cat = result.category;
    std::replace(cat.begin(), cat.end(), '"', '\'');

    m_outputStream << "{";
    m_outputStream << "\"cat\":\"" << cat << "\",";
    m_outputStream << "\"dur\":" << (result.end - result.start) << ',';
    m_outputStream << "\"name\":\"" << name << "\",";
    m_outputStream << "\"ph\":\"X\",";
    m_outputStream << "\"pid\":0,";
    m_outputStream << "\"tid\":" << result.threadID << ",";
    m_outputStream << "\"ts\":" << result.start;
    m_outputStream << "}";
}

void PerProfiler::writeFooter()
{
    m_outputStream << "]}";
}

PerfTimer::PerfTimer(const std::string& name, const std::string& cat )
    : m_result({ name, 0, 0, 0, cat })
    , m_stopped(false)
    , m_startTimepoint(std::chrono::high_resolution_clock::now())
{
}

PerfTimer::~PerfTimer()
{
    if (!m_stopped) 
    { 
        stop();
    }
}

long long PerfTimer::getTime()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    return std::chrono::time_point_cast<std::chrono::microseconds>(currentTime).time_since_epoch().count() -
        std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
}

void PerfTimer::stop()
{
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    m_result.start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
    m_result.end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
    m_result.threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

#if PROFILE_OUTPUTDEBUG 1
    std::ostringstream oss;
    oss << name << ": " << "#### Time taken - " << << m_result.end - m_result.start << "us\n\n";
    OutputDebugString(oss.str().c_str()); 
#elif PROFILE_CONSOLE 1
    std::cout << "Time taken - " << m_result.name << ":" << m_result.end - m_result.start << "us\n\n";
#endif
    PerProfiler::Instance().writeProfile(m_result);
    m_stopped = true;
}