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
; Setup CIO Call
    LDX #0 ;IOCB 0
    LDA #9 ;Put Cmd Val
    STA ICCOM,X ;Set it as the cmd
    ;LDA #HELLO&255 ;Str low byte
    LDA <HELLO ;Str low byte
    STA ICBAL,X
    ;LDA #HELLO/256 ;Str high byte
    LDA >HELLO ;Str low byte
    STA ICBAH,X
    LDA #0 ;Str Len low byte
    STA ICBLL,X
    LDA #$FF ;Str Len high byte
    STA ICBLH,X
; Call CIO
    JSR CIOV
    RTS 
HELLO
	dta c'Hello World!',b($9B)