#pragma once

#include <string>

class token
{
	std::string label;
	bool spawn_as_variable;
	bool spawn_on_zero_page;
public:
	token(const std::string& label_, bool spawn_as_variable_ = false, bool spawn_on_zero_page_ = false);
	const std::string& get_label() const;
	bool should_spawn() const;
	bool should_spawn_on_zero_page() const;
};
