#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace ISXMime
{
class MimeEntity;

class Body
{
public:
	Body();
	~Body();

	Body(const Body&) = delete;
	Body& operator=(const Body&) = delete;
	Body(Body&&) noexcept;
	Body& operator=(Body&&) noexcept;

public:
	void Set(const std::string& content);
	const std::string& Content() const;
	std::string& Content();

	bool Load(const std::string& filename);
	bool Save(const std::string& filename) const;

public:
	void SetPreamble(const std::string& preamble);
	const std::string& Preamble() const;

	void SetEpilogue(const std::string& epilogue);
	const std::string& Epilogue() const;

	void SetBoundary(const std::string& boundary);
	const std::string& Boundary() const;
	std::string GenerateRandomBoundary();

public:
	std::vector<std::shared_ptr<MimeEntity>>& Parts();
	const std::vector<std::shared_ptr<MimeEntity>>& Parts() const;

	void AddPart(const std::shared_ptr<MimeEntity>& part);
	bool RemovePart(size_t index);
	std::shared_ptr<MimeEntity> GetPart(size_t index) const;
	size_t PartCount() const;

public:
	friend std::ostream& operator<<(std::ostream& os, const Body& body);

	bool IsMultipart() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_p_impl;
};

// Stream operator declaration
std::ostream& operator<<(std::ostream& os, const Body& body);

} // namespace ISXMime
