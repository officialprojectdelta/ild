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
    movl $2, %eax
    movl %eax, -8(%rbp)
    jmp .LC1
.LC0:
    movl $3, %eax
    movl %eax, -8(%rbp)
.LC1:
    movl -12(%rbp), %eax
    cmpl $0, %eax
je .LC2
    movl $4, %eax
    movl %eax, -12(%rbp)
    jmp .LC3
.LC2:
    movl $5, %eax
    movl %eax, -12(%rbp)
.LC3:
    movl -8(%rbp), %eax
    movl -12(%rbp), %r10d
    addl %r10d, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
