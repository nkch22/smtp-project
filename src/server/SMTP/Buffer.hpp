#pragma once

#include <vector>
#include <cstddef>

namespace SMTP
{

class Buffer
{
public:
    Buffer() = default;
    ~Buffer() = default;
    void* get_data() noexcept;
    const void* get_data() const noexcept;
    std::size_t get_size() const noexcept;
    void Clear() noexcept;
private:
    std::vector<std::byte> m_data;
};

}