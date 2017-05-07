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

#include "number_type_integer.h"

number_type_integer::number_type_integer():	number_type_base(MY_SIZE)
{
}

std::string number_type_integer::get_initializer() const
{
	return "dta a(0)";
}

std::string number_type_integer::get_initializer(const std::string& number) const
{
	return "dta a(" + number + ")";
}

