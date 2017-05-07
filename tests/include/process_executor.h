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

#include <string>
#include <vector>
#include <chrono>
#include <array>
#include <system_error>

#include <boost/process.hpp>

namespace bp = boost::process;
using namespace std::chrono_literals;

class process_executor
{
	static const std::chrono::milliseconds default_timeout;
	static const std::array<int, 2> allowed_return_codes;

	const std::string& binary_name;
	bp::ipstream outstream;
	bp::opstream instream;
	std::vector<std::string> arguments;
	std::chrono::milliseconds timeout;

	std::string parse_output();
	bool succeeded(int code);

	template<typename T> friend process_executor& operator<<(process_executor& pr, const T& val);
	
public:
	process_executor(
		const std::string& _binary_name,
		const std::vector<std::string>& _arguments,
		const std::chrono::milliseconds& _timeout = default_timeout);
	std::string operator()();
};

template<typename T>
process_executor& operator<<(process_executor& pr, const T& val)
{
	pr.instream << val << std::flush;
	return pr;
}
