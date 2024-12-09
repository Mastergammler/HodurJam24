#include "module.h"

#include "logutils.cpp"

Logger logger = {};

void InitLogger(Logger& logger, const string& log_filePath)
{
    logger.log_file.open(log_filePath, ios::out | ios::app);
    logger.running = true;
    logger.log_thread = thread([&logger]() {
        while (logger.running)
        {
            vector<string> logsToWrite;
            {
                unique_lock<mutex> lock(logger.log_mutex);
                logger.log_condition.wait(lock, [&logger]() {
                    return !logger.log_buffer.empty() || !logger.running;
                });
                logsToWrite = std::move(logger.log_buffer);
                logger.log_buffer.clear();
            }
            for (const string& log : logsToWrite)
            {
                logger.log_file << log << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    });
}

string GetTimeString()
{
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);

    tm time_info;
    localtime_s(&time_info, &now_c);

    ostringstream oss;
    oss << put_time(&time_info, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// TODO: write to date based file
//  write into log directory
void InitLogger()
{
    InitOutputStreams();
    InitLogger(logger, "logs/log.txt");
    Logf("--- %s ---", GetTimeString().c_str());
}

void Log(const string s)
{
    Logf(s, "");
}

void Debug(string msg)
{
    // OutputDebugStringA(s.c_str());
    cout << "[Debug] " << msg << endl;
}

// TODO: can i use this for the log message also?
string format(const string message, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf_s(buffer, message.c_str(), args);
    va_end(args);

    string formatted(buffer);
    return formatted;
}

void Logf(const string message, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf_s(buffer, message.c_str(), args);
    va_end(args);

    string formatted(buffer);
    Debug(formatted);
    {
        unique_lock<mutex> lock(logger.log_mutex);
        logger.log_buffer.push_back(formatted);
    }
    logger.log_condition.notify_one();
}

void StopLogger(Logger& logger)
{
    logger.running = false;
    logger.log_condition.notify_one();
    logger.log_thread.join();
    logger.log_file.close();
}

void DisposeLogger()
{
    StopLogger(logger);
}
