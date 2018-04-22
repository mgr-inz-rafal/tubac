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

// TODO: Move those macros to some common include, since they are copy&pasted around

// Synth Indentation
#define SI synth.synth() <<	

// Synth Normal
#define SN synth.synth(false) <<

// Synth with Config
#define SC ss << cfg.get_indent() <<


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
	synth_DO_STRING_ASSIGNMENT();
	synth_DO_STRING_ASSIGNMENT_UPDATE_LENGTH();
	synth_DO_STRING_COMPARISON();
	synth_DO_STRING_COMPARISON_INTERNAL();
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
	synth_PRINT_STRING();
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
	SI R"(
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
	SI R"(
PUTNEWLINE
	lda #EOL
	jsr PUTCHAR
	rts
)";
}

void runtime_base::synth_PUTSPACE() const
{
	SI R"(
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
	SI R"(
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
 * TODO: Outdated comment removed
*/
void runtime_base::synth_PRINT_STRING() const
{
	// TODO: This is being reworked...
	SN "PRINT_STRING" << E_;
	SI "#if .word " << token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX) << " = #0 .and .word " << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << " = #0" << E_;
	SI "jmp PRINT_STRING_EXIT" << E_;
	SI "#end" << E_;
	SI "sbw " << token(token_provider::TOKENS::STRING_PRINTED_LENGTH) << ' ' << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << E_;
	SI "adw " << token(token_provider::TOKENS::STRING_LEFT_BASE) << ' ' << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << E_;
	SI "sbw " << token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX) << ' ' << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << E_;
	SI "ldy #0" << E_;
	SN "PRINT_STRING_LOOP" << E_;
	SI "lda (" << token(token_provider::TOKENS::STRING_LEFT_BASE) << "),y" << E_;
	SI "jsr PUTCHAR" << E_;
	SI "dew " << token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX) << E_;
	SI "dew " << token(token_provider::TOKENS::STRING_PRINTED_LENGTH) << E_;
	SI "#if .word " << token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX) << " = #0 .or .word " << token(token_provider::TOKENS::STRING_PRINTED_LENGTH) << " = #0" << E_;
	SI "jmp PRINT_STRING_EXIT" << E_;
	SI "#end" << E_;
	SI "inw " << token(token_provider::TOKENS::STRING_LEFT_BASE) << E_;
	SI "jmp PRINT_STRING_LOOP" << E_;
	SN "PRINT_STRING_EXIT" << E_;
	SI "rts" << E_;
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

// On 18.03.2018 I corrected a slight bug here, that enabled the following
// test cases to pass:
//		suites\print\comma2.txt
//		suites\use_cases\LINUX_antoni_suffixes.txt 
//from __future__ import print_function
//
//ptabw = 10
//
//def basic_print(*items):
//	auxbr = ptabw-1
//	cox = 0
//	for text in items:
//		for ch in text:
//			print(ch, sep="", end="")
//			cox += 1
//			if cox >= auxbr:
//				auxbr += ptabw
//		for i in range(cox, auxbr+1):
//			print(" ", sep="", end="")
//			cox += 1
//			if cox >= auxbr:
//				auxbr += ptabw
//	print()
void runtime_base::synth_PUTCOMMA() const
{
	SI R"(
PUTCOMMA
	mva AUXBR AUXBRT
	inc AUXBRT
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
	SI R"(
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
	SN "SOUND" << E_;
	SI "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI "jsr POP_TO" << E_;
	SI "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
	jsr POP_TO
	lda FR1
:4	asl
	eor FR0
	tax
)";
	SI "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI "jsr POP_TO" << E_;
	SI "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
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
	SI R"(
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
	SN "POP_TO" << E_;

	SI "ldy #0" << E_;
	SI "sbw (" << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << "),y #" << cfg.get_number_interpretation()->get_size() << E_;
	SI "jsr INIT_PUSH_POP_POINTER" << E_;
	SI "ldy #0" << E_;
	for (auto j = 0; j < cfg.get_number_interpretation()->get_size(); ++j)
	{
		SI "lda (" << token(token_provider::TOKENS::PUSH_POP_TARGET_STACK_PTR) << "), y" << E_;
		SI "sta (" << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << "), y" << E_;
		if(!j)
		{
			SI "iny" << E_;
		}
	}

	SI "rts" << E_;
}

