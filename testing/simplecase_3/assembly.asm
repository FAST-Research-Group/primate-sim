addi x0, x0, 0
extract r0, r0, #8
addi x0, x0, 0
addi x0, x0, -15
insert x0, x0, #8


addi x0, x0, 0
slti x1, x0, 0 ; x1<- 1
addi x0, x0, 0
sltiu x2, x0, 0 ; x2<- 0
addi x0, x0, 0
andi x3, x0, -15
addi x0, x0, 0
andi x4, x0, 3
addi x0, x0, 0
ori x5, x1, -1
addi x0, x0, 0
xori x6, x0, -15 ; x6 <- 0
addi x0, x0, 0
xori x7, x0, 1 ; x7<- (-16)
addi x0, x0, 0
xori x8, x0, -1 ; x8<- 14
addi x0, x0, 0
slli x9, x0, 1 ; x9 <- (-30)
addi x0, x0, 0
addi x10, x12, 2
addi x0, x0, 0
srli x10, x10, 1 ; x10<-1
addi x0, x0, 0
srai x11, x9, 1 ; x11<- (-15)
addi x0, x0 0
lui x12 10 ; works up to here
addi x0 x0, 0
add x15 x0 x3
addi x0 x0 0
sub x16 x0 x1
addi x0 x0 0
slt x17 x0 x1
addi x0 x0 0
sltu x18 x0 x1
addi x0 x0 0
and x20 x16 x11
addi x0 x0 0
or x21 x16 x11
addi x0 x0 0
xor x22 x9 x3
addi x0 x0 0
sll x23 x3 x4
addi x0 x0 0
srl x24 x11 x4
addi x0 x0 0
sra x25 x0 x1
jal x0 -2
addi x0 x0 0



; the cfg file had 8 registers, I changed it to 32 since it was easier for me to test