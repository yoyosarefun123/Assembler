mcro MATHOPS    
    mov @r8,  DATASEGMENT1
    add @r3, @v0
    sub @r0, !label
endmcro
.extern mov
mcro REGOPS    
clr 12.3
dec @rp
    jmp CHECK$EGMENT
    addiu STRING1
endmcro

.extern EXTERNSEG1, EXTERNSEG2 EXETEN3
.entry EXTERNSEG4 $$f

PROGRAMSTART: mov 2344, @r4
    red @r0 extra
    MATHOPS
    jmp mcro
    prn .data

.entry HELLO
HELLO: mov r3, 1
lea r1, r2
.entry HELLO
.extern HELLO 
.extern YES
.entry YES 
YES: mov r1, r2 
MATHOPS: mov r1, r2 

DATASEGMENT1:     .data 1, 2, 3 4, 5
DATASEGMENT2:     .data -1, -2, -3, -4, -5 !@
MORELABEL:      .data
STRINGSEG1:       .string "MegaAssembרהרlerTestSegOne"
STRINGSEG2:       .string "MegaAssemblerTestSegTwo
STRINGSEG3:       .string MegaAssemblerTestSegThree"
STRINGSEG4:       .string "MegaAssemblerTestSegFour"tt
STRINGSEG5:       .string fvre"MegaAssemblerTestSegFive"
MOREEEEE:       .string 
VALSEGMENT1:      .data 23, 23, 23 ,23 ,23                 ,3 ,
VALSEGMENT2:      .data d, 3
VALSEGMENT3:      .data 3+2
VALSEGMENT4:      .data -23223

.extern VALSEGMENT5!
.extern VALSEGMENT 1