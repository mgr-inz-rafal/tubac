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

#include "stack.h"

stack::stack(const std::string& _name, int _item_size, int _capacity):
	name(_name),
	pointer(_name + "_PTR"),
	item_size(_item_size),
	capacity(_capacity)
{}

const std::string& stack::get_name() const
{
	return name; 
}

const std::string& stack::get_pointer() const 
{ 
	return pointer; 
}

int stack::get_capacity() const
{
	return capacity;
}

int stack::get_item_size() const
{
	return item_size;
}
