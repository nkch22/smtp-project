#pragma once

#include <memory>
#include <optional>
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
	std::string m_content;
	std::string m_preamble;
	std::string m_epilogue;
	std::string m_boundary;
	std::vector<std::shared_ptr<MimeEntity>> m_parts;
};
} // namespace ISXMime
