#pragma once
#include <string>
#include"CLI11.hpp"
class Cmd
{
public:
	Cmd(CLI::App* app);
	CLI::App* app;

	std::wstring message;
	std::wstring url;

	std::string cmd_push;
	std::string cmd_push_remote;


	void add_option_feed();
	void add_option_push();
	void add_option_pop();

	void init();
	~Cmd();
};

