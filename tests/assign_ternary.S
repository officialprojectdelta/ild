.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl $1, %eax
    cmpl $0, %eax
je .LC0
    movl $2, %eax
    jmp .LC1
.LC0:
    movl $3, %eax
.LC1:
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
