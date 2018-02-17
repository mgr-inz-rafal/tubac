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
	explicit tbxl_grammar(reactor& r): tbxl_grammar::base_type{ program }, _r(r)
	{
		line_number = qi::int_
			[
				boost::bind(&reactor::got_line_number, &r, _1)
			];
		commands = (command % qi::string(":")
			[
				boost::bind(&reactor::got_command_separator, &r)
			]);
		line = line_number >> commands;
		program = +(line % qi::eol);

		hex_integer = '$' >> qi::hex
			[
				boost::bind(&reactor::got_integer, &r, _1)
			];
		
		// Arithmetic expressions
		expr_factor =
			NOT
			|
			RND
			|
			PEEK
			|
			DPEEK
			|
			STICK
			|
			STRIG
			|
			expr_array
				[
					boost::bind(&reactor::got_integer_array_to_retrieve, &r)
				]
			|
			qi::int_
				[
					boost::bind(&reactor::got_integer, &r, _1)
				]
			|
			hex_integer
			|
			integer_variable_name
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
			expr_factor >> *(
			('*' >> expr_factor)
				[
					boost::bind(&reactor::got_asterisk, &r)
				]
			|
			('/' >> expr_factor)
				[
					boost::bind(&reactor::got_slash, &r)
				]
			);
		expr =
			expr_terminals >> *(
				('+' >> expr_terminals)
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
			|
				("AND" >> expr_terminals)
				[
					boost::bind(&reactor::got_logical_and, &r)
				]
			|
				("OR" >> expr_terminals)
				[
					boost::bind(&reactor::got_logical_or, &r)
				]
			|
				("EXOR" >> expr_terminals)
				[
					boost::bind(&reactor::got_binary_xor, &r)
				]
			|
				("&" >> expr_terminals)
				[
					boost::bind(&reactor::got_binary_and, &r)
				]
			|
				("!" >> expr_terminals)
				[
					boost::bind(&reactor::got_binary_or, &r)
				]
			);

		// Variables
		integer_variable_name = qi::alpha >> *(qi::alnum);

		string_variable_name = integer_variable_name >> '$';
		
		string_literal = ('"' >> *(~qi::char_('"')) >> '"')
				[
					boost::bind(&reactor::got_string_literal, &r, _1)
				];

		string_assignment = -LET >> (string_variable_name >> '=' >> string_literal)
				[
					boost::bind(&reactor::got_string_variable_to_assign, &r, ::_1)
				];
		integer_assignment = -LET >> (integer_variable_name >> '=' >> expr)
				[
					boost::bind(&reactor::got_variable_to_assign, &r, ::_1)
				];

		integer_array_assignment = (-LET >> expr_array >> qi::string("=")
				[
					boost::bind(&reactor::got_execute_array_assignment, &r)
				]
				>> expr)
				[
					boost::bind(&reactor::got_integer_array_to_assign, &r)
				];

		expr_array = (integer_variable_name >> '(' >> expr
				[
					boost::bind(&reactor::got_integer_array_first_dimension, &r)
				]
				>> -(',' >> expr)
				[
					boost::bind(&reactor::got_integer_array_second_dimension, &r)
				]
				>> ')')
				[
					boost::bind(&reactor::got_integer_array_name, &r, ::_1)
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

		GOTO = (qi::string("GOTO") | (qi::string("GO") >> qi::string("TO"))) >> qi::int_
			[
				boost::bind(&reactor::got_goto_integer, &r, _1)
			];

		POKE = (qi::string("POKE") >> expr >> ',' >> expr)
			[
				boost::bind(&reactor::got_poke, &r)
			];

		DPOKE = (qi::string("DPOKE") >> expr >> ',' >> expr)
			[
				boost::bind(&reactor::got_dpoke, &r)
			];

		PEEK = (qi::string("PEEK(") >> expr >> ')')
			[
				boost::bind(&reactor::got_peek, &r)
			];

		DPEEK = (qi::string("DPEEK(") >> expr >> ')')
			[
				boost::bind(&reactor::got_dpeek, &r)
			];

		STICK = (qi::string("STICK(") >> expr >> ')')
			[
				boost::bind(&reactor::got_stick, &r)
			];

		STRIG = (qi::string("STRIG(") >> expr >> ')')
			[
				boost::bind(&reactor::got_strig, &r)
			];

		FOR = (qi::string("FOR")
			>> integer_assignment
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

		NEXT = (qi::string("NEXT") >> integer_variable_name)
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

		EXEC = (qi::string("EXEC") >> integer_variable_name
			[
				boost::bind(&reactor::got_exec, &r, ::_1)
			]);

		PROC = (qi::string("PROC") >> integer_variable_name
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

		array_declaration = string_array_declaration || integer_array_declaration;

		string_array_declaration = 
			(string_variable_name
			[
				boost::bind(&reactor::got_string_array_name, &r, ::_1)
			]
			>> '(' >> qi::int_
			[
				boost::bind(&reactor::got_string_array_size, &r, ::_1)
			]
			>> ')')			
			[
				boost::bind(&reactor::got_string_array_declaration_finished, &r)
			];
;

		integer_array_declaration = 
			(integer_variable_name
			[
				boost::bind(&reactor::got_integer_array_name, &r, ::_1)
			]
			>> '(' >> qi::int_
			[
				boost::bind(&reactor::got_integer_array_size, &r, ::_1)
			]
			>>
			-(',' >> qi::int_)
			[
				boost::bind(&reactor::got_integer_array_size_2, &r, ::_1)
			]
			>> ')')
			[
				boost::bind(&reactor::got_integer_array_declaration_finished, &r)
			];

		DIM = (((qi::string("DIM") || qi::string("COM"))
			[
				boost::bind(&reactor::got_array_declaration, &r)
			]
			) >> (array_declaration % ','));

		RND = (qi::string("RND") >> -(('(' >> expr >> ')')))
			[
				boost::bind(&reactor::got_random, &r)
			];

		NOT = (qi::string("NOT") >> expr)
			[
				boost::bind(&reactor::got_not, &r)
			];

		command =
			(string_assignment)				|
			(integer_assignment)			|
			(integer_array_assignment)		|
			(PRINT)							|
			(SOUND)							|
			(POKE)							|
			(DPOKE)							|
			(FOR)							|
			(NEXT)							|
			(IF)							|
			(ENDIF)							|
			(ELSE)							|
			(ENDIF)							|
			(WHILE)							|
			(WEND)							|
			(EXIT)							|
			(REPEAT)						|
			(UNTIL)							|
			(DO)							|
			(LOOP)							|
			(GOSUB)							|
			(RETURN)						|
			(PROC)							|
			(ENDPROC)						|
			(EXEC)							|
			(END)							|
			(LET)							|
			(DIM)							|
			(NOT)							|
			(GOTO);
	}

	qi::rule<Iterator, Skipper> line_number;
	qi::rule<Iterator, std::string()> hex_integer;
	qi::rule<Iterator, Skipper> expr;
	qi::rule<Iterator, Skipper> expr_factor;
	qi::rule<Iterator, Skipper> expr_terminals;
	qi::rule<Iterator, Skipper> expr_array;
	qi::rule<Iterator, std::string()> integer_variable_name;
	qi::rule<Iterator, std::string()> string_variable_name;
	qi::rule<Iterator, Skipper> integer_assignment;
	qi::rule<Iterator, Skipper> integer_array_assignment;
	qi::rule<Iterator, Skipper> string_assignment;
	qi::rule<Iterator, Skipper> command;
	qi::rule<Iterator, Skipper> commands;
	qi::rule<Iterator, Skipper> command_terminator;
	qi::rule<Iterator, Skipper> line;
	qi::rule<Iterator, Skipper> program;
	qi::rule<Iterator, Skipper> printable;
	qi::rule<Iterator, Skipper> printable_separator;
	qi::rule<Iterator, Skipper> array_declaration;
	qi::rule<Iterator, Skipper> integer_array_declaration;
	qi::rule<Iterator, Skipper> string_array_declaration;
	qi::rule<Iterator, Skipper> string_literal;

	// TBXL commands
	qi::rule<Iterator, Skipper> PRINT;
	qi::rule<Iterator, Skipper> SOUND;
	qi::rule<Iterator, Skipper> GOTO;
	qi::rule<Iterator, Skipper> POKE;
	qi::rule<Iterator, Skipper> DPOKE;
	qi::rule<Iterator, Skipper> PEEK;
	qi::rule<Iterator, Skipper> DPEEK;
	qi::rule<Iterator, Skipper> STICK;
	qi::rule<Iterator, Skipper> STRIG;
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
	qi::rule<Iterator, Skipper> RND;
	qi::rule<Iterator, Skipper> NOT;
};


