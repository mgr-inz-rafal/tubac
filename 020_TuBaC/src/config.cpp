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

#include "config.h"

#include <stdexcept>
#include <memory>

#include "number_type_integer.h"
#include "runtime_integer.h"
#include "synthesizer.h"

config::config(const token_provider& _tp): tp(_tp)
{
}

char config::get_endline() const
{
	return endline;
}

const std::string& config::get_indent() const
{
	return indentation;
}

void config::set_number_interpretation(const std::string& ni, synthesizer& s)
{
	if("integer" == ni)
	{
		number_type = std::make_shared<number_type_integer>();
		runtime_type = std::make_shared<runtime_integer>(get_endline(), s, *this);
		return;
	}
	else if("fixed" == ni)
	{
		throw std::invalid_argument("'fixed' numbers not supported");
	}
	else if("floating" == ni)
	{
		throw std::invalid_argument("'floating point' numbers not supported");
	}
	throw std::invalid_argument("unknown number interpretation");
}

std::shared_ptr<number_type_base> config::get_number_interpretation() const
{
	return number_type;
}

std::shared_ptr<runtime_base> config::get_runtime() const
{
	return runtime_type;
}

const token_provider& config::get_token_provider() const
{
	return tp;
}
