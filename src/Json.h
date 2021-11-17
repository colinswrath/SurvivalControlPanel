#pragma once

namespace Json
{
	std::filesystem::path GetUserDirectory();
	bool Save(std::filesystem::path filePath);
	bool Load(std::filesystem::path filePath);
}
