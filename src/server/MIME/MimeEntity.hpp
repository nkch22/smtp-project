#pragma once

#include <memory>
#include <ostream>
#include <string>

#include "Header.hpp"

namespace ISXMime
{
class Body;

class MimeEntity
{
public:
	MimeEntity();
	virtual ~MimeEntity();
	MimeEntity(const MimeEntity&) = delete;
	MimeEntity& operator=(const MimeEntity&) = delete;
	MimeEntity(MimeEntity&&) noexcept;
	MimeEntity& operator=(MimeEntity&&) noexcept;

public:
	Header& GetHeader();
	const Header& GetHeader() const;

	Body& GetBody();
	const Body& GetBody() const;

public:
	bool HasField(const std::string& field_name) const;
	
	friend std::ostream& operator<<(std::ostream& os, const MimeEntity& entity);

private:
	virtual std::ostream& Write(std::ostream& os, const char* oel = "\r\n") const;

private:
	class Impl;
	std::unique_ptr<Impl> m_p_impl;
};

std::ostream& operator<<(std::ostream& os, const MimeEntity& entity);

} // namespace ISXMime
