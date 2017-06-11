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

#include <ostream>

class synthesizer
{
	std::ostream& stream;
	const std::string& INDENT;
	const char E_;

public:
	synthesizer(std::ostream& _stream, const std::string& _INDENT, char endline);
	std::ostream& synth(bool indent = true) const;
};