#pragma once

#include <istream>
#include <memory>
#include <string>

#include "MimeEntity.hpp"

namespace ISXMime
{

class IMimeParser
{
public:
	virtual ~IMimeParser() = default;

	virtual std::shared_ptr<MimeEntity> ParseString(const std::string& mimeContent) = 0;

	virtual std::shared_ptr<MimeEntity> ParseStream(std::istream& stream) = 0;

	virtual std::shared_ptr<MimeEntity> ParseFile(const std::string& filePath) = 0;

	virtual void SetStrictMode(bool strictMode) = 0;
	virtual bool IsStrictMode() const = 0;
};

std::shared_ptr<IMimeParser> CreateDefaultParser();

} // namespace ISXMime
