#include <iostream>
#include <string>
#include "demo_cmdline.h"

/* Crate subcommand class. */
class ConnectTCP : public helper::CommandLine::Command {
public:
	ConnectTCP() : Command()
	{
		name = "tcp";
		brief = "Connection to server by IP address (default port: 1234).";
		example << "remote connect TCP [IP ADDRESS]";
		argnum = 1;

		/* Add option for this subcmd. */
		addopt(helper::CommandLine::Option("p", "port", "Option set connection port.", true));
	}

	void handler(std::map<std::string, helper::CommandLine::Option>& global_options, helper::StringArray& args)
	{
		std::string address(args[0]);

		if (options["port"].present) {
			address += ":";
			address += options["port"].value;
		}

		std::cout << "Connect to " << address << std::endl;
	}
};

/* Crate subcommand class. */
class ConnectDomain : public helper::CommandLine::Command {
public:
	ConnectDomain() : Command()
	{
		name = "domain";
		brief = "Connection to server by domain address.";
		example << "remote connect domain [DOMAIN]";
		argnum = 1;
	}

	void handler(std::map<std::string, helper::CommandLine::Option>& global_options, helper::StringArray& args)
	{
		std::cout << "Connect to " << args[0] << std::endl;
	}
};

/* Main (top) command class. */
class Connect : public helper::CommandLine::Command {
public:
	Connect() : Command()
	{
		name = "remote";
		brief = "Create remote connection.";

		addcmd(new ConnectTCP());
		addcmd(new ConnectDomain());
	}

	void handler(std::map<std::string, helper::CommandLine::Option>& global_options, helper::StringArray& args)
	{
		std::cout << "Connect to localhost." << std::endl;
	}
};

class Square : public helper::CommandLine::Command {
public:
	Square() : Command()
	{
		name = "square";
		brief = "Calculation square of number.";
		details << "Command call format: pow square <number>.";
		example << "pow square 2";
		argnum = 1;
	}

	void handler(std::map<std::string, helper::CommandLine::Option>& global_options, helper::StringArray& args)
	{
		int result = std::stoi(args[0], nullptr, 0);
		result = result * result;
		std::cout << "Square of " << args[0] << " = " << result << std::endl;
	}
};

class Cube : public helper::CommandLine::Command {
public:
	Cube() : Command()
	{
		name = "cube";
		brief = "Calculation cube of number.";
		details << "Command call format: pow cube <number>.";
		example << "pow cube 2";
		argnum = 1;
	}

	void handler(std::map<std::string, helper::CommandLine::Option>& global_options, helper::StringArray& args)
	{
		int result = std::stoi(args[0], nullptr, 0);
		result = result * result * result;
		std::cout << "Cube of " << args[0] << " = " << result << std::endl;
	}
};

/* Create default command handler. */
class Echo : public helper::CommandLine::Command {
public:
	Echo() : Command()
	{ }

	void handler(std::map<std::string, helper::CommandLine::Option>& global_options, helper::StringArray& args)
	{
		std::string line;

		for (std::string arg : args) {
			line += arg;
			line += " ";
		}

		std::cout << line.c_str() << std::endl;
	}
};

/* Create default command handler. */
class Hello : public helper::CommandLine::Command {
public:
	Hello() : Command()
	{ }

	void handler(std::map<std::string, helper::CommandLine::Option>& global_options, helper::StringArray& args)
	{
		std::string hello("Hello ");
		/*
		 * If `present` set to `true`, those option is present.
		 * Parameter of option can be set as : -nJohn or -n=John or -n John.
		 */
		if (global_options["name"].present) {
			hello += global_options["name"].value;
		}

		std::cout << hello.c_str() << std::endl;
	}
};

