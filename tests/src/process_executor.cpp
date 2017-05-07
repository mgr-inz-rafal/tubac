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

#include "process_executor.h"

#include <sstream>
#include <thread>

#include <boost/format.hpp>

#include "exceptions.h"

const std::chrono::milliseconds process_executor::default_timeout = 0s;
const std::array<int, 2> process_executor::allowed_return_codes = { 0, 259 }; // 259 for forcefully terminated emulator (on Windows!)

process_executor::process_executor(
	const std::string& _binary_name,
	const std::vector<std::string>& _arguments,
	const std::chrono::milliseconds& _timeout)
		:binary_name(_binary_name), arguments(_arguments), timeout(_timeout)
{
}

std::string process_executor::operator()()
{
	bp::child child(
		bp::exe = binary_name,
		bp::args = arguments,
		bp::std_out > outstream,
		bp::std_in < instream);
	if(timeout != default_timeout)
	{
		std::error_code ec = std::make_error_code(std::io_errc::stream); // TODO: Introduce custom error category (http://www.cplusplus.com/reference/system_error/error_category/)
		std::async(std::launch::async, [&]() {
			std::this_thread::sleep_for(timeout);
			child.terminate(ec);});
	}
	child.join();
	if (succeeded(child.exit_code()))
	{
		return parse_output();
	}
	throw process_error((boost::format("Process \"%1%\" failed to execute (error code = %2%)") % binary_name % child.exit_code()).str());
}

std::string process_executor::parse_output()
{
	std::string tmp_line;
	std::stringstream tmp_stream; 

	while(std::getline(outstream, tmp_line))
	{
		tmp_stream << tmp_line << std::endl;
	}
	return tmp_stream.str();
}

bool process_executor::succeeded(int code)
{
	return std::find(allowed_return_codes.begin(), allowed_return_codes.end(), code) != allowed_return_codes.end();
}
