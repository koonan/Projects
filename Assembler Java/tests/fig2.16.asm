copy	START   0
	EXTDEF	buffer,bufend,length
	EXTREF	rdrec,wrrec
first	STL	retadr
cloop  +JSUB    rdrec
        LDA     length
        COMP   #0
        JEQ     endfil
       +JSUB    wrrec
        J       cloop
endfil  LDA    =C'EOF'
        STA     buffer
        LDA    #3
        STA     length
       +JSUB    wrrec
        J      @retadr
retadr  RESW    1
length  RESW    1
		LTORG
buffer  RESB    4096
bufend EQU     buffer+4096
maxlen  EQU     bufend-buffer
.
.       Subroutine to read record into buffer
.
rdrec	CSECT
	EXTREF	buffer,length,bufend
        CLEAR   X
        CLEAR   A
        CLEAR   S
        LDT     maxlen
rloop   TD      input
        JEQ     rloop
        RD      input
        COMPR   A,S
        JEQ     exit
       +STCH    buffer,X
        TIXR    T
        JLT     rloop
exit   +STX     length
        RSUB
input	BYTE	X'00'
maxlen	WORD	bufend-buffer
.
.       Subroutine to write record from buffer
.
wrrec	CSECT
	EXTREF	length,buffer
        CLEAR   X
       +LDT     length
wloop   TD     =X'01'
        JEQ     wloop
       +LDCH    buffer,X
        WD     =X'01'
        TIXR    T
        JLT     wloop
        RSUB
        END     first
		