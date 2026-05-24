#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include<Windows.h>
#include <string>
#include <array>
#include <filesystem>
#include <io.h>
#include <fcntl.h>
#include"CLI11.hpp"
#include"encoding.h"
#include"toml.hpp"


namespace fs = std::filesystem;

void SetConfig(const std::wstring& remote_url) {
	if (!fs::exists(L".gc2/config.toml")) {

		fs::create_directories(L".gc2");
		SetFileAttributesW(L".gc2", FILE_ATTRIBUTE_HIDDEN);

		std::wofstream ofs(L".gc2/config.toml");

		ofs << R"([server]
url=")" << remote_url << R"(")" << std::endl;
		ofs.close();

		std::wcout << L"InitConfig done\n";
	}
}

std::string run(const std::string& cmd) {
	std::array<char, 256> buffer{};
	std::string result;

	// 使用 popen 捕获输出
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
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
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_wpopen(cmd.c_str(), L"r"), _pclose);
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

void auto_push(const std::wstring& remote_url, const std::wstring& commit_cmd) {
	using namespace std;

	int rs = _setmode(_fileno(stdout), _O_WTEXT); // 关键：宽字符模式

	// 1. 初始化仓库
	if (!std::filesystem::exists(".git")) {
		wcout << run_w(L"git init") << endl;
	}

	// 2. 添加文件
	wcout << run_w(L"git add .") << endl;

	// 3. 提交
	//cout << run("git commit -m \"Auto commit\"") << endl;
	//cout << run(commit_cmd) << endl;
	//cout << utils::w2s(run_w(L"git commit -m \"中文测试👽\" ")) << endl;
	wcout << run_w(commit_cmd) << endl;

	// 4. 检查远程
	std::wstring remotes = run_w(L"git remote -v");
	if (remotes.empty() && !remote_url.empty()) {
		//cout << run("git remote add origin " + remote_url) << endl;
		wcout << run_w(L"git remote add origin " + remote_url) << endl;
	}

	// 5. 推送
	std::wstring branch = run_w(L"git branch --show-current");
	if (branch.empty()) branch = L"main";

	// 去掉换行符
	branch.erase(branch.find_last_not_of(L" \n\r\t") + 1);

	wcout << run_w(L"git push -u origin " + branch) << endl;


	wcout << L"推送完成 ✅" << endl;
	std::wcout.flush();

}

int main(int argc, char** argv) {

	int rs = _setmode(_fileno(stdout), _O_TEXT);
	std::cout << "Normal ASCII output\n";
	std::cout.flush();


	CLI::App app{ "Git 一键上传 CLI" };
	argv = app.ensure_utf8(argv);

	std::wstring message;
	app.add_option("-m,--message", message, "提交信息");

	std::wstring url;
	app.add_option("-u,--url", url, "远程仓库 URL")->required();

	// ✔️ 先解析
	CLI11_PARSE(app, argc, argv);

	// ✔️ 再生成 commit 命令
	std::wstring commit_msg = message.empty() ? L"Auto commit" : message;
	std::wstring git_cmd = L"git commit -m \"" + commit_msg + L"\"";

	auto_push(url, git_cmd);

	return 0;
}