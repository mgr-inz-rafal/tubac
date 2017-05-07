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
#include <list>

#include "synthesizer.h"
#include "token_provider.h"

class config;

class runtime_base
{
	std::list<std::string> own_functions;
	void synth_own_functions() const;

protected:
	char E_;
	synthesizer& synth;
	const config& cfg;

	// *** These functions can be implemented    ***
	// *** in a common way for each runtime that ***
	// *** conforms to the assumptions about     ***
	// *** rendering its data in correct places  ***

	// Helpers
	virtual void synth_INIT_PUSH_POP_POINTER() const;
	virtual void synth_IsXY00() const;
	virtual void synth_POP_TO() const;
	virtual void synth_PEEK_TO() const;
	virtual void synth_FAKE_POP() const;
	virtual void synth_PUSH_FROM() const;

	// String printing
	virtual void synth_PUTCHAR() const;
	virtual void synth_PUTNEWLINE() const;
	virtual void synth_PUTSTRING() const;

	// POKEY routines
	virtual void synth_POKEY_INIT() const;
	virtual void synth_SOUND() const;

	// Memory manipulation
	virtual void synth_POKE() const;
	virtual void synth_PEEK() const;

	// *** These functions must be derived by each ***
	// *** runtime implementation                  ***
	virtual void synth_COMPARE_NUMBER() const = 0;
	virtual void synth_TRUE_FALSE() const = 0;
	virtual void synth_BADD() const = 0;
	virtual void synth_BSUB() const = 0;
	virtual void synth_BMUL() const = 0;
	virtual void synth_BDIV() const = 0;
	virtual void synth_FASC() const = 0;
	virtual void synth_COMPARE_FR0_FR1() const = 0;
	virtual void synth_FR0_boolean_invert() const = 0;
	virtual void synth_Is_FR0_true() const = 0;
	virtual void synth_helpers() const = 0;

	// Utility functions
	const std::string& token(const token_provider::TOKENS& token) const;

public:
	runtime_base(char endline, synthesizer& _synth, const config& _tp);
	virtual void synth_implementation() const = 0;
	virtual void register_own_runtime_funtion(const std::string& body);
};