.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -12(%rbp)
.LC0:
    movl -12(%rbp), %eax
    movl $10, %ebx
    cmpl %ebx, %eax
    setl %al
    cmpl $0, %eax
je .LC1
    movl -8(%rbp), %eax
    movl $2, %ebx
    movq %rax, %r11
    movq %rdx, %r12
    movslq%eax, %rax
    cqo
    idivl %ebx
    movq %r12, %rdx
    movl $2, %ebx
    imul %ebx, %eax
    movl -8(%rbp), %ebx
    cmpl %ebx, %eax
    setne %al
    cmpl $0, %eax
je .LC3
    jmp .LC2
.LC3:
    movl -8(%rbp), %eax
    movl -12(%rbp), %ebx
    addl %ebx, %eax
    movl %eax, -8(%rbp)
.LC2:
    movl -12(%rbp), %eax
    movl $1, %ebx
    addl %ebx, %eax
    movl %eax, -12(%rbp)
    jmp .LC0
.LC1:
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