void runtime_base::synth_PEEK_TO() const
{
	SN "PEEK_TO" << E_;
	SI "jsr POP_TO" << E_;
	SI "ldy #0" << E_;
	SI "adw (" << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << "),y #" << cfg.get_number_interpretation()->get_size() << E_;

	SI "rts" << E_;
}

void runtime_base::synth_FAKE_POP() const
{
	SN "FAKE_POP" << E_;
	SI "ldy #0" << E_;
	SI "sbw (" << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << "),y #" << cfg.get_number_interpretation()->get_size() << E_;
	SI "rts" << E_;
}

void runtime_base::synth_INIT_PUSH_POP_POINTER() const
{
	SN "INIT_PUSH_POP_POINTER" << E_;
	SI "ldy #0" << E_;
	SI "mwa (" << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << "),y " << token(token_provider::TOKENS::PUSH_POP_TARGET_STACK_PTR) << E_;
	SI "rts" << E_;
}

void runtime_base::synth_PUSH_FROM() const
{
	SN "PUSH_FROM" << E_;
	SI "jsr INIT_PUSH_POP_POINTER" << E_;
	SI "ldy #0" << E_;
	for (auto j = 0; j < cfg.get_number_interpretation()->get_size(); ++j)
	{
		SI "lda (" << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << "), y" << E_;
		SI "sta (" << token(token_provider::TOKENS::PUSH_POP_TARGET_STACK_PTR) << "), y" << E_;
		if(!j)
		{
			SI "iny" << E_;
		}
	}

	SI "ldy #0" << E_;
	SI "adw (" << token(token_provider::TOKENS::PUSH_POP_PTR_TO_INC_DEC) << "),y #" << cfg.get_number_interpretation()->get_size() << E_;
	SI "rts" << E_;
}

void runtime_base::synth_POKE() const
{
	SN "POKE" << E_;
	SI "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI "jsr POP_TO" << E_;
	SI "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
	jsr POP_TO
	ldy #0
	lda FR0,y
	sta (FR1),y
	rts
)";
}

void runtime_base::synth_DPOKE() const
{
	SN "DPOKE" << E_;
	SI "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI "jsr POP_TO" << E_;
	SI "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
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
	SN "PEEK" << E_;
	SI "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
	jsr POP_TO
	ldy #0
	lda (FR0),y
	sta FR1,y
	lda #0
	sta FR1+1
)";
	SI "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
	jsr PUSH_FROM
	rts
)";
}

void runtime_base::synth_DPEEK() const
{
	SN "DPEEK" << E_;
	SI "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
	jsr POP_TO
	ldy #0
	lda (FR0),y
	sta FR1,y
	iny
	lda (FR0),y
	sta FR1+1
)";
	SI "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
	jsr PUSH_FROM
	rts
)";
}

void runtime_base::synth_STICK() const
{
	SN "STICK" << E_;
	SI "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
	jsr POP_TO
	ldy FR0
	lda STICK0,y
	sta FR1
	lda #0
	sta FR1+1
)";
	SI "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
	jsr PUSH_FROM
	rts
)";
}

void runtime_base::synth_STRIG() const
{
	SN "STRIG" << E_;
	SI "mwa #FR0 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
	jsr POP_TO
	ldy FR0
	lda STRIG0,y
	sta FR1
	lda #0
	sta FR1+1
)";
	SI "mwa #FR1 " << token(token_provider::TOKENS::PUSH_POP_VALUE_PTR) << E_;
	SI R"(
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
		SN foo;
	}
}

void runtime_base::synth_CALCULATE_ARRAY_ROW_SIZE_IN_BYTES() const
{
	// ARRAY_ASSIGNMENT_TMP_SIZE stores the second index of an array.
	// We need to multiply it by single number size
	SI R"(
CALCULATE_ARRAY_ROW_SIZE_IN_BYTES
	mwa #0 ARRAY_CALCULATION_TMP
CALCULATE_ARRAY_ROW_SIZE_IN_BYTES_L0
	#if .word ARRAY_ASSIGNMENT_TMP_SIZE = #0
		mwa ARRAY_CALCULATION_TMP ARRAY_ASSIGNMENT_TMP_SIZE
		rts
	#end
)";
	SI "adw ARRAY_CALCULATION_TMP #" << cfg.get_number_interpretation()->get_size() << " ARRAY_CALCULATION_TMP" << E_;
	SI R"(
	dew ARRAY_ASSIGNMENT_TMP_SIZE
	jmp CALCULATE_ARRAY_ROW_SIZE_IN_BYTES_L0
	rts
