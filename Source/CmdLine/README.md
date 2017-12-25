## CommandLine
Simple constructor for create to command line application.

## Usage
Class have a three base subclasses.
* CommandLine::Option -- Create options.
* CommandLine::Command -- Create commands.
* CommandLine::Application -- Create command line application.
Class set default version value to '0.1.1'. For modify version use:
```cpp
helper::CommandLine::Application app("echo");
app.version = "1.1.1";
```

#### Simple application example
Use `.addDefaultCommand` method for create signle function application (Application don't have a commands).
```cpp
#include <iostream>
#include "ApplicationHelpers.h"

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

int main(int argc, char* argv[])
{
	helper::CommandLine::Application app("echo");

	app.about << "Display echo of input text.";
	app.addDefaultCommand(new Echo());
	app.start(argc, argv);

	return 0;
}
```

    $ echo.exe Some Text!
    Some Text!

#### Global options example
Create global option `--name` for all commands in application. If option have a parameter,
then last argument in `helper::CommandLine::Option` constructor should set in `true`.
```cpp
#include <iostream>
#include "ApplicationHelpers.h"

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

int main(int argc, char* argv[])
{
	helper::CommandLine::Application app("hello");

	app.about << "This program say hello.";
	app.usage << "hello [OPTIONS ...]";

	/* Create global option object. */
	app.addopt(helper::CommandLine::Option("n", "name", "Option set name for hello message.", true));
	/* Add default command handler. */
	app.addDefaultCommand(new Hello());
	app.start(argc, argv);

	return 0;
}
```

    $ hello -h
                                 *** hello ***                                  
    ** ABOUT:
      This program say hello.

    ** USAGE:
      hello [OPTIONS ...]

    ** OPTIONS:
      -V,--version     # Display application version.
      -n,--name=[...]  # Option set name for hello message.

    $ hello
    Hello
      
    $ hello -nJohn
    Hello John

#### Commands example
Create application with two commands that calculation square and cube on input number.
```cpp
#include <iostream>
#include <string>
#include "ApplicationHelpers.h"

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

int main(int argc, char* argv[])
{
	helper::CommandLine::Application app("pow");
	app.about << "Calculation square and cube of number.";
	app.usage << "pow COMMAND [OPTIONS...]";
	/* Add commands to application. */
	app.addcmd(new Square());
	app.addcmd(new Cube());

	app.start(argc, argv);

	return 0;
}
```

#### Subcommands example
Create command `remote` for create connectin to localhost and two subcommands: `tcp` and `domain`.
Subcommand `tcp` have a option `--port`.
Create application with two commands that calculation square and cube on input number.
```cpp
#include <iostream>
#include <string>
#include "ApplicationHelpers.h"

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

int main(int argc, char* argv[])
{
	helper::CommandLine::Application app("remote");

	app.about << "Demo of subcommand.";
	app.addcmd(new Connect());
	app.start(argc, argv);

	return 0;
}
```
