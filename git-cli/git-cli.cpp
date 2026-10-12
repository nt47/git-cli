#include <iostream>
#include <string>
#include"CLI11.hpp"
#include"hooks.h"
#include"cmd.h"
#include"macro.h"
#include"auto.h"
#include"ini.h"
#include"encoding.h"


void pull_rebase(const std::wstring& remote_url) {
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
	//cout << utils::w2s(run_w(L"git commit -m \"重建本地仓库👽\" ")) << endl;

	//记住，开启了_setmode，就别乱切换cout/wcout模式，缓冲区会溢出，导致程序崩溃
	wcout << run_w(L"git commit -m \"pull rebase\"") << endl;

	// 4. 检查远程
	std::wstring remotes = run_w(L"git remote -v");
	wcout << L"remotes: " << remotes << endl;
	if (remotes.empty() && !remote_url.empty()) {
		//cout << run("git remote add origin " + remote_url) << endl;

		wcout << run_w(L"git remote add origin " + remote_url) << endl;
	}

	// 5. 拉取并rebase
	std::wstring branch = run_w(L"git branch --show-current");
	if (branch.empty()) branch = L"main";

	// 去掉换行符
	branch.erase(branch.find_last_not_of(L" \n\r\t") + 1);

	wcout << run_w(L"git pull origin " + branch + L" --rebase") << endl;
}

int main(int argc, char** argv) {
	using namespace std;

	A_START;
	std::cout << "Normal ASCII output\n";

	std::vector<std::string> args;

	for (int i = 1; i < argc; ++i) {
		const std::string arg = argv[i];

		if (arg == "-ur") {
			args.emplace_back("-u");

			if (i + 1 >= argc) {
				std::cerr << "Error: -u requires a value\n";
				return 1;
			}

			args.emplace_back(argv[++i]);
			args.emplace_back("-r");
		}
		else {
			args.emplace_back(arg);
		}
	}

	std::reverse(args.begin(), args.end());



	CLI::App app{ "One Command to Synchronize on Git" };//暂时不支持中文描述，后续版本再完善
	argv = app.ensure_utf8(argv);

	std::wstring message;
	app.add_option("-m,--message", message, "Commit message");

	std::wstring url;
	app.add_option("-u,--url", url, "Remote Repository URL")->required();

	bool rebase = false;
	app.add_flag("-r,--rebase", rebase, "pull with rebase");


	//Custom Commands V2.0
	//Cmd cmd(&app);
	//cmd.init();



	// ✔️ 先解析
	//CLI11_PARSE(app, argc, argv);
	app.parse(args);

	A_STOP;



	W_START;

	// ✔️ 再生成 commit 命令
	std::wstring commit_msg = message.empty() ? L"Auto commit" : message;
	std::wstring git_cmd = L"git commit -m \"" + commit_msg + L"\"";


	auto_pop(url);//为了获取写入git config的url
	IniFile ini(L"./.git/config");
	wstring config_url = ini.Read(L"remote \"origin\"", L"url");
	//wcout << config_url << ::std::endl;
	auto_fix(config_url);

	if (rebase && url == config_url) {//暂时保留命令 git-cli -ur 123
		pull_rebase(url);
	}
	else
	{
		auto_push(git_cmd);
	}


	W_STOP;

	return 0;
}