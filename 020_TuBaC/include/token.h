#pragma once

#include <string>

class token
{
	const std::string TOKEN_INDICATOR = "___TUBAC___";
	std::string label;
	bool spawn_as_variable;
	bool spawn_on_zero_page;
public:
	token(const std::string& label_);
	const std::string& get_label() const;
	virtual bool should_spawn() const;
	virtual bool should_spawn_on_zero_page() const;
};
