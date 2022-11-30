.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $5, %eax
    cmpl $0, %eax
je .LC0
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
.LC0:
    movq %rbp, %rsp
    popq %rbp
    ret
