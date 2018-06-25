#pragma once

#include "variable_token.h"

class zero_page_token: public variable_token
{
public:
	zero_page_token(const std::string& label_);
	virtual bool should_spawn_on_zero_page() const override;
};