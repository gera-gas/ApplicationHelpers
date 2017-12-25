#ifndef CMDLINE_COMMAND_LINE_H
#define CMDLINE_COMMAND_LINE_H

#include <map>
#include "Containers/StringArray.h"

namespace helper {

	extern const char* kEmptyString;

	class CommandLine {
	private:
		enum { kTerminalMaxLine = 80 };
		static void outTitle(std::string &title);
		static void outStringArrayText(const char *indent, helper::StringArray &sary);

	public:
		class Option {
		public:
			std::string shortname;
			std::string longname;
			std::string brief;
			bool withParam;
			std::string value;
			bool present;

			Option() :
				shortname(kEmptyString),
				longname(kEmptyString),
				brief(kEmptyString),
				value(kEmptyString),
				withParam(false),
				present(false)
			{ }

			Option(const std::string& newShortname, const std::string& newLongname, const std::string& newBrief, bool Param = false) :
				shortname(newShortname),
				longname(newLongname),
				brief(newBrief),
				value(kEmptyString),
				withParam(Param),
				present(false)
			{ }
#if 0
			Option(const std::string& newLongname, const std::string& newBrief, bool Param = false) :
				shortname(kEmptyString),
				longname(newLongname),
				brief(newBrief),
				value(kEmptyString),
				withParam(Param),
				present(false)
			{
				char *str = new char[2];
				str[0] = newLongname[0];
				str[1] = '\0';
				shortname.append(str);
				delete str;
			}
#endif

			Option& operator=(Option &other)
			{
				if (&other != this) {
					shortname = other.shortname;
					longname = other.longname;
					brief = other.brief;
					value = other.value;
					withParam = other.withParam;
					present = other.present;
				}

				return *this;
			}
		};
		
		class Command {
		public:
			std::string name;
			std::string brief;
			helper::StringArray details;
			helper::StringArray example;
			std::map<std::string,Option> options;
			std::vector<Command*> subcmd;
			size_t argnum;

			Command() :
				name(kEmptyString),
				brief(kEmptyString),
				argnum(0)
			{ }

			~Command()
			{
				for (Command *cmd : subcmd) {
					delete cmd;
				}

				subcmd.clear();
				options.clear();
			}

			virtual void handler(std::map<std::string, helper::CommandLine::Option>& global_options, helper::StringArray& args)
			{
				std::cout << "error: handler do not set for this command." << std::endl;
			}

#define COMMAND_LINE_HANDLER void handler(std::map<std::string, Option>& global_options, helper::StringArray& args)

			void addopt(Option& option)
			{
				options[option.longname] = option;
			}

			void addcmd(Command *cmd)
			{
				subcmd.push_back(cmd);
			}
		};

		class Application {
		public:
			std::string name;
			helper::StringArray about;
			helper::StringArray usage;
			std::vector<Command*> commands;
			std::map<std::string, Option> options;
			Command *defaultCommand;
			std::string version;

			Application(const char *appName) :
				name(appName),
				defaultCommand(nullptr),
				version("0.1.1")
			{
				addopt(Option("V", "version", "Display application version."));
			}

			~Application()
			{
				if (defaultCommand != nullptr) {
					delete defaultCommand;
				}

				for (Command *cmd : commands) {
					delete cmd;
				}
			}

			void addopt(Option& option)
			{
				options[option.longname] = option;
			}

			void addcmd(Command *cmd)
			{
				commands.push_back(cmd);
			}

			void addDefaultCommand(Command *cmd)
			{
				defaultCommand = cmd;
			}

			void start(int argc, char* argv[]);

		private:
			helper::StringArray m_args;
			
			void displayOptions(std::map<std::string, Option>& optlist);
			void displayCommands(std::vector<Command*>& cmdlist);
			Command* commandSearch(std::vector<Command*>& cmdlist, std::string& cmdName);
			Command* commandSelect();
			std::map<std::string, std::string> getOptionFromLine(std::string& arg);
			void removeExcessOption();
			bool optionCompare(std::map<std::string, std::string>& optFromGetOption, Option& optFromApp);
			std::string optionSearch(std::map<std::string, std::string>& optFromGetOption, std::map<std::string, Option>& appOptionsList);
			std::string optionSet(std::map<std::string, std::string>& optFromGetOption, Option& appOption, size_t optionIndex);
			void optionParser(std::map<std::string, Option>& appOptionsList);
		};
	};

} /* helper */

#endif /* CMDLINE_COMMAND_LINE_H */