void test_cmdline(int argc, char* argv[])
{
#if 0
	helper::CommandLine::Option optHelp("help", "some option");
	helper::CommandLine::Option optVersion("version", "version option");

	//std::cout << optHelp.longname.c_str() << std::endl;
	//std::cout << optHelp.shortname.c_str() << std::endl;

	std::map<std::string, helper::CommandLine::Option> options;

	options[optHelp.longname] = optHelp;
	options[optVersion.longname] = optVersion;

	for (auto pair : options) {
		std::cout << pair.first.c_str() << " : " << pair.second.longname.c_str() << std::endl;
	}

	if (options.find("help") != options.end()) {
		std::cout << options["help"].shortname.c_str() << std::endl;
	}
#endif

#if 0
	helper::CommandLine::Application app("hello");

	app.addopt(helper::CommandLine::Option("help", "Help option.", true));
	app.addopt(helper::CommandLine::Option("version", "version option."));

	app.displayOptions(app.options);
#endif

#if 0
	helper::CommandLine::Application app("hello");

	app.addcmd(Square());
	app.addcmd(Cube());

	app.displayCommands(app.commands);
#endif

#if 0
	helper::CommandLine::Application app("hello");

	app.start(argc, argv);
#endif

#if 0
	helper::CommandLine::Application app("hello");

	app.addcmd(Square());
	app.addcmd(Cube());

	helper::CommandLine::Command *cmd = app.commandSearch(app.commands, std::string("cube"));

	if (cmd != nullptr) {
		std::cout << cmd->brief.c_str() << std::endl;
	}
#endif

#if 0
	helper::CommandLine::Application app("hello");

	app.addcmd(Square());
	app.addcmd(Cube());

	app.start(argc, argv);

	for (std::string arg : app.m_args) {
		std::cout << arg.c_str() << std::endl;
	}

	helper::CommandLine::Command *cmd = app.commandSelect();

	if (cmd != nullptr) {
		std::cout << "CMD:" << cmd->brief.c_str() << std::endl;
	}

	for (std::string arg : app.m_args) {
		std::cout << arg.c_str() << std::endl;
	}
#endif

#if 0
	helper::CommandLine::Application app("hello");

	std::map<std::string, std::string> opt = app.getOptionFromLine(std::string("-V"));

	if (opt["name"].length() > 0) {
		std::cout << opt["name"].c_str() << std::endl;
		std::cout << opt["type"].c_str() << std::endl;
	}
#endif

#if 0
	helper::CommandLine::Application app("hello");

	app.addopt(helper::CommandLine::Option("help", "Help option.", true));
	app.addopt(helper::CommandLine::Option("version", "version option."));

	std::map<std::string, std::string> opt = app.getOptionFromLine(std::string("-h"));

	if (opt["name"].length() > 0) {
		if (app.optionCompare(opt, app.options["help"])) {
			std::cout << "Option equ to " << opt["name"].c_str() << std::endl;
		}

		if (app.optionCompare(opt, app.options["version"])) {
			std::cout << "Option equ to " << opt["name"].c_str() << std::endl;
		}
	}
#endif

#if 0
	helper::CommandLine::Application app("hello");

	app.addopt(helper::CommandLine::Option("help", "Help option.", true));
	app.addopt(helper::CommandLine::Option("version", "version option."));

	std::string o = app.optionSearch(app.getOptionFromLine(std::string("-h")), app.options);

	if (o.length() > 0) {
		std::cout << "OPT: " << o.c_str() << std::endl;
	}
#endif

#if 0
	helper::CommandLine::Application app("hello");

	app.m_args << "cmd";
	app.m_args << "--help";
	app.m_args << "--version";
	app.m_args << "PARAM";

	app.addopt(helper::CommandLine::Option("help", "Help option.", true));
	app.addopt(helper::CommandLine::Option("version", "version option.", true));

	app.optionParser(app.options);
#endif

#if 0
	//Example-1 : Simple application example.

	helper::CommandLine::Application app("echo");

	app.about << "Display echo of input text.";
	app.addDefaultCommand(new Echo());
	app.start(argc, argv);
#endif

#if 0
	//Example-2 : Global options example.
	helper::CommandLine::Application app("hello");

	app.about << "This program say hello.";
	app.usage << "hello [OPTIONS ...]";

	/* Create global options object. */
	app.addopt(helper::CommandLine::Option("n", "name", "Option set name for hello message.", true));
	/* Add default command handler. */
	app.addDefaultCommand(new Hello());
	app.start(argc, argv);
#endif

#if 0
	//Example-3 : Commands example.
	helper::CommandLine::Application app("pow");
	app.about << "Calculation square and cube of number.";
	app.usage << "pow COMMAND [OPTIONS...]";
	/* Add commands to application. */
	app.addcmd(new Square());
	app.addcmd(new Cube());

	app.start(argc, argv);
#endif

	//Example-4 : SubCommands example.
	helper::CommandLine::Application app("remote");

	app.about << "Demo of subcommand.";
	app.addcmd(new Connect());
	app.start(argc, argv);
}
