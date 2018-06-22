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
#include "context.h"

#include <string>
#include <vector>

class reactor
{
	generator& _g;
	context ctx;

	// TODO: Move these three to "context"
	std::string variable_recently_assigned_to;
	bool recent_for_had_step;
	bool last_printed_token_was_separator;

	void reset_context();
	void init_string_comparison();

public:
	explicit reactor(generator& g);

	void got_line_number(const int& i);
	void got_command_separator();
	void got_asterisk() const;
	void got_slash() const;
	void got_plus() const;
	void got_minus() const;
	void got_logical_and() const;
	void got_logical_or() const;
	void got_binary_xor() const;
	void got_binary_and() const;
	void got_binary_or() const;
	void got_compare_equal() const;
	void got_compare_not_equal() const;
	void got_compare_less() const;
	void got_compare_greater_equal() const;
	void got_compare_greater() const;
	void got_compare_less_equal() const;
	void got_integer(int i) const;
	void got_string_literal(const std::vector<char>& vec);
	void got_print_expression();
	void got_goto_integer(const int& i) const;
	void got_gosub_integer(const int& i) const;
	void got_sound() const;
	void got_variable_to_assign(const std::string& s);
	void got_integer_array_to_assign();
	void got_integer_array_to_retrieve();
	void got_integer_array_first_dimension();
	void got_integer_array_second_dimension();
	void got_integer_array_name(const std::string& s);
	void got_integer_array_size(int i);
	void got_integer_array_size_2(int i);
	void got_integer_array_declaration_finished();
	void got_string_array_name(const std::string& s);
	void got_string_array_size(int i);
	void got_string_array_declaration_finished();
	void got_string_variable_name(const std::string& s);
	void got_string_array_first_dimension();
	void got_string_array_second_dimension();
	void got_string_variable_before_dimensions();
	void got_string_literal_for_assignment() const;
	void got_string_variable_for_assignment() const;
	void got_print_string_literal();
	void got_print_string_variable();
	void got_array_declaration();
	void got_variable_to_retrieve(const std::string& s) const;
	void got_poke() const;
	void got_dpoke() const;
	void got_peek() const;
	void got_dpeek() const;
	void got_stick() const;
	void got_strig() const;
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
	void got_print();
	void got_after_print() const;
	void got_input() const;
	void got_inputable_separator();
	void got_integer_variable_to_input(const std::string& s);
	void got_after_input();
	void got_execute_array_assignment();
	void got_execute_string_array_assignment();
	void got_random() const;
	void got_not() const;
	void got_len();
	void got_string_comparison();
	void got_string_comparison_not_equal();
	void got_string_comparison_less_or_equal();
	void got_string_comparison_greater_or_equal();
	void got_string_comparison_greater();
	void got_string_comparison_less();
	void got_string_comparison_equal();
	void got_string_literal_for_comparison() const;
	void got_data_element(const std::vector<char>& vec);
	void got_read() const;
	void got_after_read();
};
