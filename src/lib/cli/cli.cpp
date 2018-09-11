#include "cli.h"
#include <iostream>
#include <sstream>

CLI::CLI(std::istream& input, std::ostream& output):
	input_(input), output_(output), should_exist_(true), should_run_(false)
{
	cli_thread_ = new std::thread([this]{
		cliThread();
	});
}

CLI::~CLI(void)
{
	Stop();
	should_exist_ = false;
	cli_thread_->join();
}

void CLI::Start(void)
{
	should_run_ = true;
}

void CLI::Stop(void)
{
	should_run_ = false;
}

bool CLI::AddCommand(const std::string& command, const std::string& description, const cli_callback_t callback)
{
	cli_command_t cli_command = { .description_ = description, .callback_ = callback};
	try
	{
		std::lock_guard<std::mutex> guard(cli_mtx_);
		command_map_.insert(std::make_pair(command, cli_command));
		return true;
	}
	catch(std::exception& e)
	{
		output_ << "COULD NOT ADD COMMAND " << command << std::endl;
		return false;
	}
}

void CLI::Write(const std::string& message)
{
	std::lock_guard<std::mutex> guard(cli_mtx_);
	output_ << message << std::endl;
}

int CLI::cliThread(void)
{
	while(true == should_exist_)
	{
		while(true == should_run_)
		{
			std::string line;
			std::getline(input_, line);
			processInput(line);
		}
	}

	return 0;
}

void CLI::processInput(std::string& input)
{
	try
	{
		std::stringstream ss(input);
		std::string input_token;
		std::getline(ss, input_token, ' ');

		cli_command_t command;
		
		{
			std::lock_guard<std::mutex> guard(cli_mtx_);
			command = command_map_.at(input_token);
		}
		
		if(nullptr != command.callback_)
		{

			command.callback_(this, input);
		}
	}
	catch(std::exception& e)
	{
		output_ << "COMMAND NOT FOUND IN MAP" << std::endl;
	}
}