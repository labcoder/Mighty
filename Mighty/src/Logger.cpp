#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

#include "Logger.h"

std::vector<LogMessage> Logger::messages;

std::string getTimeAsString() {
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string buffer(30, '\0');
	std::strftime(&buffer[0], 30, "%D %r", std::localtime(&now));
	return buffer;
}

void Logger::Log(const std::string& message) {
	LogMessage logMessage;
	logMessage.type = LOG_INFO;
	logMessage.message = "LOG: <" + getTimeAsString() + "> | " + message;
	std::cout << "\x1B[32m" << logMessage.message << "\033[0m" << std::endl;

	messages.push_back(logMessage);
}

void Logger::Err(const std::string& message) {
	LogMessage logMessage;
	logMessage.type = LOG_ERROR;
	logMessage.message = "ERR: <" + getTimeAsString() + "> | " + message;
	std::cout << "\x1B[91m" << logMessage.message << "\033[0m" << std::endl;

	messages.push_back(logMessage);
}
