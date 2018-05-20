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

#include <boost/format.hpp>

#include <sstream>
#include <iostream>

#include "generator.h"
#include "synthesizer.h"
#include "algorithm.h"

// Synth Indentation
#define SI synth.synth() <<	

// Synth Normal
#define SN synth.synth(false) <<

// Synth with Config
#define SC ss << cfg.get_indent() <<

generator::generator(std::ostream& _stream, const config& _cfg):
	cfg(_cfg),
	E_(cfg.get_endline()), 
	pokey_initialized(false),
	synth(_stream, _cfg.get_indent(), E_)
{
	loop_context.push(LOOP_CONTEXT::OUTSIDE);

	write_atari_registers();
	write_atari_constants();

	write_code_header();
	write_internal_variables();
	
	register_generator_runtime();
}

generator::~generator()
{
	write_code_footer();

	write_runtime();
	write_run_segment();
}

const std::string& generator::token(const token_provider::TOKENS& token) const
{
	return cfg.get_token_provider().get(token);
}

void generator::write_code_header() const {
	SN token(token_provider::TOKENS::PROGRAM_START) << " equ $" << std::hex << PROGRAM_START << std::dec << E_;
	SI "org " << token(token_provider::TOKENS::PROGRAM_START) << E_;
	SN ".zpvar = $" << std::hex << ZERO_PAGE_START << std::dec << E_;
	
	SI "mva #10 PTABW" << E_;

	write_stacks_initialization();
}

void generator::write_stacks() const
{
	for(const auto& s: stacks)
	{
		SI "; STACK: " << s.second.get_name() << E_;
		SN s.second.get_name() << E_;
		SN ':' << s.second.get_capacity() << cfg.get_indent() << "dta ";
		call_n(cfg.get_number_interpretation()->get_size()-1, [&]{SN "b(0),";});
		SN "b(0)" << E_;
	}
}

void generator::write_stacks_initialization() const {
	for (const auto& s : stacks)
	{
		spawn_compiler_variable(s.second.get_pointer(), true);
		init_pointer(s.second.get_pointer(), s.second.get_name());
	}
}

void generator::write_run_segment() const
{
	// Synth run address
	SI "org RUNAD" << E_;
	SI "dta a(" << token(token_provider::TOKENS::PROGRAM_START) << ')' << E_;
}

void generator::write_code_footer() const
{
	// Infinite loop at the end so the processor
	// won't fall into wilderness
	SN token(token_provider::TOKENS::PROGRAM_END) << " jmp " << token(token_provider::TOKENS::PROGRAM_END) << E_;

	// Prepare internal data and structures
	write_integers();
	write_variables();
	write_string_literals();
	write_stacks();
}

void generator::write_integers() const
{
	SN "; Fixed integers" << E_;

	for (auto& i : integers)
	{
		SN token(token_provider::TOKENS::INTEGER);
		if ('-' == i[0])
		{
			SN "NEG_" << i.substr(1);
		}
		else
		{
			SN i;
		}
		SN E_;
		SI cfg.get_number_interpretation()->get_initializer(i) << E_;
	}
}

void generator::write_variables() const
{
	SN "; Variables" << E_;

	for (auto& i : variables)
	{
		SN token(token_provider::TOKENS::VARIABLE) << i << E_;
		SI cfg.get_number_interpretation()->get_initializer() << E_;
	}
}

void generator::write_string_literals() const
{
	SN "; String literals" << E_;

	for (auto& [literal, index] : string_literals)
	{
		SN token(token_provider::TOKENS::STRING_LITERAL_LENGTH) << index << E_;
		SI "dta a(" << literal.size() << ')' << E_;
		if(!literal.empty()) // Do not synthesize empty string literals
		{
			SN token(token_provider::TOKENS::STRING_LITERAL) << index << E_;
			SI "dta c'";
			std::for_each(literal.begin(), literal.end(), [&](auto c){ SN c; });
			SN '\'' << E_;
		}
	}
}

void generator::new_integer(const std::string& i)
{
	integers.insert(i);
}

