#pragma once

#include <string>
#include <memory>

#include "number_type_base.h"
#include "runtime_base.h"
#include "token_provider.h"

class synthesizer;

class config
{
	const char endline = '\n';
	std::string indentation = "    ";
	std::shared_ptr<number_type_base> number_type;
	std::shared_ptr<runtime_base> runtime_type;
	const token_provider& tp;

public:
	config(const token_provider& _tp);
	char get_endline() const;
	const std::string& get_indent() const;
	const token_provider& get_token_provider() const;

	void set_number_interpretation(const std::string& ni, synthesizer& s);
	std::shared_ptr<number_type_base> get_number_interpretation() const;
	std::shared_ptr<runtime_base> get_runtime() const;
};
