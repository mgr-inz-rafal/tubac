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

// ..\boost_1_66_0\boost\iterator.hpp(16): error C4996: 'std::iterator': warning STL4015: The std::iterator class template
// (used as a base class to provide typedefs) is deprecated in C++17. (The <iterator> header is NOT deprecated.)
// The C++ Standard has never required user-defined iterators to derive from std::iterator. To fix this warning, stop
// deriving from std::iterator and start providing publicly accessible typedefs named iterator_category, value_type,
// difference_type, pointer, and reference. Note that value_type is required to be non-const, even for constant iterators.
// You can define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING or _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 
// to acknowledge that you have received this warning. (compiling source file src\020_TuBaC.cpp)
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

//error C4996 : 'std::fpos<_Mbstatet>::seekpos' : warning STL4019 : The member std::fpos::seekpos() is non - Standard, and is
//preserved only for compatibility with workarounds for old versions of Visual C++.It will be removed in a future release, and in
//this release always returns 0. Please use standards - conforming mechanisms to manipulate fpos, such as conversions to and from
//streamoff, or an integral type, instead.If you are receiving this message while compiling Boost.IOStreams,
//a fix has been submitted upstream to make Boost use standards - conforming mechanisms, as it does for other compilers.
//You can define _SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING to acknowledge that you have received this warning,
//or define _REMOVE_FPOS_SEEKPOS to remove std::fpos::seekpos entirely.
#define _SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING

#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <stdexcept>
#include <string>

#include "command_line.h"
#include "config.h"
#include "generator.h"
#include "grammar.h"
#include "reactor.h"
#include "synthesizer.h"
#include "token_provider.h"

auto skipper_t = ascii::blank;
using grammar_t = const tbxl_grammar<std::string::iterator, ascii::blank_type>;

int test_parser(grammar_t& g, std::string str)
{
	std::cout << "Testing: " << str << '\n';

	std::vector<boost::variant<int, bool>> v;
	auto it = str.begin();
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
		const token_provider tp;
		config cfg(tp);
		synthesizer s(out, cfg.get_indent(), '\n');
		cfg.set_number_interpretation(cl.get_param("number-type"), s);

		// Read input file
		std::cout << "Compiling file '" << cl.get_param("input-file") << "' into '" << cl.get_param("output-file") << "'\n";
		auto program = read_file_to_string(cl.get_param("input-file"));
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

