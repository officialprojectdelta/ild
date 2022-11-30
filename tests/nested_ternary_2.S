.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $1, %eax
    cmpl $0, %eax
je .LC0
    movl $2, %eax
    cmpl $0, %eax
je .LC1
    movl $3, %eax
    jmp .LC2
.LC1:
    movl $4, %eax
.LC2:
    jmp .LC3
.LC0:
    movl $5, %eax
.LC3:
    movl %eax, -8(%rbp)
    movl $0, %eax
    cmpl $0, %eax
je .LC4
    movl $2, %eax
    cmpl $0, %eax
je .LC5
    movl $3, %eax
    jmp .LC6
.LC5:
    movl $4, %eax
.LC6:
    jmp .LC7
.LC4:
    movl $5, %eax
.LC7:
    movl %eax, -12(%rbp)
    movl -8(%rbp), %eax
    movl -12(%rbp), %esi
    imul %esi, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
