#pragma once

#include <vector>
#include <string>

namespace SMTP
{

namespace Database
{

class IUser
{
public:
    virtual ~IUser() = default;

    virtual int GetID() const = 0;
    virtual std::string GetName() const = 0;
    virtual std::string GetPassword() const = 0;
};

}

}