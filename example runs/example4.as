;comment line
;second comment line
         
mcro str
S1: .string "Hello"
endmcro

     D1:    .data 1,15,-698,175
     
     str
cmp    @r3,    @r5

.entry    D1
prn D1
bne S1

.extern S2,S3,S4

jsr S3
rts

    D2:   .data  1  ,2   ,  3
add D2, D1
sub 234, @r4
cmp             S3,S4
.entry S1
jmp LABEL

LABEL:  .string "a+-/24&6"
lea LABEL ,  @r6
stop
