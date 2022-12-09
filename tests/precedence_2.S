.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $1, %eax
    cmpl $0, %eax
sete %al
    cmpl $0, %eax
je .LC0
    movl $0, %eax
    cmpl $0, %eax
setne %al
.LC0:
    cmpl $0, %eax
setne %al
    cmpl $0, %eax
je .LC1
    movl $0, %eax
    cmpl $0, %eax
setne %al
.LC1:
    movq %rbp, %rsp
    popq %rbp
    ret
