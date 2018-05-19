		org $2000
		
bufl	equ 256
		
		lda #5			; input line
		sta ICCOM
		lda #<buf
		sta ICBAL
		lda #>buf
		sta ICBAL+1
		lda #<bufl
		sta ICBLL
		lda #>bufl
		sta ICBLL+1
		ldx #0
		jsr CIOV

chuj	jmp chuj

buf
:bufl	dta b(0)






; ATARI registers
AUDC1 equ $d201
AUDF1 equ $d200
CIOV equ $e456
FR0 equ $d4
FR1 equ $e0
ICBAL equ $344
ICBLL equ $348
ICCOM equ $342
INBUFP equ $f3
LBUFF equ $580
RANDOM equ $d20a
RUNAD equ $2e0
SKCTL equ $d20f
SSKCTL equ $232
STICK0 equ $278
STRIG0 equ $284
; ATARI constants
EOL equ $9b
PUTCHR equ $b
