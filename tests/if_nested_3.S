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
    cmpl $0, %eax
je .LC1
    movl $3, %eax
    movl %eax, -8(%rbp)
    jmp .LC2
.LC1:
    movl $4, %eax
    movl %eax, -8(%rbp)
.LC2:
.LC0:
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
