#include "zero_page_token.h"

zero_page_token::zero_page_token(const std::string& label_): variable_token(label_)
{};

bool zero_page_token::should_spawn_on_zero_page() const
{
	return true;
}
