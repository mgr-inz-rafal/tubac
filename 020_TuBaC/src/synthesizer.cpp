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

#include "synthesizer.h"

#include <ostream>
#include <string>

synthesizer::synthesizer(std::ostream& _stream, const std::string& _INDENT, char endline):
	stream(_stream), INDENT(_INDENT), E_(endline)
{
}

std::ostream& synthesizer::synth(bool indent) const
{
	if (indent)
	{
		stream << INDENT;
	}
	return stream;
}
