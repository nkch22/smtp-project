#pragma once

#include <string>
#include <list>
#include <optional>

#include "Mailbox.hpp"

namespace SMTP
{

class Transaction
{
public:
    Transaction() = default;
    ~Transaction() = default;

    bool CheckAvailability(const Mailbox& mailbox) const;
    
    void Reset() noexcept;
    
    const std::optional<Mailbox>& GetSender() const; 
    const std::list<Mailbox>& GetRecepients() const;

    void ChangeSender(const Mailbox& mailbox);
    void AddRecepient(const Mailbox& mailbox);
private:
    std::optional<Mailbox> sender;
    std::list<Mailbox> recepients;
};

}