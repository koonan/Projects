.2345678901234567890123456789
         START   1000
QUES     JSUB    READ
         COMP    #48
         JLT     N
         COMP    #57
         JLT     Y
         JEQ     Y
         COMP    #65
         JLT     N
         COMP    #90
         JLT     Y
         JEQ     Y
         COMP    #97
         JLT     N
         COMP    #122
         JEQ     Y
         JLT     Y
................................
N        TD      OUTDEV
         JEQ     N
         LDA     NO
         WD      OUTDEV
         J       *
................................
Y        TD      OUTDEV
         JEQ     Y
         LDA     YES
         WD      OUTDEV
         J       *
..................................
READ     TD      INDEV
         JEQ     READ
         LDA     #0
         RD      INDEV
         RSUB
.............................
INDEV    BYTE    X'F3'
OUTDEV   BYTE    X'04'
YES      BYTE    X'000059'
NO       BYTE    X'00004E'
         END     QUES