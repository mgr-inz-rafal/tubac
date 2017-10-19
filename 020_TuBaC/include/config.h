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
	explicit config(const token_provider& _tp);
	char get_endline() const;
	const std::string& get_indent() const;
	const token_provider& get_token_provider() const;

	void set_number_interpretation(const std::string& ni, synthesizer& s);
	std::shared_ptr<number_type_base> get_number_interpretation() const;
	std::shared_ptr<runtime_base> get_runtime() const;
};
