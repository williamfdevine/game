#ifndef CLI_H_
#define CLI_H_

#include <istream>
#include <ostream>
#include <functional>
#include <string>
#include <map>
#include <mutex>
#include <thread>

class CLI
{

public:

	using cli_callback_t = std::function<void (CLI*, std::string&)>; 

	CLI(std::istream& input, std::ostream& output);
	~CLI(void);

	void Start(void);
	void Stop(void);


	bool AddCommand(const std::string& command, const std::string& description, const cli_callback_t callback);
	void Write(const std::string& message);

private:

	struct cli_command_t
	{
		std::string description_;
		cli_callback_t callback_;
	};

	std::istream& input_;
	std::ostream& output_;
	std::mutex cli_mtx_;
	std::thread* cli_thread_;
	bool should_run_;
	bool should_exist_;
	std::map<std::string, CLI::cli_command_t> command_map_;

	int cliThread(void);
	void processInput(std::string& input);

};



#endif //CLI_H_