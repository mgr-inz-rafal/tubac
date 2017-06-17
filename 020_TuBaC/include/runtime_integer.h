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
	void synth_implementation() const override;
	void synth_COMPARE_NUMBER() const override;
	void synth_TRUE_FALSE() const override;
	void synth_BADD() const override;
	void synth_BSUB() const override;
	void synth_BMUL() const override;
	void synth_BDIV() const override;
	void synth_FASC() const override;
	void synth_COMPARE_FR0_FR1() const override;
	void synth_FR0_boolean_invert() const override;
	void synth_Is_FR0_true() const override;
	void synth_PUT_ZERO_IN_FR0() const override;
	void synth_PUT_RANDOM_IN_FR0() const  override;
	void synth_helpers() const override;
	void synth_LOGICAL_AND() const override;
	void synth_LOGICAL_OR() const override;
	void synth_XOR() const override;

public:
	runtime_integer(char endline, synthesizer& _synth, const config& _tp);
};