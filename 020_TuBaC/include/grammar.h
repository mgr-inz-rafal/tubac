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

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

#include <functional>
#include <iostream>
#include <string>

#include "reactor.h"

namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;

// boost::bind() is used instead of std::bind()
// Rationale:
//	1. This correct code won't compile in Visual Studio 2015
//     throwing "error C2338: tuple index out of bounds"
//        std::bind(&reactor::got_integer, &r, std::placeholders::_1)
//  2. It could be worked around by wrapping
//     a call inside std::function like this
//		  std::function<void(const int&)>(std::bind(&reactor::got_integer, &r, std::placeholders::_1))
//  3. Therefore I'll stick to the more compact boost approach
//		  boost::bind(&reactor::got_integer, &r, _1)

template <typename Iterator, typename Skipper>
struct tbxl_grammar : qi::grammar<Iterator, Skipper>
{
	reactor& _r;
	tbxl_grammar(reactor& r): _r(r), tbxl_grammar::base_type{ program }
	{
		line_number = qi::int_
			[
				boost::bind(&reactor::got_line_number, &r, _1)
			];
		commands = (command % ':');
		line = line_number >> commands;
		program = +(line % qi::eol);

		hex_integer = '$' >> qi::hex
			[
				boost::bind(&reactor::got_integer, &r, _1)
			];
		
		// Arithmetic expressions
		expr_factor =
			PEEK
			|
			qi::int_
				[
					boost::bind(&reactor::got_integer, &r, _1)
				]
			|
			hex_integer
			|
			variable_name
				[
					boost::bind(&reactor::got_variable_to_retrieve, &r, _1)
				]
			|
			'(' >> expr >> ')'
			|
			('-' >> expr_factor)
				[
					boost::bind(&reactor::got_minus, &r)
				]
			|
			('+' >> expr_factor)
				[
					boost::bind(&reactor::got_plus, &r)
				];
		expr_terminals = 
			expr_factor >> *(('*' >> expr_factor)
				[
					boost::bind(&reactor::got_asterisk, &r)
				]
			|
			('/' >> expr_factor)
				[
					boost::bind(&reactor::got_slash, &r)
				]);
		expr =
			expr_terminals >> *(('+' >> expr_terminals)
				[
					boost::bind(&reactor::got_plus, &r)
				]
			|
				('-' >> expr_terminals)
				[
					boost::bind(&reactor::got_minus, &r)
				]
			|
				('=' >> expr_terminals)
				[
					boost::bind(&reactor::got_compare_equal, &r)
				]
			|
				("<>" >> expr_terminals)
				[
					boost::bind(&reactor::got_compare_not_equal, &r)
				]
			|
				("<" >> expr_terminals)
				[
					boost::bind(&reactor::got_compare_less, &r)
				]
			|
				(">=" >> expr_terminals)
				[
					boost::bind(&reactor::got_compare_greater_equal, &r)
				]
			|
				(">" >> expr_terminals)
				[
					boost::bind(&reactor::got_compare_greater, &r)
				]
			|
				("<=" >> expr_terminals)
				[
					boost::bind(&reactor::got_compare_less_equal, &r)
				]
			);

		// Variables
		variable_name = qi::alpha >> *(qi::alnum);

		assignment = -LET >> (variable_name >> '=' >> expr)
				[
					boost::bind(&reactor::got_variable_to_assign, &r, ::_1)
				];

		printable_separator =
			(qi::string(";")
				[
					boost::bind(&reactor::got_separator_semicolon, &r)
				]
			|
			qi::string(",")
				[
					boost::bind(&reactor::got_separator_comma, &r)
				]);

		printable = expr
				[
					boost::bind(&reactor::got_print_expression, &r)
				]
				|| printable_separator;

		// TBXL commands
		PRINT = (qi::string("PRINT")
				[
					boost::bind(&reactor::got_print, &r)
				]
					>> *printable)
				[
					boost::bind(&reactor::got_after_print, &r)
				];

		SOUND = (qi::string("SOUND") >> boost::spirit::repeat(3)[expr >> ','] >> expr)
			[
				boost::bind(&reactor::got_sound, &r)
			];

		GOTO = qi::string("GOTO") >> qi::int_
			[
				boost::bind(&reactor::got_goto_integer, &r, _1)
			];

		POKE = (qi::string("POKE") >> expr >> ',' >> expr)
			[
				boost::bind(&reactor::got_poke, &r)
			];

		PEEK = (qi::string("PEEK(") >> expr >> ')')
			[
				boost::bind(&reactor::got_peek, &r)
			];

		FOR = (qi::string("FOR")
			>> assignment
				[
					boost::bind(&reactor::got_for, &r)
				]
			>> (qi::string("TO")
			>> expr
				[
					boost::bind(&reactor::got_to, &r)
				])
			>> -(qi::string("STEP")
			>> expr
				[
					boost::bind(&reactor::got_step, &r)
				]))
			[
				boost::bind(&reactor::got_after_for, &r)
			];

		NEXT = (qi::string("NEXT") >> variable_name)
			[
				boost::bind(&reactor::got_next, &r)
			];

		WHILE = (qi::string("WHILE")
			[
				boost::bind(&reactor::got_while, &r)
			] >> expr)
			[
				boost::bind(&reactor::got_while_condition, &r)
			];

		WEND = qi::string("WEND")
			[
				boost::bind(&reactor::got_wend, &r)
			];

		REPEAT = qi::string("REPEAT")
			[
				boost::bind(&reactor::got_repeat, &r)
			];

		UNTIL = (qi::string("UNTIL") >> expr)
			[
				boost::bind(&reactor::got_until, &r)
			];

		DO = qi::string("DO")
			[
				boost::bind(&reactor::got_do, &r)
			];

		LOOP = qi::string("LOOP")
			[
				boost::bind(&reactor::got_loop, &r)
			];

		IF = (qi::string("IF") >> expr
			[
				boost::bind(&reactor::got_if, &r)
			]
			>> -(
				(qi::string("THEN") >> commands)
					[
						boost::bind(&reactor::got_then, &r)
					]
				|
				(':' >> commands)
				)
			);

		ELSE = qi::string("ELSE")
			[
				boost::bind(&reactor::got_else, &r)
			];

		ENDIF = qi::string("ENDIF")
			[
				boost::bind(&reactor::got_endif, &r)
			];

		EXIT = qi::string("EXIT")
			[
				boost::bind(&reactor::got_exit, &r)
			];

		GOSUB = (qi::string("GOSUB") >> qi::int_
			[
				boost::bind(&reactor::got_gosub_integer, &r, _1)
			]);

		RETURN = qi::string("RETURN")
			[
				boost::bind(&reactor::got_return, &r)
			];

		EXEC = (qi::string("EXEC") >> variable_name
			[
				boost::bind(&reactor::got_exec, &r, ::_1)
			]);

		PROC = (qi::string("PROC") >> variable_name
			[
				boost::bind(&reactor::got_proc, &r, ::_1)
			]);

		ENDPROC = qi::string("ENDPROC")
			[
				boost::bind(&reactor::got_endproc, &r)
			];

		END = qi::string("END")
			[
				boost::bind(&reactor::got_end, &r)
			];

		LET = qi::string("LET");

		DIM = (qi::string("DIM ") >> variable_name
			[
				boost::bind(&reactor::got_single_dim_integer_array, &r, ::_1)
			]
			>> '(' >> qi::int_
			[
				boost::bind(&reactor::got_single_dim_integer_array_size, &r, ::_1)
			]
			>> ')');

		command =
			(assignment)		|
			(PRINT)				|
			(SOUND)				|
			(POKE)				|
			(FOR)				|
			(NEXT)				|
			(IF)				|
			(ENDIF)				|
			(ELSE)				|
			(ENDIF)				|
			(WHILE)				|
			(WEND)				|
			(EXIT)				|
			(REPEAT)			|
			(UNTIL)				|
			(DO)				|
			(LOOP)				|
			(GOSUB)				|
			(RETURN)			|
			(PROC)				|
			(ENDPROC)			|
			(EXEC)				|
			(END)				|
			(LET)				|
			(DIM)				|
			(GOTO);
	}