void generator::new_variable(const std::string& v)
{
	variables.insert(v);
}

int generator::new_string_literal(const std::vector<char>& s)
{
	if(s.size() > MAXIMUM_STRING_LITERAL_LENGTH)
	{
		// TODO: Prepare better reporting mechanism that
		// includes more context, line number, etc.
		std::cout << "Warning: string literal too long" << std::endl;
	}
	string_literals.insert({s, string_literal_id});
	std::stringstream ss;
	ss << token(token_provider::TOKENS::STRING_LITERAL) << string_literals.find(s)->second;
	//SI "jsr PUTSTRINGLITERAL" << E_;
	return string_literal_id++;
}

void generator::new_line(const int& i) const
{
	SN token(token_provider::TOKENS::LINE_INDICATOR) << i << E_;
}

void generator::write_atari_registers() const
{
	SN "; ATARI registers" << E_;
	for (const auto& [name, addr]: ATARI_REGISTERS)
	{
		SN name << " equ $" << std::hex << addr <<std::dec << E_;
	}
}

void generator::write_atari_constants() const
{
	SN "; ATARI constants" << E_;
	for (const auto& [name, value] : ATARI_CONSTANTS)
	{
		SN name << " equ $" << std::hex << value << std::dec << E_;
	}
}

void generator::put_integer_on_stack(const std::string& i) const {
	SN "; Put integer '" << i << "' on stack" << E_;

	SI "mwa #" << i << " FR0" << E_;
	push_from("FR0");
}

void generator::pop_to(const std::string& target, const generator::STACK& stack) const {
	SN "; Pop from stack (" << stacks.at(stack).get_name() << ") into '" << target << '\'' << E_;
	
	// Do the pop
	SI "mwa #" << target << ' ' << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI "mwa #" << stacks.at(stack).get_pointer() << ' ' << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << E_;
	SI "jsr POP_TO" << E_;
}

void generator::peek_to(const std::string& target, const generator::STACK& stack) const {
	SN "; Peek from stack (" << stacks.at(stack).get_name() << ") into '" << target << '\'' << E_;

	// Do the pop
	SI "mwa #" << target << ' ' << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI "mwa #" << stacks.at(stack).get_pointer() << ' ' << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << E_;
	SI "jsr PEEK_TO" << E_;
}

void generator::pop_to_variable(const std::string& target) const {
	SN "; Pop from stack into variable '" << target << '\'' << E_;
	pop_to(token(token_provider::TOKENS::VARIABLE) + target);
}

void generator::push_from(const std::string& source, const generator::STACK& stack) const {
	SN "; Push from '" << source << "' to stack (" << stacks.at(stack).get_name() << ')' << E_;

	// Do the push
	SI "mwa #" << source << ' ' << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI "mwa #" << stacks.at(stack).get_pointer() << ' ' << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << E_;
	SI "jsr PUSH_FROM" << E_;
}

void generator::push_from_variable(const std::string& source) const {
	SN "; Push from variable '" << source << "\' into stack" << E_;
	push_from(token(token_provider::TOKENS::VARIABLE) + source);
}

void generator::write_internal_variables() const {
	spawn_compiler_variable(token(token_provider::TOKENS::PUSH_POP_TARGET_STACK_PTR), true);
	spawn_compiler_variable(token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC), true);
	spawn_compiler_variable(token(token_provider::TOKENS::PUSH_POP_VALUE_PTR), true);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_LEFT_BASE), true);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX), false);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX), false);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_LEFT_PTR), true);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_RIGHT_BASE), false);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_RIGHT_FIRST_INDEX), false);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_RIGHT_SECOND_INDEX), false);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_RIGHT_PTR), true);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_ASSIGNMENT_COUNTER), true);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_PRINTED_LENGTH), false);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_CMP_LEFT_PTR), true);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_CMP_RIGHT_PTR), true);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_CMP_LEFT_LENGTH), false);
	spawn_compiler_variable(token(token_provider::TOKENS::STRING_CMP_RIGHT_LENGTH), false);
}

