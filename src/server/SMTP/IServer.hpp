#pragma once

namespace SMTP
{

class IServer
{
public:
    virtual ~IServer() = default;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Restart() = 0;
    virtual void Accept() = 0;

    virtual bool IsStarted() const = 0;

    virtual void OnStarted() = 0;
    virtual void OnStopped() = 0;
    virtual void OnRestarted() = 0;
    virtual void OnAccepted() = 0;
};

}