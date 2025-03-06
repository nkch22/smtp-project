#pragma once
/**
 * @brief 
 * 
 */

namespace SMTP
{

class IServer
{
public:
    /**
     * @brief Destroy the IServer object
     * 
     */
    virtual ~IServer() = default;

    /**
     * @brief 
     * 
     */
    virtual void Start() = 0;

    /**
     * @brief 
     * 
     */
    virtual void Stop() = 0;

    /**
     * @brief 
     * 
     */
    virtual void Restart() = 0;

    /**
     * @brief 
     * 
     */
    virtual void Accept() = 0;

    /**
     * @brief 
     * 
     * @param data 
     * @return true 
     * @return false 
     */
    virtual bool Multicast(const std::string_view data) = 0;

    /**
     * @brief 
     * 
     */
    virtual void OnStarted() = 0;

    /**
     * @brief 
     * 
     */
    virtual void OnStopped() = 0;

    /**
     * @brief 
     * 
     */
    virtual void OnRestarted() = 0;

    /**
     * @brief 
     * 
     */
    virtual void OnAccepted() = 0;
};

}