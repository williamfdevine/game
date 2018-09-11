#include <iostream>
#include <chrono>
#include <thread>
#include "cli.h"

static void sampleCallback(CLI* cli, std::string& message)
{
	cli->Write("WORLD");
}


int main(void)
{
	std::cout << "game" << std::endl;

	CLI cli(std::cin, std::cout);
	cli.AddCommand("HELLO", "TEST", std::bind(sampleCallback, std::placeholders::_1, std::placeholders::_2));
	cli.Start();

	while(1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(15));
	}
}
