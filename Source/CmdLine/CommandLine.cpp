#include <iostream>
#include <sstream>
#include "Logger/Logger.h"
#include "CmdLine/CommandLine.h"

namespace helper {

	const char* kOptionPrefixShort = "-";
	const char* kOptionPrefixLong = "--";
	const char* kEmptyString = "";

	void CommandLine::outTitle(std::string &title)
	{
		if (title.length() < (kTerminalMaxLine - 4)) {
			size_t borderlen = 80 - title.length() - 4;
			for (size_t i = 0; i < (borderlen / 2); i++) {
				std::cout << "*";
			}
			std::cout << "[";
			std::cout << title;
			std::cout << "]";
			for (size_t i = 0; i < (borderlen / 2); i++) {
				std::cout << "*";
			}
		}
		else {
			std::cout << title;
		}
		std::cout << std::endl;
	}

	void CommandLine::outStringArrayText(const char *indent, helper::StringArray &sary)
	{
		for (std::string s : sary) {
			std::cout << indent << s << std::endl;
		}
	}

	/**
	 * Main command line application manager.
	 */
	void CommandLine::Application::start(int argc, char* argv[])
	{
		for (size_t i = 1; i < argc; i++) {
			m_args << argv[i];
		}

		optionParser(options);

		/* Check on version request. */
		if (options["version"].present) {
			std::cout << name << ", version ";
			std::cout << version << std::endl;
			exit(0);
		}
		
		if(m_args.size() > 0) {
			/*
			 * Check on help request.
			 */
			if (m_args[0] == "help" || m_args[0] == "--help" || m_args[0] == "-h") {
				/*
				 * Help arguments apsent, well then display information about application.
				 */
				if (m_args.size() == 1) {
					std::cout << std::endl;

					std::string title;
					title += "*** ";
					title += name;
					title += " ***";

					size_t spaces = (kTerminalMaxLine - title.length()) / 2;

					for (size_t i = 0; i < spaces; i++) {
						std::cout << " ";
					}
					std::cout << title;
					for (size_t i = 0; i < spaces; i++) {
						std::cout << " ";
					}

					/* Display about info. */
					if (about.size() > 0) {
						std::cout << std::endl;
						std::cout << "** ABOUT:" << std::endl;
						outStringArrayText("  ", about);
					}
					/* Display usage info. */
					if (usage.size() > 0) {
						std::cout << std::endl;
						std::cout << "** USAGE:" << std::endl;
						outStringArrayText("  ", usage);
					}
					/* Display options info. */
					std::cout << std::endl;
					std::cout << "** OPTIONS:" << std::endl;
					displayOptions(options);
					/* Display commands info. */
					if (commands.size() > 0) {
						std::cout << std::endl;
						std::cout << "** COMMANDS:" << std::endl;
						displayCommands(commands);
						std::cout << std::endl;
						std::cout << "For details, type: help [COMMAND]" << std::endl;
					}
					std::cout << std::endl;
				}
				/*
				 * Help arguments exist, find command in application command list.
				 */
				else {
					m_args.erase(0);

					if (commands.size() == 0) {
						std::cout << "fatal error: commands not found." << std::endl;
						exit(1);
					}

					if (m_args.size() == 0) {
						std::cout << "fatal error: miss command name argument." << std::endl;
						exit(1);
					}

					Command *cmd = commandSelect();

					if (cmd == nullptr) {
						std::cout << "fatal error: unknown command ";
						for (auto arg : m_args) {
							std::cout << arg << " ";
						}
						std::cout << std::endl;
						exit(1);
					}

					/* Display describe information on command. */
					std::cout << std::endl;
					outTitle(cmd->name);
					std::cout << "  " << cmd->brief  << std::endl;
					if (cmd->details.size() > 0) {
						std::cout << std::endl;
						std::cout << "** DETAILS:" << std::endl;
						outStringArrayText("  ", cmd->details);
					}
					if (cmd->example.size() > 0) {
						std::cout << std::endl;
						std::cout << "** EXAMPLE:" << std::endl;
						outStringArrayText("  ", cmd->example);
					}
					if (cmd->options.size() > 0) {
						std::cout << std::endl;
						std::cout << "** OPTIONS:" << std::endl;
						displayOptions(cmd->options);
					}
					if (cmd->subcmd.size() > 0) {
						std::cout << std::endl;
						std::cout << "** SUBCOMMANDS:" << std::endl;
						displayCommands(cmd->subcmd);
						std::cout << std::endl;
						std::cout << "For details, type: help ";
						std::cout << cmd->name;
						std::cout << " [SUBCOMMAND]" << std::endl;
					}
					std::cout << std::endl;
				}
				exit(0);
			}
		}

		/*
		 * Handling default command (if handler exist here).
		 */
		if (defaultCommand != nullptr) {
			removeExcessOption();
			if (m_args.size() < defaultCommand->argnum) {
				std::cout << "fatal error: to few arguments for programm, use <help>." << std::endl;
				exit(1);
			}
			else {
				defaultCommand->handler(options, m_args);
				exit(0);
			}
		}

		/* Handling commands. */
		if (commands.size() == 0) {
			std::cout << "fatal error: commands not found." << std::endl;
			exit(1);
		}

		if (m_args.size() == 0) {
			std::cout << "fatal error: miss command name argument." << std::endl;
			exit(1);
		}

		Command *cmd = commandSelect();

		if (cmd == nullptr) {
			std::cout << "fatal error: unknown command ";
			for (auto arg : m_args) {
				std::cout << arg << " ";
			}
			std::cout << std::endl;
			exit(1);
		}

		if (m_args.size() < cmd->argnum) {
			std::cout << "fatal error: to few arguments for command, use <help> <command name>." << std::endl;
			exit(1);
		}

		/* Scaning options for this command. */
		optionParser(cmd->options);
		removeExcessOption();
		cmd->handler(options, m_args);
		exit(0);
	}

