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
	synth_LOGICAL_AND();
	synth_LOGICAL_OR();
	synth_BINARY_XOR();
	synth_BINARY_AND();
	synth_BINARY_OR();
	synth_IsXY00();
	synth_TRUE_FALSE();
	synth_FR0_boolean_invert();
	synth_PUT_ZERO_IN_FR0();
	synth_PUT_RANDOM_IN_FR0();
	synth_Is_FR0_true();
	synth_INIT_ARRAY_OFFSET();
	synth_CALCULATE_ARRAY_ROW_SIZE_IN_BYTES();
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
	synth_PUTSTRINGLITERAL();
	synth_PUTCOMMA();
	synth_SOUND();
	synth_POKEY_INIT();
	synth_POP_TO();
	synth_PEEK_TO();
	synth_PUSH_FROM();
	synth_INIT_PUSH_POP_POINTER();
	synth_POKE();
	synth_DPOKE();
	synth_PEEK();
	synth_DPEEK();
	synth_STICK();
	synth_STRIG();
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
	txa
	pha
	tya
	pha
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
	pla
	tay
	pla
	tax
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
No new line is printed at the end.
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

/*
Outputs string located at STRING_LITERAL_PTR.
Last character is not inverted. Length of the string
is located at STRING_LITERAL_PTR-1
*/
void runtime_base::synth_PUTSTRINGLITERAL() const
{
	synth.synth() << R"(
PUTSTRINGLITERAL
	ldy #0
)";
	synth.synth() << "lda (" << token(token_provider::TOKENS::STRING_LITERAL_PTR) << "),y" << E_;

	synth.synth() << R"(
	tax
	iny
@
)";
	synth.synth() << "lda (" << token(token_provider::TOKENS::STRING_LITERAL_PTR) << "),y" << E_;
	synth.synth() << R"(
	jsr PUTCHAR
	iny
	dex
	bne @-
	rts
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
	for (auto j = 0; j < cfg.get_number_interpretation()->get_size(); ++j)
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
	for (auto j = 0; j < cfg.get_number_interpretation()->get_size(); ++j)
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
	rts
)";
}

void runtime_base::synth_DPOKE() const
{
	synth.synth(false) << "DPOKE" << E_;
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

void runtime_base::synth_DPEEK() const
{
	synth.synth(false) << "DPEEK" << E_;
	synth.synth() << "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << R"(
	jsr POP_TO
	ldy #0
	lda (FR0),y
	sta FR1,y
	iny
	lda (FR0),y
	sta FR1+1
)";
	synth.synth() << "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << R"(
	jsr PUSH_FROM
	rts
)";
}

void runtime_base::synth_STICK() const
{
	synth.synth(false) << "STICK" << E_;
	synth.synth() << "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << R"(
	jsr POP_TO
	ldy FR0
	lda STICK0,y
	sta FR1
	lda #0
	sta FR1+1
)";
	synth.synth() << "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << R"(
	jsr PUSH_FROM
	rts
)";
}

void runtime_base::synth_STRIG() const
{
	synth.synth(false) << "STRIG" << E_;
	synth.synth() << "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	synth.synth() << R"(
	jsr POP_TO
	ldy FR0
	lda STRIG0,y
	sta FR1
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

void runtime_base::synth_CALCULATE_ARRAY_ROW_SIZE_IN_BYTES() const
{
	// ARRAY_ASSIGNMENT_TMP_SIZE stores the second index of an array.
	// We need to multiply it by single number size
	synth.synth() << R"(
CALCULATE_ARRAY_ROW_SIZE_IN_BYTES
	mwa #0 ARRAY_CALCULATION_TMP
CALCULATE_ARRAY_ROW_SIZE_IN_BYTES_L0
	#if .word ARRAY_ASSIGNMENT_TMP_SIZE = #0
		mwa ARRAY_CALCULATION_TMP ARRAY_ASSIGNMENT_TMP_SIZE
		rts
	#end
)";
	synth.synth() << "adw ARRAY_CALCULATION_TMP #" << cfg.get_number_interpretation()->get_size() << " ARRAY_CALCULATION_TMP" << E_;
	synth.synth() << R"(
	dew ARRAY_ASSIGNMENT_TMP_SIZE
	jmp CALCULATE_ARRAY_ROW_SIZE_IN_BYTES_L0
	rts
ARRAY_CALCULATION_TMP dta a(0)
)";
}

void runtime_base::synth_INIT_ARRAY_OFFSET() const
{
	synth.synth() << R"(
INIT_ARRAY_OFFSET
	jsr CALCULATE_ARRAY_ROW_SIZE_IN_BYTES
INIT_ARRAY_OFFSET_L0
	#if .word FR0 = #0
		mwa FR1 ARRAY_ASSIGNMENT_TMP_SIZE
		jsr CALCULATE_ARRAY_ROW_SIZE_IN_BYTES
		adw ARRAY_ASSIGNMENT_TMP_ADDRESS ARRAY_ASSIGNMENT_TMP_SIZE ARRAY_ASSIGNMENT_TMP_ADDRESS
		rts
	#end
	adw ARRAY_ASSIGNMENT_TMP_ADDRESS ARRAY_ASSIGNMENT_TMP_SIZE ARRAY_ASSIGNMENT_TMP_ADDRESS
	dew FR0
	jmp INIT_ARRAY_OFFSET_L0
)";
}

void runtime_base::synth_helpers() const
{
	synth.synth(false) << ".zpvar ARRAY_ASSIGNMENT_TMP_ADDRESS .word" << E_;
	synth.synth(false) << "ARRAY_ASSIGNMENT_TMP_SIZE dta a(0)" << E_;
	synth.synth(false) << "ARRAY_ASSIGNMENT_TMP_VALUE " << cfg.get_number_interpretation()->get_initializer() << E_;
}
