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

#include <boost/program_options.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <fstream>
#include <string>
#include <stdexcept>

#include "command_line.h"
#include "config.h"
#include "grammar.h"
#include "reactor.h"
#include "generator.h"
#include "synthesizer.h"
#include "token_provider.h"

auto skipper_t = ascii::blank;
using grammar_t = const tbxl_grammar<std::string::iterator, ascii::blank_type>;

int test_parser(grammar_t& g, std::string str)
{
	std::cout << "Testing: " << str << '\n';

	std::vector<boost::variant<int, bool>> v;
	std::string::iterator it = str.begin();
	if ((qi::phrase_parse(it, str.end(), g, skipper_t, v)) && (it == str.end()))
	{
		std::cout << "Parsing succeeded\n";
		return 0;
	}
	else
	{
		std::cout << "Parsing failed\n";
		return 3;
	}
}

std::string read_file_to_string(const std::string& name)
{
	std::ifstream in(name, std::ios::binary);
	in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

int main(int argc, char **argv)
{
	try
	{
		command_line cl(argc, argv);
		if(!cl.act())
		{
			return 1;
		}

		// Setup synthesizer
		std::ofstream out;
		token_provider tp;
		config cfg(tp);
		synthesizer s(out, cfg.get_indent(), '\n');
		cfg.set_number_interpretation(cl.get_param("number-type"), s);

		// Read input file
		std::cout << "Compiling file '" << cl.get_param("input-file") << "' into '" << cl.get_param("output-file") << "'\n";
		std::string program = read_file_to_string(cl.get_param("input-file"));
		boost::trim(program);

		// Setup output file
		out.open(cl.get_param("output-file"));

		// Generate
		generator gen(out, cfg);
		reactor r(gen);
		grammar_t g(r);
		return test_parser(g, program);
	}
	catch(const std::ifstream::failure& e)
	{
		std::cout << "FILE ACCESS ERROR: " << e.what() << std::endl;
		return 1;
	}
	catch(const std::exception& e)
	{
		std::cout << "GENERAL ERROR: " << e.what() << std::endl;
		return 2;
	}
}