	qi::rule<Iterator, Skipper> line_number;
	qi::rule<Iterator, std::string()> hex_integer;
	qi::rule<Iterator, Skipper> expr;
	qi::rule<Iterator, Skipper> expr_factor;
	qi::rule<Iterator, Skipper> expr_terminals;
	qi::rule<Iterator, std::string()> variable_name;
	qi::rule<Iterator, Skipper> assignment;
	qi::rule<Iterator, Skipper> command;
	qi::rule<Iterator, Skipper> commands;
	qi::rule<Iterator, Skipper> command_terminator;
	qi::rule<Iterator, Skipper> line;
	qi::rule<Iterator, Skipper> program;
	qi::rule<Iterator, Skipper> printable;
	qi::rule<Iterator, Skipper> printable_separator;

	// TBXL commands
	qi::rule<Iterator, Skipper> PRINT;
	qi::rule<Iterator, Skipper> SOUND;
	qi::rule<Iterator, Skipper> GOTO;
	qi::rule<Iterator, Skipper> POKE;
	qi::rule<Iterator, Skipper> PEEK;
	qi::rule<Iterator, Skipper> FOR;
	qi::rule<Iterator, Skipper> NEXT;
	qi::rule<Iterator, Skipper> WHILE;
	qi::rule<Iterator, Skipper> WEND;
	qi::rule<Iterator, Skipper> IF;
	qi::rule<Iterator, Skipper> ELSE;
	qi::rule<Iterator, Skipper> ENDIF;
	qi::rule<Iterator, Skipper> EXIT;
	qi::rule<Iterator, Skipper> REPEAT;
	qi::rule<Iterator, Skipper> UNTIL;
	qi::rule<Iterator, Skipper> DO;
	qi::rule<Iterator, Skipper> LOOP;
	qi::rule<Iterator, Skipper> GOSUB;
	qi::rule<Iterator, Skipper> RETURN;
	qi::rule<Iterator, Skipper> EXEC;
	qi::rule<Iterator, Skipper> PROC;
	qi::rule<Iterator, Skipper> ENDPROC;
	qi::rule<Iterator, Skipper> END;
	qi::rule<Iterator, Skipper> LET;
	qi::rule<Iterator, Skipper> DIM;
};