	void CommandLine::Application::displayOptions(std::map<std::string, Option>& optlist)
	{
		size_t maxlen = 0;
		std::map<std::string, std::string> listout;

		for (auto opt : optlist) {
			std::string optnames;
			if (opt.second.shortname.length() == 0) {
				optnames += "  ";
			}
			else {
				optnames += kOptionPrefixShort + opt.second.shortname;
			}
			optnames += ",";
			if (opt.second.longname.length() != 0) {
				optnames += kOptionPrefixLong + opt.second.longname;
			}
			if (opt.second.withParam) {
				optnames += "=[...]";
			}
			listout[optnames] = opt.second.brief;
			if (optnames.length() > maxlen) {
				maxlen = optnames.length();
			}
		}

		for (auto opt : listout) {
			std::cout << "  " << opt.first;
			size_t spaceAlign = maxlen - opt.first.length();
			while (spaceAlign--) {
				std::cout << " ";
			}
			std::cout << "  # ";
			std::cout << opt.second << std::endl;
		}
	}

	void CommandLine::Application::displayCommands(std::vector<Command*>& cmdlist)
	{
		size_t maxlen = 0;

		for (auto cmd : cmdlist) {
			if (cmd->name.length() > maxlen) {
				maxlen = cmd->name.length();
			}
		}

		for (auto cmd : cmdlist) {
			std::cout << "  " << cmd->name;
			size_t spaceAlign = maxlen - cmd->name.length();
			while (spaceAlign--) {
				std::cout << " ";
			}
			std::cout << "  # ";
			std::cout << cmd->brief << std::endl;
		}
	}

	CommandLine::Command* CommandLine::Application::commandSearch(std::vector<Command*>& cmdlist, std::string& cmdName)
	{
		for (auto cmd : cmdlist) {
			if (cmd->name == cmdName) {
				return cmd;
			}
		}

		return nullptr;
	}

	CommandLine::Command* CommandLine::Application::commandSelect()
	{
		if (m_args.size() == 0) {
			return nullptr;
		}

		Command* command = commandSearch(commands, m_args[0]);

		size_t numDeleteIterations = 0;
		if (command != nullptr) {
			numDeleteIterations++;
			for (size_t i = numDeleteIterations; i < m_args.size(); i++) {
				Command* cmd = commandSearch(command->subcmd, m_args[i]);
				if (cmd == nullptr) {
					break;
				}
				numDeleteIterations++;
				command = cmd;
			}
		}

		while (numDeleteIterations--) {
			m_args.erase(0);
		}

		return command;
	}

	std::map<std::string, std::string> CommandLine::Application::getOptionFromLine(std::string& arg)
	{
		std::map<std::string, std::string> result;

		result["name"] = kEmptyString;
		result["type"] = kEmptyString;

		helper::StringArray prefixes;

		prefixes << kOptionPrefixLong;
		prefixes << kOptionPrefixShort;

		for (std::string prefix : prefixes) {
			if (arg.length() > prefix.length()) {
				if (strncmp(kOptionPrefixLong, arg.c_str(), prefix.length()) == 0) {
					result["name"] = &(arg.c_str())[prefix.length()];
					result["type"] = prefix;
					return result;
				}
			}
		}

		return result;
	}

	void CommandLine::Application::removeExcessOption()
	{
		for (std::string arg : m_args) {
			std::map<std::string, std::string> opt = getOptionFromLine(arg);

			if (opt["name"].length() > 0) {
				std::cout << "fatal error: unknown option ";
				std::cout << opt["type"].c_str();
				std::cout << opt["name"].c_str() << std::endl;
				exit(1);
			}
		}
	}

