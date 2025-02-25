#include "Buffer.hpp"

namespace SMTP
{

void* Buffer::get_data() noexcept
{
    return std::data(m_data);
}

const void* Buffer::get_data() const noexcept
{
    return std::data(m_data);
}

std::size_t Buffer::get_size() const noexcept
{
    return std::size(m_data);
}

void Buffer::Clear() noexcept
{
    m_data.clear();
}

}