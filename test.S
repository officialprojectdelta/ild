.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $4, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    movl $7, %ebx
    cmpl %ebx, %eax
    setl %al
    cmpl $0, %eax
    je .LC0
    movl $3, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
    jmp .LC1
.LC0:
    movl -8(%rbp), %eax
    movl $2, %ebx
    movq %rax, %r12
    movq %rdx, %r13
    movslq%eax, %rax
    cqo
    idivl %ebx
    movq %r13, %rdx
    movq %rbp, %rsp
    popq %rbp
    ret
.LC1:
    movq $0, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
