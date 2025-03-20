#include "MimeEntity.hpp"

#include "Body.hpp"

namespace ISXMime
{

class MimeEntity::Impl
{
public:
	Impl() = default;

	Header header;
	Body body;
};

MimeEntity::MimeEntity() : m_p_impl(std::make_unique<Impl>()) {}
MimeEntity::~MimeEntity() = default;

MimeEntity::MimeEntity(MimeEntity&&) noexcept = default;
MimeEntity& MimeEntity::operator=(MimeEntity&&) noexcept = default;

Header& MimeEntity::GetHeader()
{
	return m_p_impl->header;
}

const Header& MimeEntity::GetHeader() const
{
	return m_p_impl->header;
}

Body& MimeEntity::GetBody()
{
	return m_p_impl->body;
}

const Body& MimeEntity::GetBody() const
{
	return m_p_impl->body;
}

bool MimeEntity::HasField(const std::string& field_name) const
{
	return m_p_impl->header.HasField(field_name);
}

std::ostream& MimeEntity::Write(std::ostream& os, const char* oel) const
{
	os << m_p_impl->header;
	os << oel;
	os << m_p_impl->body;

	return os;
}

std::ostream& operator<<(std::ostream& os, const MimeEntity& entity)
{
	return entity.Write(os);
}

} // namespace ISXMime
