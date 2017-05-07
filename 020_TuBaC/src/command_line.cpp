/*
 *
 * Turbo Basic Compiler by mgr_inz_rafal.
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <rchabowski@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 *														// mgr inz. Rafal
 * ----------------------------------------------------------------------------
 */

#include "command_line.h"

#include <iostream>
#include <string>
#include <vector>

command_line::command_line(int argc, char **argv)
{
	// Positional arguments
	positional_args.add("input-file", -1);
	hidden_options.add_options()
		("input-file", po::value<std::string>()->required(), "input file name");

	// Standard arguments
	options.add_options()
		("help,h", "Shows help message")
		("number-type,n", po::value<std::string>()->default_value("integer"),
			"Defines the way compiler will interpret the numbers internally.\n\n"
			"Values:\n"
			"  integer: \tAll numbers will be interpreted as "
			"double-byte integers. This will produce the fastest "
			"code, but usage of floating-point numbers is forbidden\n"
			"  fixed: \tAll numbers will be interpreted as "
			"fixed-point integers. This will allow floating-point "
			"calculations, but with less accuracy than standard "
			"floating-point interpretation\n"
			"  floating: \tAll numbers will be interpreted as "
			"standard 6-byte floating point numbers and Fastmath "
			"package will be used for calculations. This provides "
			"maximum compabibility but for the cost of lowest speed ")
		("output-file,o", po::value<std::string>()->required(),
			"Specify where the assembly file is created")
	;

	all_options.add(options).add(hidden_options);

	po::store(po::command_line_parser(argc, argv)
		.options(all_options)
		.positional(positional_args)
		.run(), vm);
}

/*
Return false if program should not continue.
*/
bool command_line::act()
{
	if (vm.count("help"))
	{
		std::cout << "Turbo Basic Compiler by mgr_inz_rafal" << std::endl;
		std::cout << "Version: 0.1" << std::endl;
		std::cout << "-------------------------------------------------" << std::endl;
		std::cout << "Usage: tubac.exe [options] input_file\n";
		std::cout << options;
		return false;
	}

	po::notify(vm);
	return true;
}

const std::string& command_line::get_param(const std::string& name) const
{
	return vm[name].as<std::string>();
}
