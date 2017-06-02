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

#include "runtime_base.h"
#include "config.h"

runtime_base::runtime_base(char endline, synthesizer& _synth, const config& _config):
	E_(endline), synth(_synth), cfg(_config)
{
}

const std::string& runtime_base::token(const token_provider::TOKENS& token) const
{
	return cfg.get_token_provider().get(token);
}

// Synthesises various common functions
void runtime_base::synth_implementation() const
{
	synth_COMPARE_NUMBER();
	synth_COMPARE_FR0_FR1();
	synth_IsXY00();
	synth_TRUE_FALSE();
	synth_FR0_boolean_invert();
	synth_Is_FR0_true();
	synth_helpers();

	synth_BADD();
	synth_BSUB();
	synth_BMUL();
	synth_BDIV();
	synth_FASC();
	synth_PUTCHAR();
	synth_PUTNEWLINE();
	synth_PUTSPACE();
	synth_PUTSTRING();
	synth_PUTCOMMA();
	synth_SOUND();
	synth_POKEY_INIT();
	synth_POP_TO();
	synth_PEEK_TO();
	synth_PUSH_FROM();
	synth_INIT_PUSH_POP_POINTER();
	synth_POKE();
	synth_PEEK();
	synth_FAKE_POP();

	synth_own_functions();
}

/*
Outputs char present in A to screen via CIO
*/
void runtime_base::synth_PUTCHAR() const
{
	synth.synth() << R"(
PUTCHAR
	sta PUTCHAR_TO_OUTPUT
	lda #PUTCHR
	sta ICCOM
	lda #<PUTCHAR_TO_OUTPUT
	sta ICBAL
	lda #>PUTCHAR_TO_OUTPUT
	sta ICBAL+1
	lda #1
	sta ICBLL
	lda #0
	sta ICBLL+1
	ldx #0
	jsr CIOV
	inc COX
	#if .byte COX >= AUXBR
		lda AUXBR
		add PTABW
		sta AUXBR
	#end
	rts
.var PUTCHAR_TO_OUTPUT .byte
)";
}

/*
Outputs new line
*/
void runtime_base::synth_PUTNEWLINE() const
{
	synth.synth() << R"(
PUTNEWLINE
	lda #EOL
	jsr PUTCHAR
	rts
)";
}

void runtime_base::synth_PUTSPACE() const
{
	synth.synth() << R"(
PUTSPACE
	lda #' '
	jsr PUTCHAR
	rts
)";
}


/*
Outputs string located in LBUFF to the screen char-by-char by
using PUTCHAR. Last character must be inverted.
New line is printed at the end.
*/
void runtime_base::synth_PUTSTRING() const
{
	synth.synth() << R"(
PUTSTRING_MARK_END
	and #%10000000
	cmp #%10000000
	beq PUTSTRING_MARK_END_LABEL_0
	lda #0
	sta PUTSTRING_END
	lda LBUFF,y
	rts
PUTSTRING_MARK_END_LABEL_0
	lda #1
	sta PUTSTRING_END
	lda LBUFF,y
	and #%01111111
	rts
PUTSTRING
	ldy INBUFP
PUTSTRING_LOOP_0
	tya
	pha
	lda LBUFF,y
	jsr PUTSTRING_MARK_END
	jsr PUTCHAR
	lda PUTSTRING_END
	cmp #1
	beq PUTSTRING_LABEL_0
	pla
	tay
	iny
	jmp PUTSTRING_LOOP_0
PUTSTRING_LABEL_0
	pla
	rts
.var PUTSTRING_END .byte
)";
}

// Inspired by the following code in Python provided by Mono
//from __future__ import print_function
//ptabw = 10
//def basic_print(*items):
//	auxbr = ptabw
//	cox = 0
//	for text in items:
//		for ch in text:
//			print(ch, sep="", end="")
//			cox += 1
//			if cox >= auxbr:
//				auxbr += ptabw
//		for i in range(cox, auxbr):
//			print(" ", sep="", end="")
//			cox += 1
//			if cox >= auxbr:
//				auxbr += ptabw
//	print()
//	
//basic_print("","abc","","1234567890","!@#$")
void runtime_base::synth_PUTCOMMA() const
{
	synth.synth() << R"(
PUTCOMMA
	mva AUXBR AUXBRT
PUTCOMMA_LABEL_0
	#if .byte COX < AUXBRT
		jsr PUTSPACE
		jmp PUTCOMMA_LABEL_0
	#end
	rts
.var PTABW .byte
.var AUXBR .byte
.var COX .byte
.var AUXBRT .byte
)";
}

/*
Checks if both X and Y registers are 0.
Returns 1 in accumulator if so.
*/
void runtime_base::synth_IsXY00() const
{
	synth.synth() << R"(
IsXY00
	lda #1
	cpx #0
	bne IsXY00_LABEL_0
	cpy #0
	bne IsXY00_LABEL_0
	jmp IsXY00_LABEL_1
IsXY00_LABEL_0
	lda #0
IsXY00_LABEL_1
	rts
)";
}

