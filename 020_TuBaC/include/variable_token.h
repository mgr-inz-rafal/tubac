#pragma once

#include "token.h"

class variable_token: public token
{
public:
	variable_token(const std::string& label_);
	virtual bool should_spawn() const override;
};