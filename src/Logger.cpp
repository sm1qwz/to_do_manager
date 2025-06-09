#include "Logger.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>

Logger& Logger::instance(const std::string& logFilePath) {
    static Logger logger(logFilePath);
    return logger;
}

Logger::Logger(const std::string& logFilePath) {
    ofs_.open(logFilePath, std::ios::app);
    if (!ofs_) {
        throw std::runtime_error("Cannot open log file: " + logFilePath);
    }
}

Logger::~Logger() {
    if (ofs_.is_open()) {
        ofs_.close();
    }
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx_);
    // Текущее время в формате ISO 8601
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm buf;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&buf, &in_time_t);
#else
    localtime_r(&in_time_t, &buf);
#endif

    std::ostringstream ss;
    ss << std::put_time(&buf, "%Y-%m-%dT%H:%M:%S");
    ofs_ << "[" << ss.str() << "] " << message << "\n";
    ofs_.flush();
}
