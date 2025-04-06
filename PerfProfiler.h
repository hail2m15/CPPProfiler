#pragma once
#include <string>
#include <fstream>
#include <mutex>

#define PROFILE_CONSOLE

#if PROFILE_OUTPUTDEBUG 1
#elif PROFILE_CONSOLE 1
#include <iostream>
#endif

#ifdef PROFILING 1
#define PROFILE_SCOPE(name,cat) PerfTimer timer(name,cat)
#define PROFILE_FUNCTION()  PROFILE_SCOPE(__FUNCSIG__, "Func")
#else
#define PROFILE_SCOPE(name,cat)
#define PROFILE_FUNCTION()  PROFILE_SCOPE(__FUNCSIG__, "Func")
#endif

struct ProfileResult
{
    const std::string name;
    long long start;
    long long end;
    size_t threadID;
    const std::string category;
};

class PerProfiler
{
private:
    PerProfiler() {};

public:

    static PerProfiler& Instance();
    ~PerProfiler();
    void beginSession(const std::string& name, const std::string& filepath = "results.json");
    void endSession();
    void writeHeader();
    void writeProfile(const ProfileResult& result);
    void writeFooter();
private:
    std::string     m_sessionName = "None";
    std::ofstream   m_outputStream;
    int             m_profileCount = 0;
    std::mutex      m_lock;
    bool            m_activeSession = false;
};

class PerfTimer
{
public:
    explicit PerfTimer(const std::string& name, const std::string& cat = "Func");
    ~PerfTimer();
    long long getTime();
    void stop();
private:
    ProfileResult m_result;
    bool m_stopped;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
};