ARRAY_CALCULATION_TMP dta a(0)
)";
}

void runtime_base::synth_INIT_ARRAY_OFFSET() const
{
	SI R"(
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
	SN ".zpvar ARRAY_ASSIGNMENT_TMP_ADDRESS .word" << E_;
	SN "ARRAY_ASSIGNMENT_TMP_SIZE dta a(0)" << E_;
	SN "ARRAY_ASSIGNMENT_TMP_VALUE " << cfg.get_number_interpretation()->get_initializer() << E_;
}

void runtime_base::synth_DO_STRING_ASSIGNMENT_UPDATE_LENGTH() const
{
	SN "DO_STRING_ASSIGNMENT_UPDATE_LENGTH" << E_;
	SI "ldy #0" << E_;
	SI "lda " << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << E_;
	SI "sta (" << token(token_provider::TOKENS::STRING_LEFT_BASE) << "),y" << E_;
	SI "iny" << E_;
	SI "lda " << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << "+1" << E_;
	SI "sta (" << token(token_provider::TOKENS::STRING_LEFT_BASE) << "),y" << E_;
	SI "rts" << E_;
}

void runtime_base::synth_DO_STRING_COMPARISON() const
{
	SN "DO_STRING_COMPARISON" << E_;
	SI "mwa " << token(token_provider::TOKENS::STRING_LEFT_BASE) << ' ' << token(token_provider::TOKENS::STRING_CMP_LEFT_PTR) << E_;
	SI "adw " << token(token_provider::TOKENS::STRING_CMP_LEFT_PTR) << ' ' << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << E_;
	SI "mwa " << token(token_provider::TOKENS::STRING_RIGHT_BASE) << ' ' << token(token_provider::TOKENS::STRING_CMP_RIGHT_PTR) << E_;
	SI "adw " << token(token_provider::TOKENS::STRING_CMP_RIGHT_PTR) << ' ' << token(token_provider::TOKENS::STRING_RIGHT_FIRST_INDEX) << E_;
	SI "jsr DO_STRING_COMPARISON_INTERNAL" << E_;
	SI "rts" << E_;
	SN "STRING_COMPARISON_TMP_1 dta b(0)" << E_;
	SN "STRING_COMPARISON_TMP_2 dta b(0)" << E_;
	SN "STRING_COMPARISON_LEFT_LENGTH dta a(0)" << E_;
	SN "STRING_COMPARISON_RIGHT_LENGTH dta a(0)" << E_;
}

// TODO: Verify compatibility with empty strings
void runtime_base::synth_DO_STRING_COMPARISON_INTERNAL() const
{
	SN "DO_STRING_COMPARISON_INTERNAL" << E_;
	SI "ldy #0" << E_;
	SN "DO_STRING_COMPARISON_INTERNAL_0" << E_;
	SI "lda (" << token(token_provider::TOKENS::STRING_CMP_RIGHT_PTR) << "),y" << E_;
	SI "sta STRING_COMPARISON_TMP_1" << E_;
	SI "lda (" << token(token_provider::TOKENS::STRING_CMP_LEFT_PTR) << "),y" << E_;
	SI R"(#if .byte @ < STRING_COMPARISON_TMP_1
	mwa RUNTIME_INTEGER_TRUE FR0
	rts
	#end
	dew STRING_COMPARISON_LEFT_LENGTH
	dew STRING_COMPARISON_RIGHT_LENGTH
	#if .word STRING_COMPARISON_RIGHT_LENGTH = #0 
	mwa RUNTIME_INTEGER_FALSE FR0
	rts
	#end
	#if .word STRING_COMPARISON_LEFT_LENGTH = #0 
	mwa RUNTIME_INTEGER_TRUE FR0
	rts
	#end
)";

	SI "inw " << token(token_provider::TOKENS::STRING_CMP_LEFT_PTR) << E_;
	SI "inw " << token(token_provider::TOKENS::STRING_CMP_RIGHT_PTR) << E_;
	SI "jmp DO_STRING_COMPARISON_INTERNAL_0" << E_;
	SI "rts" << E_;
}

