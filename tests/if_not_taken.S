.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -12(%rbp)
    movl -8(%rbp), %eax
    cmpl $0, %eax
je .LC0
    movl $1, %eax
    movl %eax, -12(%rbp)
.LC0:
    movl -12(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