/*
Reads 4 bytes from stack and put them in 
the POKEY registers.
*/
void runtime_base::synth_SOUND() const
{
	synth.synth(false) << "SOUND" << E_;
	synth.synth() << "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << "jsr POP_TO" << E_;
	synth.synth() << "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << R"(
	jsr POP_TO
	lda FR1
:4	asl
	eor FR0
	tax
)";
	synth.synth() << "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << "jsr POP_TO" << E_;
	synth.synth() << "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << R"(
	jsr POP_TO
	lda FR1
	asl
	tay
	lda FR0
	sta AUDF1,y
	txa
	sta AUDC1,y
	rts
)";
}

void runtime_base::synth_POKEY_INIT() const
{
	synth.synth() << R"(
POKEY_INIT
	ldx #8
	lda #0
	sta SKCTL
	sta SSKCTL
POKEY_INIT_CLEAR
	sta AUDF1,x
	dex
	bpl POKEY_INIT_CLEAR
	lda #3
	sta SKCTL
	sta SSKCTL
	rts
)";
}

void runtime_base::synth_POP_TO() const
{
	synth.synth(false) << "POP_TO" << E_;

	synth.synth() << "ldy #0" << E_;
	synth.synth() << "sbw (" << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << "),y #" << cfg.get_number_interpretation()->get_size() << E_;
	synth.synth() << "jsr INIT_PUSH_POP_POINTER" << E_;
	synth.synth() << "ldy #0" << E_;
	for (int j = 0; j < cfg.get_number_interpretation()->get_size(); ++j)
	{
		synth.synth() << "lda (" << token(token_provider::TOKENS::PUSH_POP_TARGET_STACK_PTR) << "), y" << E_;
		synth.synth() << "sta (" << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << "), y" << E_;
		if(!j)
		{
			synth.synth() << "iny" << E_;
		}
	}

	synth.synth() << "rts" << E_;
}

void runtime_base::synth_PEEK_TO() const
{
	synth.synth(false) << "PEEK_TO" << E_;
	synth.synth() << "jsr POP_TO" << E_;
	synth.synth() << "ldy #0" << E_;
	synth.synth() << "adw (" << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << "),y #" << cfg.get_number_interpretation()->get_size() << E_;

	synth.synth() << "rts" << E_;
}

void runtime_base::synth_FAKE_POP() const
{
	synth.synth(false) << "FAKE_POP" << E_;
	synth.synth() << "ldy #0" << E_;
	synth.synth() << "sbw (" << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << "),y #" << cfg.get_number_interpretation()->get_size() << E_;
	synth.synth() << "rts" << E_;
}

void runtime_base::synth_INIT_PUSH_POP_POINTER() const
{
	synth.synth(false) << "INIT_PUSH_POP_POINTER" << E_;
	synth.synth() << "ldy #0" << E_;
	synth.synth() << "mwa (" << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << "),y " << token(token_provider::TOKENS::PUSH_POP_TARGET_STACK_PTR) << E_;
	synth.synth() << "rts" << E_;
}

void runtime_base::synth_PUSH_FROM() const
{
	synth.synth(false) << "PUSH_FROM" << E_;
	synth.synth() << "jsr INIT_PUSH_POP_POINTER" << E_;
	synth.synth() << "ldy #0" << E_;
	for (int j = 0; j < cfg.get_number_interpretation()->get_size(); ++j)
	{
		synth.synth() << "lda (" << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << "), y" << E_;
		synth.synth() << "sta (" << token(token_provider::TOKENS::PUSH_POP_TARGET_STACK_PTR) << "), y" << E_;
		if(!j)
		{
			synth.synth() << "iny" << E_;
		}
	}

	synth.synth() << "ldy #0" << E_;
	synth.synth() << "adw (" << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << "),y #" << cfg.get_number_interpretation()->get_size() << E_;
	synth.synth() << "rts" << E_;
}

void runtime_base::synth_POKE() const
{
	synth.synth(false) << "POKE" << E_;
	synth.synth() << "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << "jsr POP_TO" << E_;
	synth.synth() << "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << R"(
	jsr POP_TO
	ldy #0
	lda FR0,y
	sta (FR1),y
	iny
	lda FR0,y
	sta (FR1),y
	rts
)";
}

void runtime_base::synth_PEEK() const
{
	synth.synth(false) << "PEEK" << E_;
	synth.synth() << "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << R"(
	jsr POP_TO
	ldy #0
	lda (FR0),y
	sta FR1,y
	lda #0
	sta FR1+1
)";
	synth.synth() << "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << R"(
	jsr PUSH_FROM
	rts
)";
}

void runtime_base::register_own_runtime_funtion(const std::string& body)
{
	own_functions.push_back(body);
}

void runtime_base::synth_own_functions() const
{
	for (const auto& foo : own_functions)
	{
		synth.synth(false) << foo;
	}
}