void generator::spawn_compiler_variable(const std::string& name, bool zero_page) const {
	SN "; Creating compiler variable '" << name << '\'';
	if (zero_page)
	{
		SN " on ZP";
	}
	SN E_;

	SN "." <<
		(zero_page ? "zp" : "")
		<< "var " << name << " .word" << E_;
}

void generator::init_pointer(const std::string& name, const std::string& source) const {
	SN "; Init pointer '" << name << "' with address of '" << source << '\'' << E_;
	SI "lda <" << source << E_;
	SI "sta " << name << E_;
	SI "lda >" << source << E_;
	SI "sta " << name << "+1" << E_;
}

void generator::addition() const {
	SN "; Execute addition (FR0 + FR1). Result stored in FR0" << E_;
	SI "jsr BADD" << E_;
}

void generator::subtraction() const {
	SN "; Execute subtraction (FR0 - FR1). Result stored in FR0" << E_;
	SI "jsr BSUB" << E_;
}

void generator::multiplication() const {
	SN "; Execute multiplication (FR0 * FR1). Result stored in FR0" << E_;
	SI "jsr BMUL" << E_;
}

void generator::division() const {
	SN "; Execute division (FR0 / FR1). Result stored in FR0" << E_;
	SI "jsr BDIV" << E_;
}

void generator::logical_and() const {
	SN "; Execute logical and (FR0 AND FR1). Result stored in FR0" << E_;
	SI "jsr LOGICAL_AND" << E_;
}

void generator::logical_or() const {
	SN "; Execute logical or (FR0 OR FR1). Result stored in FR0" << E_;
	SI "jsr LOGICAL_OR" << E_;
}

void generator::binary_xor() const {
	SN "; Execute binary exclusive or (FR0 EXOR FR1). Result stored in FR0" << E_;
	SI "jsr BINARY_XOR" << E_;
}

void generator::binary_and() const {
	SN "; Execute binary and (FR0 & FR1). Result stored in FR0" << E_;
	SI "jsr BINARY_AND" << E_;
}

void generator::binary_or() const {
	SN "; Execute binary or (FR0 ! FR1). Result stored in FR0" << E_;
	SI "jsr BINARY_OR" << E_;
}

void generator::compare_equal() const {
	SN "; Comparing FR0 and FR1 for equality" << E_;
	SI "lda #0" << E_;
	SI "sta INTEGER_COMPARE_TMP" << E_;
	SI "jsr COMPARE_FR0_FR1" << E_;
}

void generator::compare_less() const {
	SN "; Comparing FR0 and FR1 for less" << E_;
	SI "lda #1" << E_;
	SI "sta INTEGER_COMPARE_TMP" << E_;
	SI "jsr COMPARE_FR0_FR1" << E_;
}

void generator::compare_greater() const {
	SN "; Comparing FR0 and FR1 for greater" << E_;
	SI "lda #-1" << E_;
	SI "sta INTEGER_COMPARE_TMP" << E_;
	SI "jsr COMPARE_FR0_FR1" << E_;
}

void generator::assign_to_array(const std::string& a) const {
	SI "mwa #" << token(token_provider::TOKENS::INTEGER_ARRAY) << a << "+4 ARRAY_ASSIGNMENT_TMP_ADDRESS" << E_;
	SI "mwa " << token(token_provider::TOKENS::INTEGER_ARRAY) << a << " ARRAY_ASSIGNMENT_TMP_SIZE" << E_;
	SI "jsr INIT_ARRAY_OFFSET" << E_;
	SI "ldy #" << cfg.get_number_interpretation()->get_size()-1 << E_;
	SN "@" << E_;
	SI "lda ARRAY_ASSIGNMENT_TMP_VALUE,y" << E_;
	SI "sta (ARRAY_ASSIGNMENT_TMP_ADDRESS),y" << E_;
	SI "dey" << E_;
	SI "cpy #-1" << E_;
	SI "bne @-" << E_;
}

