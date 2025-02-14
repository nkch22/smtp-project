#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <source_location>
#include <string>
#include <thread>
#include <vector>

#define DECL __declspec(dllexport)

#define LOG_START_ARGS(...) \
  Logger log;               \
  log.save_arguments(__VA_ARGS__)

#define LOG_START \
  Logger log;     \
  log.save_func_start()

#define LOG_RETURN(value) \
  log.save_return(value); \
  return value

enum LogLevels {
  LOG_LEVEL_NO,
  LOG_LEVEL_PROD,
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_TRACE
};

class DECL Buffer {
 private:
  std::string* real_buff;

 public:
  Buffer();
  Buffer(const std::string&);

  ~Buffer();

  std::string get() const;

  void operator<<(const std::string&);

  void operator<<(const int&);
  void operator<<(const unsigned int&);
  void operator<<(const double&);
  void operator<<(const bool&);

  void operator<<(const void*);

  template <typename T>
  void operator<<(const T&);
};

class DECL Logger {
 private:
  class RealLogger {
   private:
    enum MessageTypes { ERROR, WARNING, COMMON };

    static RealLogger* instance;

    static unsigned short* level;
    static std::string* output_path;
    static std::ofstream* file;
    static std::mutex* mutex;

    RealLogger(const unsigned short&, const std::string&);

    ~RealLogger() = default;

   public:
    static RealLogger* get_instance();
    static RealLogger* get_instance(const unsigned short&, const std::string&);

    static void destroy();

    void real_save(const Buffer&, const MessageTypes&,
                   const std::source_location&);

    void real_set_level(const unsigned short&);
    unsigned short real_get_level() const;
  };

  RealLogger* real;

  Buffer buff;

  template <typename T>
  void save_argument(const T&);

  void save_arguments();

 public:
  Logger();
  ~Logger();

  static bool init();
  static bool init(const unsigned short& level);
  static bool init(const std::string& save_path);
  static bool init(const unsigned short& level, const std::string& save_path);

  static bool destroy();

  void save_error(const std::string&,
                  const std::source_location location =
                      std::source_location::current());
  void save_warning(const std::string&,
                    const std::source_location location =
                        std::source_location::current());
  void save_message(const std::string&,
                    const std::source_location location =
                        std::source_location::current());

  void set_level(const unsigned short&);
  unsigned short get_level() const;

  template <typename T>
  void save_return(const T&, const std::source_location location =
                                   std::source_location::current());

  template <typename T, typename... Args>
  void save_arguments(
      const T&&, Args&&... args,
      const std::source_location location = std::source_location::current());

  void save_func_start(const std::source_location location =
                           std::source_location::current());
};