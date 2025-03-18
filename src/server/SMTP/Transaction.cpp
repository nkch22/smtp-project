#include "Transaction.hpp"

namespace SMTP
{

bool Transaction::CheckAvailability(const Mailbox& mailbox) const
{
    return true;
}

void Transaction::Reset() noexcept
{
    sender.reset();
    recepients.clear();
}
    
const std::optional<Mailbox>& Transaction::get_sender() const
{
    return sender;
}

const std::list<Mailbox>& Transaction::get_recepients() const
{
    return recepients;
}

void Transaction::ChangeSender(const Mailbox& mailbox)
{
    sender.emplace(mailbox);
}

void Transaction::AddRecepient(const Mailbox& mailbox)
{
    recepients.push_back(mailbox);
}

}