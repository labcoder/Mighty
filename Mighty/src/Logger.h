#pragma once

#include <string>
#include <vector>

enum LogType {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

struct LogMessage {
	LogType type;
	std::string message;
};

class Logger {
public:
	static std::vector<LogMessage> messages;
	static void Log(const std::string& message);
	static void Err(const std::string& message);
};

