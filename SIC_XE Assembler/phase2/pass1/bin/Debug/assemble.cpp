.23456789012345678901234567890
.23456789012345678901234567890
         LDB     #10
         LDS     #1
         LDX     #0
         LDT     #1
         LDA     NUM
         JSUB    NEG
         JSUB    LEN
         JSUB    DIVISOR
.********OPERATION********
         LDX     LENGTH
         LDA     NUM
OPR      DIVR    T,A
         ADD     #48
         JSUB    PRINT
         COMPR   T,S
         JEQ     FINAL
         SUB     #48
         MULR    T,A
         SUB     NUM
         MUL     ONE
         DIVR    B,T
         STA     NUM
         J       OPR
FINAL    J       *
.********PRINT**************
PRINT    TD      DEV
         JEQ     PRINT
         WD      DEV
         RSUB
.********DIVISOR(T)*********
DIVISOR  COMPR   X,S
         JEQ     END
         MULR    B,T
         SUBR    S,X
         J       DIVISOR
END      RSUB
.********LEN*************
LEN      LDA     NUM
LOOP     DIV     #10
         ADDR    S,X
         COMP    #0
         JGT     LOOP
         STX     LENGTH
         RSUB
resub jfjf
.********NEGATIVE********
NEG      COMP    #0
         JLT     OUTPUT
         RSUB
OUTPUT   TD      DEV
         JEQ     OUTPUT
         MUL     ONE
         STA     NUM
         LDCH    #45
         WD      DEV
         RSUB
.*********DATA*************
NUM      byte    x'800001'
ONE      WORD    -1
         Word    1,2,3
DEV      BYTE    X'045'
DEV      BYTE    X'GFG'
DEV      BYTE    X'1FAB'
LENGTH   RESW    1
         END
