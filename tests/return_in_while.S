.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
.LC0:
    movl $1, %eax
    cmpl $0, %eax
je .LC1
    movl $2, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
    jmp .LC0
.LC1:
    movq %rbp, %rsp
    popq %rbp
    ret
