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

#include "number_type_base.h"

class number_type_integer : public number_type_base
{
	static const int MY_SIZE = 2;

public:
	number_type_integer();

	std::string get_initializer() const override;
	std::string get_initializer(const std::string& number) const override;
};