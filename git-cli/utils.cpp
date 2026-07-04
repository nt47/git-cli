#include"utils.h"

std::string utils::js::replace(std::string& s, const std::string& pattern, const std::string& replacement) {
	if (pattern.empty()) return s;

	size_t pos = 0;
	while ((pos = s.find(pattern, pos)) != std::string::npos) {
		s.replace(pos, pattern.length(), replacement);
		pos += replacement.length();  // 安全，因为 replacement 不包含 pattern
	}
	return s;
}
