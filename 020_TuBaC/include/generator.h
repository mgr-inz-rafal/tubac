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

#include <set>
#include <map>
#include <stack>
#include <vector>

#include "config.h"
#include "synthesizer.h"
#include "token_provider.h"
#include "stack.h"
#include "basic_array.h"
#include "context.h"

class generator
{
public:
	enum class STACK
	{
		EXPRESSION,
		FOR_CONDITION,
		FOR_COUNTER,
		RETURN_ADDRESS_STACK,
		FOR_STEP
	};

	enum class LOOP_CONTEXT
	{
		OUTSIDE,
		FOR,
		WHILE,
		REPEAT,
		DO
	};

private:
	const config& cfg;

	const int EXPRESSION_STACK_CAPACITY = 64;
	const int RETURN_ADDRESS_STACK_CAPACITY = 64;
	const int FOR_LOOP_STACK_CAPACITY = 16;
	const unsigned int MAXIMUM_STRING_LITERAL_LENGTH = 254;	// TODO: Allow 255 (now the legth includes single byte with length of the literal

	const int ZERO_PAGE_START = 0x80;
	const int PROGRAM_START = 0x2000;
	const int POINTER_SIZE = 2;
	const std::map<std::string, int> ATARI_REGISTERS = {
		{ "RUNAD",		0x02E0 },
		{ "FR0",		0x00D4 },
		{ "FR1",		0x00E0 },
		{ "LBUFF",		0x0580 },
		{ "INBUFP",		0x00F3 },
		{ "ICCOM",		0x0342 },
		{ "ICBAL",		0x0344 },
		{ "ICBLL",		0x0348 },
		{ "CIOV",		0xE456 },
		{ "AUDF1",		0xD200 },
		{ "AUDC1",		0xD201 },
		{ "SKCTL",		0xD20F },
		{ "SSKCTL",		0x0232 },
		{ "RANDOM",		0xD20A },
		{ "STICK0",		0x0278 },
		{ "STRIG0",		0x0284 }
	};
	const std::map<std::string, int> ATARI_CONSTANTS = {
		{ "PUTCHR",		0x000B },
		{ "EOL",		0x009B }
	};

	const std::map<STACK, stack> stacks = {
		{ STACK::EXPRESSION, stack(
			token(token_provider::TOKENS::EXPRESSION_STACK),
			cfg.get_number_interpretation()->get_size(),
			EXPRESSION_STACK_CAPACITY) },
		{ STACK::FOR_CONDITION, stack(
			token(token_provider::TOKENS::FOR_CONDITION_STACK),
			POINTER_SIZE,
			FOR_LOOP_STACK_CAPACITY) },
		{ STACK::FOR_COUNTER, stack(
			token(token_provider::TOKENS::FOR_COUNTER_STACK),
			POINTER_SIZE,
			FOR_LOOP_STACK_CAPACITY) },
		{ STACK::RETURN_ADDRESS_STACK, stack(
			token(token_provider::TOKENS::RETURN_ADDRESS_STACK),
			POINTER_SIZE,
			RETURN_ADDRESS_STACK_CAPACITY) },
		{ STACK::FOR_STEP, stack(
			token(token_provider::TOKENS::FOR_STEP_STACK),
			POINTER_SIZE,
			FOR_LOOP_STACK_CAPACITY) }
	};

	///////////////////////////////////////////////////////////////////////////////////////////
	// TODO: Rework as loop control structures. Currently each
	// loop type has its own:
	// - counter
	// - stack

	// IF support structures
	int counter_after_if = 0;
	std::stack<int> stack_if;		
	std::set<int> ifs_with_else;	

	// WHILE support structures
	int counter_while = 0;
	std::stack<int> stack_while;	

	// REPEAT support structures
	int counter_repeat = 0;
	std::stack<int> stack_repeat;	

	// DO support structures
	int counter_do = 0;
	std::stack<int> stack_do;	

	// PROC support structures
	std::stack<std::string> stack_procedure;

	// Other support structures
	int counter_generic_label = 0;
	std::stack<LOOP_CONTEXT> loop_context;
	int string_literal_id = 0;
	///////////////////////////////////////////////////////////////////////////////////////////