void generator::retrieve_from_array(const std::string& a) const {
	SI "mwa #" << token(token_provider::TOKENS::INTEGER_ARRAY) << a << "+4 ARRAY_ASSIGNMENT_TMP_ADDRESS" << E_;
	SI "mwa " << token(token_provider::TOKENS::INTEGER_ARRAY) << a << " ARRAY_ASSIGNMENT_TMP_SIZE" << E_;
	SI "jsr INIT_ARRAY_OFFSET" << E_;
	SI "ldy #" << cfg.get_number_interpretation()->get_size()-1 << E_;
	SN "@" << E_;
	SI "lda (ARRAY_ASSIGNMENT_TMP_ADDRESS),y" << E_;
	SI "sta FR0,y" << E_;
	SI "dey" << E_;
	SI "cpy #-1" << E_;
	SI "bne @-" << E_;
}

void generator::random() const {
	SI "jsr PUT_RANDOM_IN_FR0" << E_;
}

void generator::FP_to_ASCII() const {
	SI "jsr FASC" << E_;
}

void generator::init_print() const {
	SI R"(
	lda PTABW
	sta AUXBR
	dec AUXBR
	lda #0
	sta COX
)";
}

void generator::print_LBUFF() const {
	SN "; Printing string located at LBUFF" << E_;
	SI "jsr PUTSTRING" << E_;
}

void generator::FR0_boolean_invert() const {
	SN "; Inverting logical (boolean) value stored in FR0" << E_;
	SI "jsr FR0_boolean_invert" << E_;
}

void generator::goto_line(const int& i) const {
	SN "; Go to line " << i << E_;
	SI "jmp " << token(token_provider::TOKENS::LINE_INDICATOR) << i << E_;
}

void generator::gosub(const int& i) const {
	SN "; Go sub line " << i << E_;
	SI "jsr " << token(token_provider::TOKENS::LINE_INDICATOR) << i << E_;
}

void generator::gosub(const std::string& s) const {
	SN "; Go sub procedure " << s << E_;
	SI "jsr " << token(token_provider::TOKENS::PROCEDURE) << s << E_;
}

void generator::write_runtime() const {
	SN "; Here come the compiler runtime functions" << E_;
	cfg.get_runtime()->synth_implementation();
}

void generator::sound()
{
	if(!pokey_initialized)
	{
		pokey_initialized = true;
		SI "jsr POKEY_INIT" << E_;
	}
	SI "jsr SOUND" << E_;
}

void generator::poke() const {
	SI "jsr POKE" << E_;
}

void generator::dpoke() const {
	SI "jsr DPOKE" << E_;
}

void generator::peek() const {
	SI "jsr PEEK" << E_;
}

void generator::dpeek() const {
	SI "jsr DPEEK" << E_;
}

void generator::stick() const {
	SI "jsr STICK" << E_;
}

void generator::strig() const {
	SI "jsr STRIG" << E_;
}

void generator::after_if()
{
	// If this particular if didn't have ELSE statement, we need
	// to synth it just before the ENDIF
	if(ifs_with_else.find(stack_if.top()) == ifs_with_else.end())
	{
		SN token(token_provider::TOKENS::INSIDE_IF_INDICATOR) << stack_if.top() << E_;
	}
	SN token(token_provider::TOKENS::AFTER_IF_INDICATOR) << stack_if.top() << E_;
	stack_if.pop();
}

void generator::inside_if()
{
	SI "jmp " << token(token_provider::TOKENS::AFTER_IF_INDICATOR) << stack_if.top() << E_;
	SN token(token_provider::TOKENS::INSIDE_IF_INDICATOR) << stack_if.top() << E_;
	ifs_with_else.insert(stack_if.top());
}

void generator::skip_if_on_false()
{
	pop_to("FR0");
	stack_if.push(counter_after_if++);
	SN "; Skip execution if logical value is false " << E_;
	SI "jsr Is_FR0_true" << E_;
	SI "cmp #0" << E_;
	SI "jeq " << token(token_provider::TOKENS::INSIDE_IF_INDICATOR) << (counter_after_if - 1) << E_;
}

