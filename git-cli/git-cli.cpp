#include <iostream>
#include <cstdio>
#include <string>
#include <array>
#include"CLI11.hpp"
#include"cmd.h"
#include"hooks.h"
#include"macro.h"
#include"auto.h"
#include"ini.h"


void auto_pop(const std::wstring& remote_url) {
	using namespace std;
	// 1. 初始化仓库
	if (!std::filesystem::exists(".git")) {
		wcout << run_w(L"git init") << endl;
	}
	// 2. 检查远程
	std::wstring remotes = run_w(L"git remote -v");
	wcout << L"remotes: " << remotes << endl;
	if (remotes.empty() && !remote_url.empty()) {
		//cout << run("git remote add origin " + remote_url) << endl;
		wcout << run_w(L"git remote add origin " + remote_url) << endl;
	}
	// 3. 拉取
	std::wstring branch = run_w(L"git branch --show-current");
	if (branch.empty()) branch = L"main";
	// 去掉换行符
	branch.erase(branch.find_last_not_of(L" \n\r\t") + 1);
	wcout << run_w(L"git pull origin " + branch) << endl;
}

void auto_push(const std::wstring& remote_url, const std::wstring& commit_cmd) {
	using namespace std;

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
	wcout << L"remotes: " << remotes << endl;
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

}


void auto_fix(const std::wstring& remote_url, const std::wstring& git_cmd) {
	using namespace std;

	wstring rs_msg = run_w(L"git ls-remote " + remote_url);
	wstring url = L"";
	//std::wcout << L"Error: " << rs_msg << std::endl;

	//wstring rs_msg = run_w(L"git status");
	//std::wcout << L"Msg:" << rs_msg << std::endl;//返回值例子

	if (wcswcs(rs_msg.c_str(), L"fatal")) {
		wcout << L"Error: " << rs_msg << std::endl;

		while (wcswcs(rs_msg.c_str(), L"fatal"))
		{
			wcout << L"[Auto Fix]Remote URL Not Exists, Please Enter a Valid URL...[!!!]" << endl;
			wcin >> url;
			rs_msg = run_w(L"git ls-remote " + url);
			if (wcswcs(rs_msg.c_str(), L"fatal") == 0)
			{
				wcout << L"[Auto Fix]Remote URL Validated Successfully ✅" << endl;
				wcout << run_w(L"git remote set-url origin " + url) << endl;
				break;
			}
		}

		auto_pop(url);

	}
	else {
		return;
	}

}

int main(int argc, char** argv) {
	using namespace std;

	A_START;
	std::cout << "Normal ASCII output\n";



	CLI::App app{ "One Command to Synchronize on Git" };//暂时不支持中文描述，后续版本再完善
	argv = app.ensure_utf8(argv);

	std::wstring message;
	app.add_option("-m,--message", message, "Commit message");

	std::wstring url;
	app.add_option("-u,--url", url, "Remote Repository URL")->required();


	//Custom Commands V2.0
	//Cmd cmd(&app);
	//cmd.init();



	// ✔️ 先解析
	CLI11_PARSE(app, argc, argv);

	A_STOP;



	W_START;

	// ✔️ 再生成 commit 命令
	std::wstring commit_msg = message.empty() ? L"Auto commit" : message;
	std::wstring git_cmd = L"git commit -m \"" + commit_msg + L"\"";


	auto_pop(url);
	IniFile ini(L"./.git/config");
	wstring config_url = ini.Read(L"remote \"origin\"", L"url");
	//wcout << config_url << ::std::endl;
	auto_fix(config_url, git_cmd);

	auto_push(url, git_cmd);


	W_STOP;

	return 0;
}