	char E_;
	std::set<std::string> integers;
	std::set<std::string> variables;
	std::multimap<std::vector<char>, int> string_literals;	// TODO: Optimize for duplicated string literals
	bool pokey_initialized;

	synthesizer synth;

	void write_code_header() const;
	void write_code_footer() const;
	void register_generator_runtime() const;

	void write_stacks_initialization() const;
	void write_stacks() const;
	void write_integers() const;
	void write_variables() const;
	void write_string_literals() const;
	void write_atari_registers() const;
	void write_atari_constants() const;
	void write_internal_variables() const;
	void write_run_segment() const;

	void write_runtime() const;

	void spawn_compiler_variable(const std::string& name, bool zero_page) const;
	void init_pointer(const std::string& name, const std::string& source) const;

	const std::string& token(const token_provider::TOKENS& token) const;
	std::string get_next_generic_label();
	std::string last_generic_label;
	std::string get_integer_array_token(const std::string& name) const;
	std::string get_string_array_token(const std::string& name, token_provider::TOKENS kind) const;

public:
	generator(std::ostream& _stream, const config& _cfg);
	~generator();

	void new_integer(const std::string& i);
	int new_string_literal(const std::vector<char>& s);
	void new_variable(const std::string& v);
	void new_line(const int& i) const;
	void put_integer_on_stack(const std::string& i) const;
	void pop_to(const std::string& target, const generator::STACK& stack = generator::STACK::EXPRESSION) const;
	void pop_to_variable(const std::string& target) const;
	void peek_to(const std::string& target, const generator::STACK& stack = generator::STACK::EXPRESSION) const;
	void push_from(const std::string& source, const generator::STACK& stack = generator::STACK::EXPRESSION) const;
	void push_from_variable(const std::string& source) const;
	void FP_to_ASCII() const;
	void FR0_boolean_invert() const;
	void init_print() const;
	void print_LBUFF() const;
	void print_newline() const;
	void print_comma() const;
	void goto_line(const int& i) const;
	void gosub(const int& i) const;
	void gosub(const std::string& s) const;
	void sound();
	void poke() const;
	void dpoke() const;
	void peek() const;
	void dpeek() const;
	void stick() const;
	void strig() const;
	void after_if();
	void inside_if();
	void skip_if_on_false();
	void for_loop_condition();
	void for_loop_counter(const std::string& counting_variable);
	void for_step(bool default_step = false) const;
	void next();
	void while_();
	void while_condition();
	void wend();
	void exit();
	void repeat();
	void until();
	void do_();
	void loop();
	void return_() const;
	void proc(const std::string& s);
	void end() const;
	void init_integer_array(const basic_array& arr) const;
	void init_string_array(const basic_array& arr) const;
	void init_string_variable_offsets(const std::string& name, context::ARRAY_ASSIGNMENT_SIDE side) const;
	void put_zero_in_FR0() const;
	void addition() const;
	void subtraction() const;
	void multiplication() const;
	void division() const;
	void compare_equal() const;
	void compare_less() const;
	void compare_greater() const;
	void assign_to_array(const std::string& a) const;
	void retrieve_from_array(const std::string& a) const;
	void random() const;
	void logical_and() const;
	void logical_or() const;
	void binary_xor() const;
	void binary_and() const;
	void binary_or() const;
	void init_string_literal_offsets(const context& ctx, context::ARRAY_ASSIGNMENT_SIDE side = context::ARRAY_ASSIGNMENT_SIDE::RIGHT);
	void do_string_assignment() const;
	void print_string(const std::string& name) const;
	void decrease_word(const std::string& ptr) const;
	void put_byte_in_variable(const std::string& name, int value) const;
	void do_string_comparison() const;
	void tmp_00(const std::string& cs, bool two_dim, int literal_id, const context& ctx);
	void tmp_01(const std::string& cs, const bool is_two_dimensional, int is_literal, const context& ctx);
	void init_string_comparison_type(context::COMPARE_TYPE compare) const;
};

