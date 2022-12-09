.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    cmpl $0, %eax
je .LC0
    movl $1, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
    jmp .LC1
.LC0:
    movl $2, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
.LC1:
    movq %rbp, %rsp
    popq %rbp
    ret
