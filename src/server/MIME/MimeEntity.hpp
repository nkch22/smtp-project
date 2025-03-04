#pragma once

#include <iostream>
#include <memory>
#include <ostream>
#include <string>

#include "Body.hpp"
#include "Header.hpp"

namespace ISXMime
{
class MimeEntity
{
public:
	MimeEntity();
	~MimeEntity();

public:
	Header& GetHeader();
	const Header& GetHeader() const;

	Body& GetBody();
	const Body& GetBody() const;

public:
	bool HasField(const std::string& field_name) const;

	friend std::ostream& operator<<(std::ostream& os, const MimeEntity& entity);

protected:
	virtual std::ostream& Write(std::ostream& os, const char* oel = "\r\n");

private:
	Header m_header; 
	Body m_body;	
};
} // namespace ISXMime
