#pragma once

#include "User.hpp"

struct Message
{
	User from;
	User to;
	std::string content;
};