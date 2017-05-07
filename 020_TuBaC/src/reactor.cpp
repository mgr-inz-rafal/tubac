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

#include "reactor.h"

#include <iostream>
#include <sstream>

reactor::reactor(generator& g) : _g(g) {};

void reactor::got_line_number(const int& i) const
{
	_g.new_line(i);
}

void reactor::got_asterisk() const
{
	std::cout << "MUL" << std::endl;
	_g.pop_to("FR1");
	_g.pop_to("FR0");
	_g.multiplication();
	_g.push_from("FR0");
}

void reactor::got_slash() const
{
	std::cout << "DIV" << std::endl;
	_g.pop_to("FR1");
	_g.pop_to("FR0");
	_g.division();
	_g.push_from("FR0");
}

void reactor::got_plus() const
{
	std::cout << "ADD" << std::endl;
	_g.pop_to("FR1");
	_g.pop_to("FR0");
	_g.addition();
	_g.push_from("FR0");
}

void reactor::got_minus() const
{
	std::cout << "SUB" << std::endl;
	_g.pop_to("FR1");
	_g.pop_to("FR0");
	_g.subtraction();
	_g.push_from("FR0");
}

void reactor::got_compare_equal() const
{
	std::cout << "EQ" << std::endl;
	_g.pop_to("FR1");
	_g.pop_to("FR0");
	_g.compare_equal();
	_g.push_from("FR0");
}

void reactor::got_compare_not_equal() const
{
	std::cout << "NEQ" << std::endl;
	_g.pop_to("FR1");
	_g.pop_to("FR0");
	_g.compare_equal();
	_g.FR0_boolean_invert();
	_g.push_from("FR0");
}

void reactor::got_compare_less() const
{
	std::cout << "LESS" << std::endl;
	_g.pop_to("FR1");
	_g.pop_to("FR0");
	_g.compare_less();
	_g.push_from("FR0");
}

void reactor::got_compare_greater_equal() const
{
	std::cout << "GREATER EQUAL" << std::endl;
	_g.pop_to("FR1");
	_g.pop_to("FR0");
	_g.compare_less();
	_g.FR0_boolean_invert();
	_g.push_from("FR0");
}

void reactor::got_compare_greater() const
{
	std::cout << "GREATER" << std::endl;
	_g.pop_to("FR1");
	_g.pop_to("FR0");
	_g.compare_greater();
	_g.push_from("FR0");
}

void reactor::got_compare_less_equal() const
{
	std::cout << "LESS EQUAL" << std::endl;
	_g.pop_to("FR1");
	_g.pop_to("FR0");
	_g.compare_greater();
	_g.FR0_boolean_invert();
	_g.push_from("FR0");
}

void reactor::got_integer(int i)
{
	std::cout << "INTEGER: " << i << "..." << std::endl;

	_g.new_integer(std::to_string(i));
	_g.put_integer_on_stack(std::to_string(i));
}

void reactor::got_print_expression() const
{
	std::cout << "PRINT EXPRESSION" << std::endl;
	_g.pop_to("FR0");
	_g.FP_to_ASCII();
	_g.print_LBUFF();
}

void reactor::got_goto_integer(const int& i) const
{
	std::cout << "GOTO INTEGER " << i << std::endl;
	_g.goto_line(i);
}

void reactor::got_gosub_integer(const int& i) const
{
	std::cout << "GOSUB INTEGER " << i << std::endl;
	_g.gosub(i);
}

void reactor::got_variable_to_assign(const std::string& s)
{
	std::cout << "ASSIGN TO VARIABLE " << s << std::endl;
	variable_recently_assigned_to = s;
	_g.new_variable(s);
	_g.pop_to_variable(s);
}

void reactor::got_variable_to_retrieve(const std::string& s) const
{
	std::cout << "RETRIEVE FROM VARIABLE " << s << std::endl;
	_g.push_from_variable(s);
}

void reactor::got_sound() const
{
	std::cout << "SOUND" << std::endl;
	_g.sound();
}

void reactor::got_poke() const
{
	std::cout << "POKE" << std::endl;
	_g.poke();
}

void reactor::got_peek() const
{
	std::cout << "PEEK" << std::endl;
	_g.peek();
}

void reactor::got_for()
{
	std::cout << "FOR" << std::endl;
	recent_for_had_step = false;
	_g.for_loop_counter(variable_recently_assigned_to);
}

void reactor::got_to() const
{
	std::cout << "TO" << std::endl;
	_g.for_loop_condition();
}

void reactor::got_step()
{
	std::cout << "STEP" << std::endl;
	recent_for_had_step = true;
}

void reactor::got_after_for() const
{
	std::cout << "AFTER FOR" << std::endl;
	_g.for_step(!recent_for_had_step);
}

void reactor::got_next() const
{
	std::cout << "NEXT" << std::endl;
	_g.next();
}

void reactor::got_if() const
{
	std::cout << "IF" << std::endl;
	_g.skip_if_on_false();
}

void reactor::got_then() const
{
	std::cout << "THEN" << std::endl;
	_g.after_if();
}

void reactor::got_else() const
{
	std::cout << "ELSE" << std::endl;
	_g.inside_if();
}

void reactor::got_endif() const
{
	std::cout << "ENDIF" << std::endl;
	_g.after_if();
}

void reactor::got_while() const
{
	std::cout << "WHILE" << std::endl;
	_g.while_();
}

void reactor::got_while_condition() const
{
	std::cout << "WHILE CONDITION" << std::endl;
	_g.while_condition();
}

void reactor::got_wend() const
{
	std::cout << "WEND" << std::endl;
	_g.wend();
}

void reactor::got_exit() const
{
	std::cout << "EXIT" << std::endl;
	_g.exit();
}

void reactor::got_repeat() const
{
	std::cout << "REPEAT" << std::endl;
	_g.repeat();
}

void reactor::got_until() const
{
	std::cout << "UNTIL" << std::endl;
	_g.until();
}

void reactor::got_do() const
{
	std::cout << "DO" << std::endl;
	_g.do_();
}

void reactor::got_loop() const
{
	std::cout << "LOOP" << std::endl;
	_g.loop();
}

void reactor::got_return() const
{
	std::cout << "RETURN" << std::endl;
	_g.return_();
}

void reactor::got_exec(const std::string& s) const
{
	std::cout << "EXEC " << s << std::endl;
	_g.gosub(s);
}

void reactor::got_proc(const std::string& s) const
{
	std::cout << "PROC " << s << std::endl;
	_g.proc(s);
}

void reactor::got_endproc() const
{
	std::cout << "ENDPROC" << std::endl;
	_g.return_();
}

void reactor::got_end() const
{
	_g.end();
}
