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

#include "process_group_executor.h"
#include "exceptions.h"

#include <iostream>

process_group_executor::process_group_executor(std::vector<process_executor*> _group)
	:group(std::move(_group))
{
}

opt::optional<std::string> process_group_executor::run()
{
	try
	{
		opt::optional<std::string> ret;
		std::for_each(group.begin(), group.end(), [&ret](auto process) 
		{
			ret = (*process)();
		});

		// In case everything went well return output of the last process
		return ret;
	}
	catch (const std::exception& e)
	{
		std::cout << "Process execution error: " << e.what() << std::endl;
		return {};
	}
}

