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

#include "runtime_base.h"

class runtime_integer: public runtime_base
{
	// Functions specific to integer runtime
	void synth_BCDByte2Ascii() const;
	void synth_INBUFP_INIT() const;

protected:
	// Override of the pure interface
	virtual void synth_implementation() const;
	virtual void synth_COMPARE_NUMBER() const;
	virtual void synth_TRUE_FALSE() const;
	virtual void synth_BADD() const;
	virtual void synth_BSUB() const;
	virtual void synth_BMUL() const;
	virtual void synth_BDIV() const;
	virtual void synth_FASC() const;
	virtual void synth_COMPARE_FR0_FR1() const;
	virtual void synth_FR0_boolean_invert() const;
	virtual void synth_Is_FR0_true() const;
	virtual void synth_helpers() const;

public:
	runtime_integer(char endline, synthesizer& _synth, const config& _tp);
};