void generator::for_loop_condition()
{
	last_generic_label = get_next_generic_label();

	pop_to("FR0");
	push_from("FR0", generator::STACK::FOR_CONDITION);

	SI "mwa #" << last_generic_label << " FR0" << E_;
	push_from("FR0", generator::STACK::RETURN_ADDRESS_STACK);
}

void generator::for_step(bool default_step) const {
	if (default_step)
	{
		// Use "1"
		SI "mwa #1 FR0" << E_;
	}
	else
	{
		// Calculated value is on the expression stack
		pop_to("FR0");
	}
	push_from("FR0", generator::STACK::FOR_STEP);
	SN last_generic_label << E_;
}

void generator::for_loop_counter(const std::string& counting_variable)
{
	loop_context.push(LOOP_CONTEXT::FOR);
	SI "mwa #" << token(token_provider::TOKENS::VARIABLE) + counting_variable << " FR0" << E_;
	push_from("FR0", generator::STACK::FOR_COUNTER);
}

std::string generator::get_next_generic_label()
{
	return (boost::format("%1%_%2%") % token(token_provider::TOKENS::GENERIC_LABEL) % counter_generic_label++).str();
}

void generator::next()
{
	// Peek loop counter
	peek_to("FR1", generator::STACK::FOR_COUNTER);

	// Peek loop step
	peek_to("FR0", generator::STACK::FOR_STEP);

	// Increase loop counter
	SI "ldy #0" << E_;
	SI "adw (FR1),y FR0,y (FR1),y" << E_;

	// Move the counter value into FR0
	SI "ldy #0" << E_;
	SI "mwa (FR1),y FR0" << E_;

	// Peek loop condition
	peek_to("FR1", generator::STACK::FOR_CONDITION);

	// Compare (less or equal)
	compare_greater();
	FR0_boolean_invert();

	// If less then peek address from RETURN_ADDRESS_STACK and jump
	SI "lda FR0" << E_;
	SI "cmp #1" << E_;
	SI "bne @+" << E_;
	peek_to("FR1", generator::STACK::RETURN_ADDRESS_STACK);

	//S "jmp (FR1)" << E_;		// DONE: Make safer jump via rts
	SI R"(
	sbw FR1 #1
	lda FR1+1
	pha
	lda FR1
	pha
	rts
)";

	// Otherwise quick-pop addresses from three related stacks and continue
	loop_context.pop();
	SN "@" << E_;
	SI "jsr CLEAR_FOR_LOOP_STACKS" << E_;
}

void generator::register_generator_runtime() const
{
	// TODO: Rework this "get_indent()-crap. Consider enabling synth() to user-provided streams.
	std::stringstream ss;
	ss << "CLEAR_FOR_LOOP_STACKS" << E_;
	SC "mwa #" << stacks.at(STACK::FOR_COUNTER).get_pointer() << ' ' << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << E_;
	SC "jsr FAKE_POP" << E_;
	SC "mwa #" << stacks.at(STACK::FOR_CONDITION).get_pointer() << ' ' << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << E_;
	SC "jsr FAKE_POP" << E_;
	SC "mwa #" << stacks.at(STACK::RETURN_ADDRESS_STACK).get_pointer() << ' ' << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << E_;
	SC "jsr FAKE_POP" << E_;
	SC "mwa #" << stacks.at(STACK::FOR_STEP).get_pointer() << ' ' << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << E_;
	SC "jsr FAKE_POP" << E_;
	SC "rts" << E_;

	cfg.get_runtime()->register_own_runtime_funtion(ss.str());
}

void generator::while_()
{
	loop_context.push(LOOP_CONTEXT::WHILE);
	stack_while.push(counter_while++);
	SN token(token_provider::TOKENS::WHILE_INDICATOR) << stack_while.top() << E_;
}

void generator::while_condition()
{
	pop_to("FR0");
	SI "jsr Is_FR0_true" << E_;
	SI "cmp #0" << E_;
	SI "jeq " << token(token_provider::TOKENS::AFTER_WHILE_INDICATOR) << stack_while.top() << E_;
}

