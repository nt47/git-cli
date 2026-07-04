#include "cmd.h"
#include"encoding.h"
#include"auto.h"


Cmd::Cmd(CLI::App* app) {
	// 构造函数
	Cmd::app = app;
}

void Cmd::init() {
	// 初始化函数
	add_option_feed();
	add_option_push();
	add_option_pop();
}

void Cmd::add_option_feed() {
	// 添加feed选项
	auto feed = app->add_subcommand("feed", "Feed AI for something");

	//feed->add_option("feed", message, "to feed")->required();
	feed->add_option("-m,--message", message, "Commit message");

	feed->add_option("-u,--url", url, "Remote Repository URL");

	feed->callback([&]() {

		std::cout << "feed called message: " << utils::w2s(message) << std::endl;
		std::cout << "feed called URL: " << utils::w2s(url) << std::endl;
		});
}

void Cmd::add_option_push() {
	// 添加推送选项
	auto push = app->add_subcommand("push", "Push something");


	//push->add_option("push", cmd_push, "to push")->required();//push 123 -r 123
	push->add_option("-r,--remote", cmd_push_remote, "remote name");//push -r 123


	push->callback([&]() {
		std::cout << "push called remote: " << cmd_push_remote << std::endl;
		});
}

void Cmd::add_option_pop() {
	// 添加弹出选项
	auto pop = app->add_subcommand("pop", "Pop something");

	pop->callback([&]() {
		std::cout << "pop called" << std::endl;
		});
}

Cmd::~Cmd() {
	// 析构函数
}
