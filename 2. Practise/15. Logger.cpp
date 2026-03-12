#include <iostream>
#include <string>
using namespace std;

/*
    ============================
        1) Logger Level Enum
    ============================
*/
enum class LoggerLevel {
    INFO = 1,
    DEBUG = 2,
    WARNING = 3,
    ERROR = 4
};

/*
    helper: enum ko string me convert
*/
string levelToString(LoggerLevel level) {
    if (level == LoggerLevel::INFO) return "INFO";
    if (level == LoggerLevel::DEBUG) return "DEBUG";
    if (level == LoggerLevel::WARNING) return "WARNING";
    return "ERROR";
}

/*
    ============================
        2) Logger Base Class
    ============================
*/
class Logger {
protected:
    Logger* nextLogger;
    LoggerLevel myLevel;

public:
    Logger(LoggerLevel level) {
        this->myLevel = level;
        this->nextLogger = nullptr;
    }

    void setNextLogger(Logger* next) {
        this->nextLogger = next;
    }

    // log method: handle or pass forward
    void log(LoggerLevel level, string message) {
        // agar ye logger ka level match ho gaya => print
        if (level == myLevel) {
            cout << "[" << levelToString(level) << "] " << message << "\n";
            return;
        }

        // warna next ko pass karo
        if (nextLogger != nullptr) {
            nextLogger->log(level, message);
        }
    }
};

/*
    ============================
        3) Logger Manager (Filter)
    ============================
    Ye chain + min level manage karega
*/
class LoggerManager {
private:
    LoggerLevel minLevel;
    Logger* headLogger;

public:
    LoggerManager() {
        // default min level INFO
        minLevel = LoggerLevel::INFO;

        // create chain
        Logger* infoLogger = new Logger(LoggerLevel::INFO);
        Logger* debugLogger = new Logger(LoggerLevel::DEBUG);
        Logger* warningLogger = new Logger(LoggerLevel::WARNING);
        Logger* errorLogger = new Logger(LoggerLevel::ERROR);

        infoLogger->setNextLogger(debugLogger);
        debugLogger->setNextLogger(warningLogger);
        warningLogger->setNextLogger(errorLogger);

        headLogger = infoLogger;
    }

    void setLogLevel(LoggerLevel level) {
        minLevel = level;
    }

    void log(LoggerLevel level, string message) {
        // ✅ FILTER: agar requested log level < minLevel => ignore
        if ((int)level < (int)minLevel) {
            return;
        }

        headLogger->log(level, message);
    }
};

/*
    ============================
        4) Main
    ============================
*/
int main() {
    LoggerManager logger;

    cout << "== Default Level = INFO ==\n";
    logger.log(LoggerLevel::INFO, "This is info");
    logger.log(LoggerLevel::DEBUG, "This is debug");
    logger.log(LoggerLevel::WARNING, "This is warning");
    logger.log(LoggerLevel::ERROR, "This is error");

    cout << "\n== Set Level = WARNING ==\n";
    logger.setLogLevel(LoggerLevel::WARNING);

    logger.log(LoggerLevel::INFO, "This should NOT print");
    logger.log(LoggerLevel::DEBUG, "This should NOT print");
    logger.log(LoggerLevel::WARNING, "This should print");
    logger.log(LoggerLevel::ERROR, "This should print");

    return 0;
}
