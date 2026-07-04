#include"hooks.h"
#include <array>
#include <memory>
#include <stdexcept>
#include"encoding.h"

std::string run(const std::string& cmd) {
	std::array<char, 256> buffer{};
	std::string result;

	// 使用 popen 捕获输出
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen((cmd + " 2>&1").c_str(), "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}

	while (fgets(buffer.data(), (int)buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}

	return result;
}


std::wstring run_w(const std::wstring& cmd)
{
	std::array<char, 256> buffer{};
	std::string result_mb;

	// 用 _wpopen 执行宽字符命令
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_wpopen((cmd + L" 2>&1").c_str(), L"r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("wpopen() failed!");
	}

	// 注意：子进程输出仍然是多字节（GBK 或 UTF-8）
	while (fgets(buffer.data(), (int)buffer.size(), pipe.get()) != nullptr) {
		result_mb += buffer.data();
	}

	// 多字节 → UTF-16
	//int wide_len = MultiByteToWideChar(CP_UTF8, 0, result_mb.c_str(), -1, nullptr, 0);
	//std::wstring result(wide_len, L'\0');
	//MultiByteToWideChar(CP_UTF8, 0, result_mb.c_str(), -1, &result[0], wide_len);

	//utf8 转 utf16
	std::wstring result = utils::utf8_to_utf16(result_mb);

	return result;
}