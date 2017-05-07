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

class stack
{
	std::string name;
	std::string pointer;
	int item_size;
	int capacity;
public:
	stack(const std::string& _name, int _item_size, int _capacity);
	const std::string& get_name() const;
	const std::string& get_pointer() const;
	int get_capacity() const;
	int get_item_size() const;
};

