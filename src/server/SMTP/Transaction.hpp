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
    bool CheckAvailability(const Mailbox& mailbox) const;
    
    void Reset() noexcept;
    
    const std::optional<Mailbox>& get_sender() const;
    const std::list<Mailbox>& get_recepients() const;

    void ChangeSender(const Mailbox& mailbox);
    void AddRecepient(const Mailbox& mailbox);
private:
    std::optional<Mailbox> sender;
    std::list<Mailbox> recepients;
};

}