void generator::wend()
{
	loop_context.pop();
	const auto while_count = stack_while.top();
	stack_while.pop();
	SI "jmp " << token(token_provider::TOKENS::WHILE_INDICATOR) << while_count << E_;
	SN token(token_provider::TOKENS::AFTER_WHILE_INDICATOR) << while_count << E_;
}

void generator::exit()
{
	switch(loop_context.top())
	{
	case LOOP_CONTEXT::OUTSIDE:
		throw std::runtime_error("EXIT found outside the loop");
	case LOOP_CONTEXT::FOR:
		SI "jmp @+" << E_;
		break;
	case LOOP_CONTEXT::WHILE:
		SI "jmp " << token(token_provider::TOKENS::AFTER_WHILE_INDICATOR) << stack_while.top() << E_;
		break;
	case LOOP_CONTEXT::REPEAT:
		SI "jmp " << token(token_provider::TOKENS::AFTER_REPEAT_INDICATOR) << stack_repeat.top() << E_;
		break;
	case LOOP_CONTEXT::DO:
		SI "jmp " << token(token_provider::TOKENS::AFTER_DO_INDICATOR) << stack_do.top() << E_;
		break;
	default:
		throw std::runtime_error("EXIT found in unknown loop context");
	}
}

void generator::repeat()
{
	loop_context.push(LOOP_CONTEXT::REPEAT);
	stack_repeat.push(counter_repeat++);
	SN token(token_provider::TOKENS::REPEAT_INDICATOR) << stack_repeat.top() << E_;
}

void generator::until()
{
	pop_to("FR0");
	SI "jsr Is_FR0_true" << E_;
	SI "cmp #0" << E_;
	SI "jeq " << token(token_provider::TOKENS::REPEAT_INDICATOR) << stack_repeat.top() << E_;
	SN token(token_provider::TOKENS::AFTER_REPEAT_INDICATOR) << stack_repeat.top() << E_;
	stack_repeat.pop();
}

void generator::do_()
{
	loop_context.push(LOOP_CONTEXT::DO);
	stack_do.push(counter_do++);
	SN token(token_provider::TOKENS::DO_INDICATOR) << stack_do.top() << E_;
}

void generator::loop()
{
	SI "jmp " << token(token_provider::TOKENS::DO_INDICATOR) << stack_do.top() << E_;
	SN token(token_provider::TOKENS::AFTER_DO_INDICATOR) << stack_do.top() << E_;
	stack_do.pop();
}

void generator::return_() const {
	SI "rts" << E_;
}

void generator::proc(const std::string& s)
{
	stack_procedure.push(s);
	SN token(token_provider::TOKENS::PROCEDURE) << s << E_;
}

void generator::end() const
{
	SI "jmp " << token(token_provider::TOKENS::PROGRAM_END) << E_;
}

void generator::print_newline() const
{
	SI "jsr PUTNEWLINE" << E_;
}

void generator::print_comma() const
{
	SI "jsr PUTCOMMA" << E_;
}

void generator::init_integer_array(const basic_array& arr) const
{
	// TODO: Check whether such array has already been declared
	// TODO: Rework this "get_indent()-crap. Consider enabling synth() to user-provided streams.
	std::stringstream ss;
	ss << get_integer_array_token(arr.get_name()) << E_;
	SC "dta a(" << arr.get_size(0)+1 << "),a(" << arr.get_size(1)+1 << ')' << E_;
	ss << ':' << ((arr.get_size(0)+1)*(arr.get_size(1)+1)) << cfg.get_indent() << cfg.get_number_interpretation()->get_initializer() << E_;

	cfg.get_runtime()->register_own_runtime_funtion(ss.str());
}

