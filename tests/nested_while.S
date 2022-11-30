.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $1, %eax
    movl %eax, -8(%rbp)
.LC0:
    movl -8(%rbp), %eax
    movl $3, %ebx
    movq %rax, %rcx
    movslq%eax, %rax
    cqo
    idivl %ebx
    movl $20, %ebx
    cmpl %ebx, %eax
    setl %al
    cmpl $0, %eax
je .LC1
    movl $1, %eax
    movl %eax, -12(%rbp)
.LC2:
    movl -12(%rbp), %eax
    movl $10, %ebx
    cmpl %ebx, %eax
    setl %al
    cmpl $0, %eax
je .LC3
    movl -12(%rbp), %eax
    movl $2, %ebx
    imul %ebx, %eax
    movl %eax, -12(%rbp)
    jmp .LC2
.LC3:
    movl -8(%rbp), %eax
    movl -12(%rbp), %ebx
    addl %ebx, %eax
    movl %eax, -8(%rbp)
    jmp .LC0
.LC1:
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
