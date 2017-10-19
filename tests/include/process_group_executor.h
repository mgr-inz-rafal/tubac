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

#pragma once

#include "process_executor.h"
#ifdef _WIN32
#include <optional>
#else
#include <experimental/optional>
#endif

class process_group_executor
{
	std::vector<process_executor*> group;

public:
	explicit process_group_executor(std::vector<process_executor*> _group);
#ifdef _WIN32
	std::optional<std::string> run();
#else
	std::experimental::optional<std::string> run();
#endif
};
