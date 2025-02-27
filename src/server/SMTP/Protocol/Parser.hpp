#pragma once

#include <optional>
#include <memory>
#include <mutex>
#include <vector>

#include "Options.hpp"
#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class Parser
{
public:
    Parser(const Options global_options);
    OptionalCommand TryParseRequest(const std::string& request) const;
    OptionalCommand TryParseRequest(const std::string& request, const Options& custom_options) const;
    const Options& get_global_options() const noexcept;
    void set_global_options(const Options& options);
private:
    OptionalCommand TryParseHelo(const std::string& request, const Options& custom_options) const;
    OptionalCommand TryParseEhlo(const std::string& request, const Options& custom_options) const;
    OptionalCommand TryParseData(const std::string& request, const Options& custom_options) const;
    OptionalCommand TryParseAuth(const std::string& request, const Options& custom_options) const;
    OptionalCommand TryParseMailFrom(const std::string& request, const Options& custom_options) const;
    OptionalCommand TryParseNoop(const std::string& request, const Options& custom_options) const;
    OptionalCommand TryParseQuit(const std::string& request, const Options& custom_options) const;
    OptionalCommand TryParseRcptTo(const std::string& request, const Options& custom_options) const;
    OptionalCommand TryParseRset(const std::string& request, const Options& custom_options) const;
    OptionalCommand TryParseVrfy(const std::string& request, const Options& custom_options) const;
    OptionalCommand TryParseHelp(const std::string& request, const Options& custom_options) const;

    static std::string ToUpper(const std::string& string);
    static std::string RemoveWhitespaces(const std::string& string);
    static std::vector<std::string> Split(const std::string& string);

    std::mutex m_global_options_mutex;
    Options m_global_options;
};

}

}