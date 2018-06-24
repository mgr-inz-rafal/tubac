#include "token.h"

token::token(const std::string& label_): label(label_)
{}


const std::string& token::get_label() const
{
	return label;
}
