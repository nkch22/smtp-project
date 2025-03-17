#pragma once

namespace SMTP
{

namespace Database
{

class IDatabase
{
public:
    virtual ~IDatabase() = default;

    virtual void ReadUser() = 0;
    virtual void ReadEmail() = 0;
    
    virtual void CreateUser() = 0;
    virtual void CreateEmail() = 0;
    
    virtual void UpdateUser() = 0;
    virtual void UpdateEmail() = 0;

    virtual void RemoveUser() = 0;
    virtual void RemoveEmail() = 0;
};

}

}