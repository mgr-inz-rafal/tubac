#pragma once

#include <string>

class token
{
	std::string label;
public:
	token(const std::string& label_);
	const std::string& get_label() const;
};
