#pragma once

#include <ostream>

class synthesizer
{
	std::ostream& stream;
	const std::string& INDENT;
	const char E_;

public:
	synthesizer(std::ostream& _stream, const std::string& _INDENT, char endline);
	std::ostream& synth(bool indent = true);
};