void runtime_base::synth_DO_STRING_ASSIGNMENT() const
{
	SN "DO_STRING_ASSIGNMENT" << E_;
	SI "mwa " << token(token_provider::TOKENS::STRING_LEFT_BASE) << ' ' << token(token_provider::TOKENS::STRING_LEFT_PTR) << E_;
	SI "adw " << token(token_provider::TOKENS::STRING_LEFT_PTR) << ' ' << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << E_;
	SI "mwa " << token(token_provider::TOKENS::STRING_RIGHT_BASE) << ' ' << token(token_provider::TOKENS::STRING_RIGHT_PTR) << E_;
	SI "adw " << token(token_provider::TOKENS::STRING_RIGHT_PTR) << ' ' << token(token_provider::TOKENS::STRING_RIGHT_FIRST_INDEX) << E_;
	SI "mwa " << token(token_provider::TOKENS::STRING_RIGHT_BASE) << ' ' << token(token_provider::TOKENS::STRING_RIGHT_PTR) << E_;
	SI "adw " << token(token_provider::TOKENS::STRING_RIGHT_PTR) << ' ' << token(token_provider::TOKENS::STRING_RIGHT_FIRST_INDEX) << E_;
	// Copy operation pointers are now initialized. Reuse second index value
	// for storing the maximum count of copy steps
	SI "sbw " << token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX) << ' ' << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << E_;
	SI "sbw " << token(token_provider::TOKENS::STRING_RIGHT_SECOND_INDEX) << ' ' << token(token_provider::TOKENS::STRING_RIGHT_FIRST_INDEX) << E_;
	// Do the actual copy
	SI "ldy #0" << E_;
	SI "sty " << token(token_provider::TOKENS::STRING_ASSIGNMENT_COUNTER) << E_;
	SI "sty " << token(token_provider::TOKENS::STRING_ASSIGNMENT_COUNTER) << "+1" << E_;
	SN "DO_STRING_ASSIGNMENT_LOOP" << E_;
	SI "lda (" << token(token_provider::TOKENS::STRING_RIGHT_PTR) << "),y" << E_;
	SI "sta (" << token(token_provider::TOKENS::STRING_LEFT_PTR) << "),y" << E_;
	SI "dew " << token(token_provider::TOKENS::STRING_RIGHT_SECOND_INDEX) << E_;
	SI "dew " << token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX) << E_;
	SI "#if .word " << token(token_provider::TOKENS::STRING_RIGHT_SECOND_INDEX) << " = #0 .or .word " << token(token_provider::TOKENS::STRING_LEFT_SECOND_INDEX) << " = #0" << E_;
	SI "inw " << token(token_provider::TOKENS::STRING_ASSIGNMENT_COUNTER) << E_;
	SI "adw " << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << ' ' << token(token_provider::TOKENS::STRING_ASSIGNMENT_COUNTER) << E_;

	// Adjust the target string length... -->
	SI "sbw " << token(token_provider::TOKENS::STRING_LEFT_BASE) << " #2" << E_;
	SI "ldy #0" << E_;
	SI "lda (" << token(token_provider::TOKENS::STRING_LEFT_BASE) << "),y" << E_;
	SI "sta STRING_ASSIGNMENT_TMP" << E_;
	SI "iny" << E_;
	SI "lda (" << token(token_provider::TOKENS::STRING_LEFT_BASE) << "),y" << E_;
	SI "sta STRING_ASSIGNMENT_TMP+1" << E_;
	SI "#if .word STRING_ASSIGNMENT_TMP < " << token(token_provider::TOKENS::STRING_LEFT_FIRST_INDEX) << E_;
	SI "jsr DO_STRING_ASSIGNMENT_UPDATE_LENGTH" << E_;
	SI "#end" << E_;
	SI "#if .byte LEFT_HAS_SECOND = #0" << E_;
	SI "jsr DO_STRING_ASSIGNMENT_UPDATE_LENGTH" << E_;
	SI "#end" << E_;
	// --> ...before exiting from subroutine.

	SI "rts" << E_;
	SI "#end" << E_;
	SI "inw " << token(token_provider::TOKENS::STRING_LEFT_PTR) << E_;
	SI "inw " << token(token_provider::TOKENS::STRING_RIGHT_PTR) << E_;
	SI "inw " << token(token_provider::TOKENS::STRING_ASSIGNMENT_COUNTER) << E_;
	SI "jmp DO_STRING_ASSIGNMENT_LOOP" << E_;
	SN "STRING_ASSIGNMENT_TMP dta a(0)" << E_;	// TODO: See if more string assignment related variables could be tmp instead of compiler spawned
	SN "LEFT_HAS_SECOND dta b(0)" << E_;		// TODO: ditto
}

#undef SI
#undef SN
#undef SC