	/**
	 * @brief Compare parsing option(form by getopt) with application option.
	 *
	 * @param optFromGetOption -- object formed by 'getOptionFromLine'.
	 * @param optFromApp  -- application option object.
	 *
	 * @return Return true if options is equal, else return false.
	 */
	bool CommandLine::Application::optionCompare(std::map<std::string, std::string>& optFromGetOption, Option& optFromApp)
	{
		if (optFromGetOption["type"] == kOptionPrefixShort) {
			if (optFromGetOption["name"].length() >= optFromApp.shortname.length()) {
				if (optFromGetOption["name"].substr(0, optFromApp.shortname.length()) == optFromApp.shortname) {
					return true;
				}
			}
		}
		else if (optFromGetOption["type"] == kOptionPrefixLong) {
			if (optFromGetOption["name"].length() >= optFromApp.longname.length()) {
				if (optFromGetOption["name"].substr(0, optFromApp.longname.length()) == optFromApp.longname) {
					return true;
				}
			}
		}		

		return false;
	}

	/**
	 * @brief Search option in application options.
	 *
	 * @param optFromGetOption  -- object formed by 'getOptionFromLine'.
	 * @param appOptionsList -- options object list.
	 *
	 * @return option key - name if option found, else return 'kEmptyString'.
	 */
	std::string CommandLine::Application::optionSearch(std::map<std::string, std::string>& optFromGetOption, std::map<std::string, Option>& appOptionsList)
	{
		std::string result(kEmptyString);

		if (optFromGetOption["name"].length() > 0) {
			for (auto opt : appOptionsList) {
				if (optionCompare(optFromGetOption, opt.second)) {
					result = opt.first;
					break;
				}
			}
		}

		return result;
	}

	/**
	 * @brief Set application option object.
	 *
	 * @param optFromGetOption -- object formed by 'getOptionFromLine'.
	 * @param appOption -- application option object.
	 * @param optionIndex -- index in 'm_args' array.
	 *
	 * @return Return option argument name in 'm_args' for delete or 'kEmptyString'.
	 */
	std::string CommandLine::Application::optionSet(std::map<std::string, std::string>& optFromGetOption, Option& appOption, size_t optionIndex)
	{
		std::string result(kEmptyString);

		if (appOption.withParam) {
			/*
			 * Option have a parameter buit into option name by '='.
			 */
			size_t pos = optFromGetOption["name"].find("=");

			if (pos != std::string::npos) {
				pos++;
				appOption.value = optFromGetOption["name"].substr(pos, optFromGetOption["name"].length() - pos);
				if (appOption.value.length() == 0) {
					std::cout << "fatal error: miss argument for option ";
					std::cout << optFromGetOption["type"];
					std::cout << optFromGetOption["name"] << std::endl;
					exit(1);
				}
				appOption.present = true;
				return result;
			}

			/*
			 * Option parameter buit-in option name.
			 */
			if (optFromGetOption["type"] == kOptionPrefixShort && optFromGetOption["name"].length() > appOption.shortname.length()) {
				appOption.value = optFromGetOption["name"].substr(
					appOption.shortname.length(),
					optFromGetOption["name"].length() - appOption.shortname.length()
				);
				appOption.present = true;
				return result;
			}

			/*
			 * Option parameter present in next argument position.
			 */
			if ((optionIndex + 1) >= m_args.size()) {
				std::cout << "fatal error: miss argument for option ";
				std::cout << optFromGetOption["type"];
				std::cout << optFromGetOption["name"] << std::endl;
				exit(1);
			}
			/* If next argument is a option. */
			if (getOptionFromLine(m_args[optionIndex + 1])["name"].length() != 0) {
				std::cout << "fatal error: miss argument for option ";
				std::cout << optFromGetOption["type"];
				std::cout << optFromGetOption["name"] << std::endl;
				exit(1);
			}
			appOption.value = m_args[optionIndex + 1];
			result = appOption.value;
			appOption.present = true;
		}
		else {
			appOption.present = true;
		}

		return result;
	}

	/**
	 * Parsing options in command line.
	 */
	void CommandLine::Application::optionParser(std::map<std::string, Option>& appOptionsList)
	{
		if (appOptionsList.size() == 0 || m_args.size() == 0) {
			return;
		}

		helper::StringArray deleteList;
		size_t optionIndex = 0;

		for (auto arg : m_args) {
			std::map<std::string, std::string> opt = getOptionFromLine(arg);

			if (opt["name"] != kEmptyString && opt["name"] != "h" && opt["name"] != "help") {
				std::string optName = optionSearch(opt, appOptionsList);
				if (optName != kEmptyString) {
					deleteList << arg;
					std::string nextArgName = optionSet(opt, appOptionsList[optName], optionIndex);
					if (nextArgName != kEmptyString) {
						deleteList << nextArgName;
					}
				}
			}

			optionIndex++;
		}

		/* Delete option from m_args. */
		for (auto argName : deleteList) {
			m_args.erase(argName);
		}
	}

} /* helper */
