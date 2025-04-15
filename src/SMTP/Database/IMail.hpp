#pragma once

#include <string>

namespace SMTP
{

namespace Database
{

class IMail
{
public:
    virtual ~IMail() = default;

    virtual int GetID() const = 0;
    virtual int GetUserFromID() const = 0;
    virtual int GetUserToID() const = 0;
    virtual std::string GetMessage() const = 0;
}; 

}

}