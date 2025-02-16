#include "Logger.h"

#define DEFAULT_COLOR "\033[0m"
#define ERROR_COLOR "\033[0;91m"
#define WARNING_COLOR "\033[0;33m"
#define INFORMATION_COLOR "\033[0;92m"

// RealLogger

unsigned short default_level = 1, default_amount = 30;

Logger::RealLogger* Logger::RealLogger::instance = nullptr;

unsigned short* Logger::RealLogger::level = nullptr;
std::string* Logger::RealLogger::output_path = nullptr;
std::ofstream* Logger::RealLogger::file = nullptr;
std::mutex* Logger::RealLogger::mutex = nullptr;

Logger::RealLogger::RealLogger(const unsigned short& _level,
                               const std::string& _save,
                               const unsigned int& amount) {
  level = new unsigned short{_level};
  output_path = new std::string{_save};
  mutex = new std::mutex;

  std::string log_dir{"/Logs"};

  if (std::filesystem::is_directory(*output_path)) {
    log_dir = *output_path + "/Logs";
  }

  unsigned int count =
      std::distance(std::filesystem::directory_iterator{log_dir}, {});

  if (count >= amount) {
    for (auto& file : std::filesystem::directory_iterator(log_dir)) {
      std::filesystem::remove(file);

      count = std::distance(std::filesystem::directory_iterator{log_dir}, {});
      if (count < amount)
        break;
    }
  }

  std::string buff_name = log_dir + "log_" +
                          std::format("{:%d-%m-%y-%H_%M_%S}",
                                      std::chrono::current_zone()->to_local(
                                          std::chrono::system_clock::now())) +
                          ".txt";
  file = new std::ofstream{buff_name};
}

Logger::RealLogger* Logger::RealLogger::get_instance() {
  return get_instance(default_level, "", default_amount);
}

Logger::RealLogger* Logger::RealLogger::get_instance(
    const unsigned short& level, const std::string& path,
    const unsigned int& amount) {
  if (instance == nullptr) {
    instance = new Logger::RealLogger{level, path, amount};
  }
  return instance;
}

void Logger::RealLogger::destroy() {
  if (instance != nullptr) {
    delete level;
    delete output_path;
    delete file;
    delete mutex;

    delete instance;
    instance = nullptr;
  }
}

void Logger::RealLogger::real_save(const std::string& str,
                                   const MessageTypes& type,
                                   const std::source_location& location) {
  std::string time = std::format(
      " [{:%d-%m-%y-%H_%M_%S}]",
      std::chrono::current_zone()->to_local(std::chrono::system_clock::now()));

  std::string message_type;
  switch (type) {
    case ERROR:
      message_type = "[ERROR]";
      break;
    case WARNING:
      message_type = "[WARNING]";
      break;
    case INFORMATION:
      message_type = "[INFORMATION]";
      break;
  }

  std::string level_str{" [" + std::to_string(*level) + "]"};

  std::string func_name{"["};
  func_name += location.file_name();
  func_name += "]";

  {
    std::lock_guard<std::mutex> lock{*mutex};
    std::cout << "[" << std::this_thread::get_id() << "]" << time << " ";
    switch (type) {
      case ERROR:
        std::cout << ERROR_COLOR;
        break;
      case WARNING:
        std::cout << WARNING_COLOR;
        break;
      case INFORMATION:
        std::cout << INFORMATION_COLOR;
        break;
    }

    std::cout << message_type << DEFAULT_COLOR " " << level_str << " " << func_name << " " << str
              << "\n";

    *file << "[" << std::this_thread::get_id() << "]" << time << " "
          << message_type << " " << level_str << " " << func_name << " " << str
          << "\n";
  }
}

void Logger::RealLogger::real_set_level(const unsigned short& _level) {
  *level = _level;
}
unsigned short Logger::RealLogger::real_get_level() const {
  return *level;
}

// Logger

Logger::Logger()
    : buff{},
      real{Logger::RealLogger::get_instance()} {};

Logger::Logger() {};

bool Logger::init() {
  return init(default_level, "", default_amount);
}
bool Logger::init(const unsigned short& level) {
  return init(level, "", default_amount);
}
bool Logger::init(const std::string& save_path) {
  return init(default_level, save_path, default_amount);
}
bool Logger::init(const unsigned int& amount) {
  return init(default_level, "", amount);
}
bool Logger::init(const unsigned short& level, const std::string& save_path,
                  const unsigned int& amount) {
  Logger::RealLogger* buff =
      Logger::RealLogger::get_instance(level, save_path, amount);
  return buff != nullptr;
}

bool Logger::destroy() {
  Logger::RealLogger* buff = Logger::RealLogger::get_instance();
  Logger::RealLogger::destroy();
  return buff == nullptr;
}