void generator::init_string_array(const basic_array& arr) const
{
	std::stringstream ss;
	ss << get_string_array_token(arr.get_name(), token_provider::TOKENS::STRING_ARRAY_CAPACITY) << E_;
	SC "dta a(" << arr.get_size(0) << ')' << E_;
	ss << get_string_array_token(arr.get_name(), token_provider::TOKENS::STRING_ARRAY_CURRENT) << E_;
	SC "dta a(0)" << E_;
	ss << get_string_array_token(arr.get_name(), token_provider::TOKENS::STRING_ARRAY_CONTENT) << E_;
	ss << ':' << arr.get_size(0) << " dta b(0)" << E_;
	cfg.get_runtime()->register_own_runtime_funtion(ss.str());
}

std::string generator::get_integer_array_token(const std::string& name) const
{
	return token(token_provider::TOKENS::INTEGER_ARRAY) + name;
}

std::string generator::get_string_array_token(const std::string& name, token_provider::TOKENS kind) const
{
	return token(kind) + name;
}

void generator::put_zero_in_FR0() const
{
	SI "jsr PUT_ZERO_IN_FR0" << E_;
}

void generator::init_string_variable_offsets(const std::string& name, context::ARRAY_ASSIGNMENT_SIDE side) const
{
	switch(side)
	{
	case context::ARRAY_ASSIGNMENT_SIDE::LEFT:
		SI "mwa #0 " << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << E_;
		SI "mwa " << get_string_array_token(name, token_provider::TOKENS::STRING_ARRAY_CAPACITY) << ' ' << token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX) << E_;
		SI "mwa #" << get_string_array_token(name, token_provider::TOKENS::STRING_ARRAY_CONTENT) << ' ' << token(token_provider::TOKENS::STRING_LEFT_BASE) << E_;
		break;
	case context::ARRAY_ASSIGNMENT_SIDE::RIGHT:
		SI "mwa #0 " << token(token_provider::TOKENS::STRING_RIGHT_FIRST_INDEX) << E_;
		SI "mwa " << get_string_array_token(name, token_provider::TOKENS::STRING_ARRAY_CAPACITY) << ' ' << token(token_provider::TOKENS::STRING_RIGHT_SECOND_INDEX) << E_;
		SI "mwa #" << get_string_array_token(name, token_provider::TOKENS::STRING_ARRAY_CONTENT) << ' ' << token(token_provider::TOKENS::STRING_RIGHT_BASE) << E_;
		break;
	}
}

void generator::init_string_literal_offsets(const context& ctx, context::ARRAY_ASSIGNMENT_SIDE side)
{
	int lid = ctx.get_last_string_literal_id();
	bool is_literal_empty;
	for (auto& [literal, index] : string_literals)
	{
		if(index == lid)
		{
			is_literal_empty = literal.empty();
		}
	}
	switch(side)
	{
	case context::ARRAY_ASSIGNMENT_SIDE::LEFT:
		if(is_literal_empty)
		{
			SI "mwa #0 " << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << E_;
			SI "mwa #0 " << token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX) << E_;
		}
		else
		{
			SI "mwa #0 " << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << E_;
			SI "mwa " << token(token_provider::TOKENS::STRING_LITERAL_LENGTH) << lid << ' ' << token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX) << E_;
			SI "mwa #" << token(token_provider::TOKENS::STRING_LITERAL) << lid << ' ' << token(token_provider::TOKENS::STRING_LEFT_BASE) << E_;
		}
		break;
	case context::ARRAY_ASSIGNMENT_SIDE::RIGHT:
		if(is_literal_empty)
		{
			SI "mwa #0 " << token(token_provider::TOKENS::STRING_RIGHT_FIRST_INDEX) << E_;
			SI "mwa #0 " << token(token_provider::TOKENS::STRING_RIGHT_SECOND_INDEX) << E_;
		}
		else
		{
			SI "mwa #0 " << token(token_provider::TOKENS::STRING_RIGHT_FIRST_INDEX) << E_;
			SI "mwa " << token(token_provider::TOKENS::STRING_LITERAL_LENGTH) << lid << ' ' << token(token_provider::TOKENS::STRING_RIGHT_SECOND_INDEX) << E_;
			SI "mwa #" << token(token_provider::TOKENS::STRING_LITERAL) << lid << ' ' << token(token_provider::TOKENS::STRING_RIGHT_BASE) << E_;
		}
		break;
	}
}

