#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <filesystem>
#include <io.h>
#include <fcntl.h>


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

void auto_push(const std::string& remote_url) {
	using namespace std;

	// 1. 初始化仓库
	if (!std::filesystem::exists(".git")) {
		cout << run("git init") << endl;
	}

	// 2. 添加文件
	cout << run("git add .") << endl;

	// 3. 提交
	cout << run("git commit -m \"Auto commit\"") << endl;

	// 4. 检查远程
	string remotes = run("git remote -v");
	if (remotes.empty() && !remote_url.empty()) {
		cout << run("git remote add origin " + remote_url) << endl;
	}

	// 5. 推送
	string branch = run("git branch --show-current");
	if (branch.empty()) branch = "main";

	// 去掉换行符
	branch.erase(branch.find_last_not_of(" \n\r\t") + 1);

	cout << run("git push -u origin " + branch) << endl;

	int rs = _setmode(_fileno(stdout), _O_WTEXT); // 关键：宽字符模式

	wcout << L"推送完成 ✅" << endl;
	std::wcout.flush();
}

int main() {
	// 切回窄字符模式
	int rs = _setmode(_fileno(stdout), _O_TEXT);
	std::cout << "Normal ASCII output\n";
	std::cout.flush();
	auto_push("https://github.com/nt47/git-cli.git");
	return 0;
}


