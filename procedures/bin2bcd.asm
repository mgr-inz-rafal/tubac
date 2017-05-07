	org 2000
	
ICHID = $0340 ;IOCB 0 S:
ICCOM = $0342 ;IOCB Command
ICBAL = $0344 ;Xfer Buffer Adr
ICBAH = $0345
ICPTL = $0346 ;PutByte Adr
ICPTH = $0347
ICBLL = $0348 ;Buffer Len
ICBLH = $0349
CIOV = $E456 ; CIO Vector

PUTCHR = 0x000B


BIN2BCD LDA #0          ;Clear the result area
        STA RES+0
        STA RES+1
        STA RES+2
        LDX #16         ;Setup the bit counter
        SED             ;Enter decimal mode
_LOOP   ASL VAL+0       ;Shift a bit out of the binary
        ROL VAL+1       ;... value
        LDA RES+0       ;And add it into the result, doubling
        ADC RES+0       ;... it at the same time
        STA RES+0
        LDA RES+1
        ADC RES+1
        STA RES+1
        LDA RES+2
        ADC RES+2
        STA RES+2
        DEX             ;More bits to process?
        BNE _LOOP
        CLD             ;Leave decimal mode

; --------------------------------
	lda #2
	sta CNT
	
_AGAIN
	ldy CNT
	lda RES,y
	and #%11110000
	lsr
	lsr
	lsr
	lsr
	add #48
	
	sta CHR
	lda #<CHR
	sta ICBAL
	lda #>CHR
	sta ICBAL+1
	jsr __TUBAC__PUTCHAR

	ldy CNT
	dec CNT
	lda RES,y
	and #%00001111
	add #48
	
	sta CHR
	lda #<CHR
	sta ICBAL
	lda #>CHR
	sta ICBAL+1
	jsr __TUBAC__PUTCHAR
	
	lda CNT
	cmp #$ff
	bne _AGAIN

dupa	jmp dupa

__TUBAC__PUTCHAR	
	lda #PUTCHR
	sta ICCOM
	lda #1
	sta ICBLL
	lda #0
	sta ICBLL+1
	ldx #0
	jsr CIOV
	rts
	
		
VAL
		dta b($ff),b($ff)
RES
		dta b(0),b(0),b(0)
CHR
		dta b(0)
CNT
		dta b(0)