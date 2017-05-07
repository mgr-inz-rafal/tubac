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

#include <boost/program_options.hpp>

#include <string>

namespace po = boost::program_options;

class command_line
{
	po::positional_options_description positional_args;
	po::options_description options = {"Allowed options"};
	po::options_description hidden_options;
	po::options_description all_options;
	po::variables_map vm;

public:
	command_line(int argc, char **argv);
	bool act();

	const std::string& get_param(const std::string& name) const;
};
