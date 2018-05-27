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

#include "runtime_integer.h"

// Synth Indentation
#define SI synth.synth() <<	

// Synth Normal
#define SN synth.synth(false) <<

// Synth with Config
#define SC ss << cfg.get_indent() <<

runtime_integer::runtime_integer(char endline, synthesizer& _synth, const config& _config)
	: runtime_base(endline, _synth, _config)
{
}

void runtime_integer::synth_implementation() const
{
	runtime_base::synth_implementation();
	synth_BCDByte2Ascii();
	synth_INBUFP_INIT();
}

// Traverses the LBUFF buffer and sets the
// INBUFP pointer to the first non-zero character
void runtime_integer::synth_INBUFP_INIT() const
{
	SN R"(
INBUFP_INIT
	ldy #0
INBUFP_INIT_LABEL_1
	lda LBUFF,y
	cmp #'0'
	bne INBUFP_INIT_LABEL_0
	iny
	jmp INBUFP_INIT_LABEL_1
INBUFP_INIT_LABEL_0
	sty INBUFP
	rts
)";
}

// Converts byte located at FASC_RES,y into Ascii characters
// and stores the result in location pointed by FASC_PTR
void runtime_integer::synth_BCDByte2Ascii() const
{
	SN R"(
BCDByte2Ascii
	lda FASC_RES,y
	pha
	and #%11110000
:4	lsr
	add #$30
	ldy #0
	sta (FASC_PTR),y
	pla
	and #%00001111
	add #$30
	iny
	sta (FASC_PTR),y
	rts
)";
}

/*
Adds two numbers located at FR0 and FR1.
Result is stored in FR0.
*/
void runtime_integer::synth_BADD() const
{
	SN R"(
BADD
	adw FR0 FR1 FR0
	rts
)";
}


/*
Subtracts two numbers located at FR0 and FR1.
Result is stored in FR0.
*/
void runtime_integer::synth_BSUB() const
{
	SN R"(
BSUB
	sbw FR0 FR1 FR0
	rts
)";
}

/*
Multiplicates two numbers located at FR0 and FR1.
Uses accumulation method.
Consider using this instead: http://codebase64.org/doku.php?id=base:16bit_multiplication_32-bit_product
Result is stored in FR0.
*/
void runtime_integer::synth_BMUL() const
{
	SN R"(
BMUL
	lda #0
	sta BMUL_RES
	sta BMUL_RES+1
	ldx FR0
	ldy FR0+1
	jsr IsXY00
	cmp #1
	beq BMUL_LABEL_0
	ldx FR1
	ldy FR1+1
	jsr IsXY00
	cmp #1
	beq BMUL_LABEL_0
	mwa FR0 BMUL_RES
BMUL_LABEL_1
	sbw FR1 #1
	ldx FR1
	ldy FR1+1
	jsr IsXY00
	cmp #1
	beq BMUL_LABEL_0
	adw BMUL_RES FR0 BMUL_RES
	jmp BMUL_LABEL_1
BMUL_LABEL_0
	mwa BMUL_RES FR0
	rts
BMUL_RES
	dta b(0), b(0)
)";
}

/*
Divides two numbers located at FR0 and FR1.
Result is stored in FR0.

Inspired by: http://codebase64.org/doku.php?id=base:16bit_division_16-bit_result
*/
void runtime_integer::synth_BDIV() const
{
	SN R"(
BDIV
	lda #0
	sta BDIV_REMAINDER
	sta BDIV_REMAINDER+1
	ldx #16
BDIV_LOOP
	asl FR0
	rol FR0+1	
	rol BDIV_REMAINDER
	rol BDIV_REMAINDER+1
	lda BDIV_REMAINDER
	sec
	sbc FR1
	tay
	lda BDIV_REMAINDER+1
	sbc FR1+1
	bcc BDIV_SKIP
	sta BDIV_REMAINDER+1
	sty BDIV_REMAINDER
	inc BDIV_RES
BDIV_SKIP
	dex
	bne BDIV_LOOP	
	rts
BDIV_RES EQU FR0
BDIV_REMAINDER
	dta a(0)
)";
}

/*
Converts the number located at FR0 to ASCII representation.
Result is stored in LBUFF. Last character to print must be inverted.
INBUFP should be set to indicate first significant character within LBUFF.
*/
void runtime_integer::synth_FASC() const
{
	SN R"(
FASC
	lda #0
	sta FASC_RES+0
	sta FASC_RES+1
	sta FASC_RES+2
	ldx #16
	sed
FASC_LOOP_0
	asl FR0+0
	rol FR0+1
	lda FASC_RES+0
	adc FASC_RES+0
	sta FASC_RES+0
	lda FASC_RES+1
	adc FASC_RES+1
	sta FASC_RES+1
	lda FASC_RES+2
	adc FASC_RES+2
	sta FASC_RES+2
	dex
	bne FASC_LOOP_0
	cld
	lda #<LBUFF
	sta FASC_PTR
	lda #>LBUFF
	sta FASC_PTR+1
	ldy #2
	jsr BCDByte2Ascii
	adw FASC_PTR #2
	ldy #1
	jsr BCDByte2Ascii
	adw FASC_PTR #2
	ldy #0
	jsr BCDByte2Ascii
	ldy #1
	lda (FASC_PTR),y
	ora #%10000000
	sta (FASC_PTR),y
	jsr INBUFP_INIT
	rts
FASC_RES
	dta b(0), b(0), b(0)
.zpvar FASC_PTR .word
)";
}

