#include "CSGameFile.h"

#include "StringUtil.h"

namespace se::cs {
	bool GameFile::getIsMasterFile() const {
		const auto GameFile_getIsMasterFile = reinterpret_cast<bool(__thiscall*)(const GameFile*)>(0x402B1C);
		return GameFile_getIsMasterFile(this);
	}

	bool GameFile::getToLoadFlag() const {
		const auto GameFile_getToLoadFlag = reinterpret_cast<bool(__thiscall*)(const GameFile*)>(0x40148D);
		return GameFile_getToLoadFlag(this);
	}

	void GameFile::setToLoadFlag(bool state) {
		const auto GameFile_setToLoadFlag = reinterpret_cast<void(__thiscall*)(GameFile*, bool)>(0x401D84);
		GameFile_setToLoadFlag(this, state);
	}

	int GameFile::sortAgainst(const GameFile* other) const {
		const auto isMaster = getIsMasterFile();
		const auto otherIsMaster = other->getIsMasterFile();
		if (isMaster != otherIsMaster) {
			return otherIsMaster ? 1 : -1;
		}

		return CompareFileTime(&findFileData.ftLastWriteTime, &other->findFileData.ftLastWriteTime);;
	}

	bool GameFile::getOpenMWAddonSourceName(std::string& sourceName) const {
		constexpr std::string_view addonMarker = ".omwaddon-";
		constexpr std::string_view nativeSuffix = ".esp";
		constexpr size_t cachePrefixLength = 16;
		const std::string_view filename = fileName;

		const auto markerOffset = filename.rfind(addonMarker);
		if (markerOffset == std::string_view::npos) {
			return false;
		}

		const auto hashOffset = markerOffset + addonMarker.length();
		if (filename.length() != hashOffset + cachePrefixLength + nativeSuffix.length()) {
			return false;
		}
		if (!string::iequal(filename.substr(filename.length() - nativeSuffix.length()), nativeSuffix)) {
			return false;
		}
		for (auto i = hashOffset; i < hashOffset + cachePrefixLength; ++i) {
			if (!std::isxdigit(static_cast<unsigned char>(filename[i]))) {
				return false;
			}
		}

		sourceName.assign(filename.substr(0, markerOffset + addonMarker.length() - 1));
		return true;
	}

	bool GameFile::isOpenMWAddonAlias() const {
		std::string sourceName;
		return getOpenMWAddonSourceName(sourceName);
	}
}
