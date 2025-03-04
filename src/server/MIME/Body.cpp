#include "Body.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>

#include "MimeEntity.hpp"

namespace ISXMime
{

class Body::Impl
{
public:
	Impl() = default;

	std::string content;
	std::string preamble;
	std::string epilogue;
	std::string boundary;
	std::vector<std::shared_ptr<MimeEntity>> parts;
};

Body::Body() : m_p_impl(std::make_unique<Impl>()) {}
Body::~Body() = default;

Body::Body(Body&&) noexcept = default;
Body& Body::operator=(Body&&) noexcept = default;

void Body::Set(const std::string& content)
{
	m_p_impl->content = content;
}

const std::string& Body::Content() const
{
	return m_p_impl->content;
}

std::string& Body::Content()
{
	return m_p_impl->content;
}

bool Body::Load(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file) return false;

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	m_p_impl->content = content;
	return true;
}

bool Body::Save(const std::string& filename) const
{
	std::ofstream file(filename, std::ios::binary);
	if (!file) return false;

	file << m_p_impl->content;
	return file.good();
}

void Body::SetPreamble(const std::string& preamble)
{
	m_p_impl->preamble = preamble;
}

const std::string& Body::Preamble() const
{
	return m_p_impl->preamble;
}

void Body::SetEpilogue(const std::string& epilogue)
{
	m_p_impl->epilogue = epilogue;
}

const std::string& Body::Epilogue() const
{
	return m_p_impl->epilogue;
}

void Body::SetBoundary(const std::string& boundary)
{
	m_p_impl->boundary = boundary;
}

const std::string& Body::Boundary() const
{
	return m_p_impl->boundary;
}

std::string Body::GenerateRandomBoundary()
{
	// random boundary
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<> dist(0, 35);

	const char* charset = "abcdefghijklmnopqrstuvwxyz0123456789";
	std::string boundary = "----=_NextPart_";

	// timestamp for uniqueness
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);
	std::stringstream time_str;
	time_str << std::hex << now_c;
	boundary += time_str.str() + "_";

	// random characters
	for (int i = 0; i < 16; ++i) boundary += charset[dist(generator)];

	m_p_impl->boundary = boundary;
	return boundary;
}

std::vector<std::shared_ptr<MimeEntity>>& Body::Parts()
{
	return m_p_impl->parts;
}

const std::vector<std::shared_ptr<MimeEntity>>& Body::Parts() const
{
	return m_p_impl->parts;
}

void Body::AddPart(const std::shared_ptr<MimeEntity>& part)
{
	m_p_impl->parts.push_back(part);
}

bool Body::RemovePart(size_t index)
{
	if (index >= m_p_impl->parts.size()) return false;

	m_p_impl->parts.erase(m_p_impl->parts.begin() + index);
	return true;
}

std::shared_ptr<MimeEntity> Body::GetPart(size_t index) const
{
	if (index >= m_p_impl->parts.size()) return nullptr;

	return m_p_impl->parts[index];
}

size_t Body::PartCount() const
{
	return m_p_impl->parts.size();
}

bool Body::IsMultipart() const
{
	return !m_p_impl->boundary.empty() && !m_p_impl->parts.empty();
}

std::ostream& operator<<(std::ostream& os, const Body& body)
{
	// if this is a simple (non-multipart) body - output the content
	if (!body.IsMultipart()) return os << body.Content();

	// for multipart bodies, we need to output all parts with boundaries
	if (!body.Preamble().empty()) os << body.Preamble() << "\r\n";

	// output each part with a boundary
	for (const auto& part : body.Parts())
	{
		os << "--" << body.Boundary() << "\r\n";

		std::ostringstream part_stream;
		part_stream << *part;
		os << part_stream.str() << "\r\n";
	}

	// close with the final boundary
	os << "--" << body.Boundary() << "--\r\n";

	if (!body.Epilogue().empty()) os << body.Epilogue();

	return os;
}

} // namespace ISXMime
