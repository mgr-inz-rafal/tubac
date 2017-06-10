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

#include "generator.h"

#include <string>

struct basic_array
{
	std::string name;
	int size_1;
	int size_2;

	void init()
	{
		name = "";
		size_1 = size_2 = 0;
	}
};

class reactor
{
	generator& _g;
	std::string variable_recently_assigned_to;
	bool recent_for_had_step;
	bool last_printed_token_was_separator;
	basic_array array_being_declared;
	bool assigning_to_two_dimensional_array;

public:
	reactor(generator& g);

	void got_line_number(const int& i) const;
	void got_asterisk() const;
	void got_slash() const;
	void got_plus() const;
	void got_minus() const;
	void got_compare_equal() const;
	void got_compare_not_equal() const;
	void got_compare_less() const;
	void got_compare_greater_equal() const;
	void got_compare_greater() const;
	void got_compare_less_equal() const;
	void got_integer(int i);
	void got_print_expression();
	void got_goto_integer(const int& i) const;
	void got_gosub_integer(const int& i) const;
	void got_sound() const;
	void got_variable_to_assign(const std::string& s);
	void got_integer_array_to_assign(const std::string& s);
	void got_integer_array_to_assign_first_dimension();
	void got_integer_array_to_assign_second_dimension();
	void got_integer_array_name(const std::string& s);
	void got_integer_array_size(int i);
	void got_integer_array_size_2(int i);
	void got_array_declaration();
	void got_array_declaration_finished();
	void got_variable_to_retrieve(const std::string& s) const;
	void got_poke() const;
	void got_peek() const;
	void got_for();
	void got_to() const;
	void got_step();
	void got_after_for() const;
	void got_next() const;
	void got_if() const;
	void got_else() const;
	void got_endif() const;
	void got_then() const;
	void got_while() const;
	void got_while_condition() const;
	void got_wend() const;
	void got_exit() const;
	void got_repeat() const;
	void got_until() const;
	void got_do() const;
	void got_loop() const;
	void got_return() const;
	void got_exec(const std::string& s) const;
	void got_proc(const std::string& s) const;
	void got_endproc() const;
	void got_end() const;
	void got_separator_semicolon();
	void got_separator_comma();
	void got_after_print() const;
	void got_print();
};
