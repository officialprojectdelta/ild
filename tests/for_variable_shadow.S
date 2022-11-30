.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -12(%rbp)
    movl $0, %eax
    movl %eax, -8(%rbp)
.LC0:
    movl -8(%rbp), %eax
    movl $10, %ebx
    cmpl %ebx, %eax
    setl %al
    cmpl $0, %eax
je .LC1
    movl -8(%rbp), %eax
    movl %eax, -16(%rbp)
    movl -16(%rbp), %eax
    movl %eax, -20(%rbp)
.LC3:
    movl -20(%rbp), %eax
    movl $10, %ebx
    cmpl %ebx, %eax
    setl %al
    cmpl $0, %eax
je .LC4
    movl -12(%rbp), %eax
    movl $1, %ebx
    addl %ebx, %eax
    movl %eax, -12(%rbp)
.LC5:
    movl -20(%rbp), %eax
    movl $1, %ebx
    addl %ebx, %eax
    movl %eax, -20(%rbp)
    jmp .LC3
.LC4:
.LC2:
    movl -8(%rbp), %eax
    movl $1, %ebx
    addl %ebx, %eax
    movl %eax, -8(%rbp)
    jmp .LC0
.LC1:
    movl -12(%rbp), %eax
    movl -8(%rbp), %ebx
    addl %ebx, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