void generator::do_string_assignment() const
{
	SI "jsr DO_STRING_ASSIGNMENT" << E_;
}

void generator::print_string(const std::string& name) const
{
	if(!name.empty())
	{
		SI "mwa " << token(token_provider::TOKENS::STRING_ARRAY_CURRENT) << name << ' ' << token(token_provider::TOKENS::STRING_PRINTED_LENGTH) << E_;
	}
	SI "jsr PRINT_STRING" << E_;
}

void generator::decrease_word(const std::string& ptr) const
{
	SI "dew " << ptr << E_;
}

void generator::put_byte_in_variable(const std::string& name, int value) const
{
	SI "mva #" << value << ' ' << name << E_;
}

// TODO: Tokenize!
void generator::do_string_comparison() const
{
	SI "#if .word ___TUBAC___STRING_COMPARISON_LEFT_LENGTH = #0 .or .word ___TUBAC___STRING_COMPARISON_RIGHT_LENGTH = #0" << E_;
	SI "jsr DO_STRING_COMPARISON_WITH_EMPTY_LITERAL" << E_;
	SI "#else" << E_;
	SI "jsr DO_STRING_COMPARISON" << E_;
	SI "#end" << E_;
}

void generator::string_comparison_handler(context& ctx)
{
	std::string length;
	std::string first_index;
	std::string second_index;
	switch(ctx.get_string_assignment_array_side())
	{
	case context::ARRAY_ASSIGNMENT_SIDE::LEFT:
		length			= token(token_provider::TOKENS::STRING_CMP_LEFT_LENGTH);
		first_index		= token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX);
		second_index	= token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX);
		break;
	case context::ARRAY_ASSIGNMENT_SIDE::RIGHT:
		length			= token(token_provider::TOKENS::STRING_CMP_RIGHT_LENGTH);
		first_index		= token(token_provider::TOKENS::STRING_RIGHT_FIRST_INDEX);
		second_index	= token(token_provider::TOKENS::STRING_RIGHT_SECOND_INDEX);
		break;
	}

	if(-1 != ctx.get_last_string_literal_id())
	{
		SI "mwa " << token(token_provider::TOKENS::STRING_LITERAL_LENGTH) << ctx.get_last_string_literal_id() << ' ' << length << E_;
	}
	else if(!ctx.get_last_string_literal_id())
	{
		SI "mwa " << token(token_provider::TOKENS::STRING_ARRAY_CURRENT) << ctx.string_array_get().get_name() << ' ' << length << E_;
		SI "sbw " << length << ' ' << first_index << E_;
	}
	else
	{
		SI "mwa " << second_index << ' ' << length << E_;
		SI "sbw " << length << ' ' << first_index << E_;
	}
}

void generator::init_string_comparison_type(context::COMPARE_TYPE compare) const
{
	switch(compare)
	{
	case context::COMPARE_TYPE::EQUAL:
		SI "mva #0 STRING_COMPARISON_TYPE" << E_;
		break;
	case context::COMPARE_TYPE::GREATER:
		SI "mva #1 STRING_COMPARISON_TYPE" << E_;;
		break;
	case context::COMPARE_TYPE::LESS:
		SI "mva #2 STRING_COMPARISON_TYPE" << E_;;
		break;
	default: 
		std::cout << "Internal compiler error: Unknown compare type: " << static_cast<int>(compare) << std::endl;
	}
}

void generator::inputline() const
{
	SI "jsr INPUTLINE" << E_;
}

void generator::emplace_input_buffer(bool into_string, const std::string& var_name) const
{
	if(into_string)
	{
		SI "jsr EMPLACE_INPUT_BUFFER_INTO_STRING" << E_;
	}
	else
	{
		SI "jsr EMPLACE_INPUT_BUFFER_INTO_INTEGER" << E_;
	}
}

#undef SI
#undef SN
#undef SC