/*
Compares two numbers (FR0 and FR1). Result is stored in A:
 1 = FR0 is less
 0 = FR0 equals FR1
-1 = FR0 is greater
*/
void runtime_integer::synth_COMPARE_NUMBER() const
{
	SN R"(
COMPARE_NUMBER
	#if .word FR0 = FR1
	lda #0
	rts
	#end
	#if .word FR0 < FR1
	lda #1
	#else
	lda #-1
	#end
	rts
)";
}

/*
Compares FR0 and FR1. Based on the INTEGER_COMPARE_TMP (which is
set accordingly to the required compare type) stores
TRUE or FALSE value in FR0.
*/
void runtime_integer::synth_COMPARE_FR0_FR1() const
{
	SN R"(
COMPARE_FR0_FR1
	jsr COMPARE_NUMBER
	cmp INTEGER_COMPARE_TMP
	beq COMPARE_FR0_FR1_TRUE
	mwa RUNTIME_INTEGER_FALSE FR0
	rts
COMPARE_FR0_FR1_TRUE
	mwa RUNTIME_INTEGER_TRUE FR0
	rts
)";
}

void runtime_integer::synth_TRUE_FALSE() const
{
	SN "RUNTIME_INTEGER_FALSE dta a(0)" << E_;
	SN "RUNTIME_INTEGER_TRUE dta a(1)" << E_;
}

void runtime_integer::synth_FR0_boolean_invert() const
{
	SN R"(
FR0_boolean_invert
	#if .word FR0 = RUNTIME_INTEGER_FALSE
		mwa RUNTIME_INTEGER_TRUE FR0
	#else
		mwa RUNTIME_INTEGER_FALSE FR0
	#end
	rts
)";
}

// A=0 if FRO is equal to RUNTIME_INTEGER_FALSE,
// A=1 otherwise
void runtime_integer::synth_Is_FR0_true() const
{
	SN R"(
Is_FR0_true
	#if .word FR0 = RUNTIME_INTEGER_FALSE
		lda #0
	#else
		lda #1
	#end
	rts
)";
}

void runtime_integer::synth_helpers() const
{
	runtime_base::synth_helpers();
	SN "INTEGER_COMPARE_TMP dta b(0)" << E_;
}

void runtime_integer::synth_LOGICAL_AND() const
{
	SN R"(
LOGICAL_AND
	#if .word FR0 <> #0 .and .word FR1 <> #0
	mwa RUNTIME_INTEGER_TRUE FR0
	#else
	mwa RUNTIME_INTEGER_FALSE FR0
	#end
	rts
)";
}

void runtime_integer::synth_LOGICAL_OR() const
{
	SN R"(
LOGICAL_OR
	#if .word FR0 <> #0 .or .word FR1 <> #0
	mwa RUNTIME_INTEGER_TRUE FR0
	#else
	mwa RUNTIME_INTEGER_FALSE FR0
	#end
	rts
)";
}

void runtime_integer::synth_BINARY_XOR() const
{
	SN R"(
BINARY_XOR
	lda FR0
	eor FR1
	sta FR0
	lda FR0+1
	eor FR1+1
	sta FR0+1
	rts
)";
}

void runtime_integer::synth_BINARY_AND() const
{
	SN R"(
BINARY_AND
	lda FR0
	and FR1
	sta FR0
	lda FR0+1
	and FR1+1
	sta FR0+1
	rts
)";
}

void runtime_integer::synth_BINARY_OR() const
{
	SN R"(
BINARY_OR
	lda FR0
	ora FR1
	sta FR0
	lda FR0+1
	ora FR1+1
	sta FR0+1
	rts
)";
}

void runtime_integer::synth_PUT_ZERO_IN_FR0() const
{
	SN R"(
PUT_ZERO_IN_FR0
	mwa #0 FR0
	rts
)";
}

void runtime_integer::synth_PUT_RANDOM_IN_FR0() const
{
	SN R"(
PUT_RANDOM_IN_FR0
	mva RANDOM FR0
	mva RANDOM FR0+1
	rts
)";
}

void runtime_integer::synth_EMPLACE_INPUT_BUFFER_INTO_INTEGER() const
{
	SN R"(
EMPLACE_INPUT_BUFFER_INTO_INTEGER
	rts
)";
}

#undef SI
#undef SN
#undef SC
