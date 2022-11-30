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
    movl $3, %ebx
    cmpl %ebx, %eax
    setl %al
    cmpl $0, %eax
je .LC1
    movl -8(%rbp), %eax
    movl $1, %ebx
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
