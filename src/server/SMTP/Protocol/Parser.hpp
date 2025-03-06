#pragma once
/**
 * @file Parser.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

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

/**
 * @brief Class that parses SMTP requests
 * 
 */
class Parser
{
public:
    Parser(const Options global_options);

    /**
     * @brief 
     * 
     * @param request 
     * @return OptionalCommand 
     */
    OptionalCommand TryParseRequest(const std::string& request) const;

    /**
     * @brief 
     * 
     * @param request 
     * @param custom_options 
     * @return OptionalCommand 
     */
    OptionalCommand TryParseRequest(const std::string& request, const Options& custom_options) const;

    /**
     * @brief Get the global options object
     * 
     * @return const Options& 
     */
    const Options& get_global_options() const noexcept;

    /**
     * @brief Set the global options object
     * 
     * @param options 
     */
    void set_global_options(const Options& options);
private:

    /**
     * @brief 
     * 
     * @param string 
     * @return std::string 
     */
    static std::string ToUpper(const std::string& string);

    std::mutex m_global_options_mutex;
    Options m_global_options;
};

}

}