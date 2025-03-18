#pragma once

#include "SMTP/Database/IMail.hpp"

using SMTP::Database::IMail;

class Mail : public IMail
{
public:
    ~Mail() = default;
};