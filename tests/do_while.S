.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $1, %eax
    movl %eax, -8(%rbp)
.LC0:
    movl -8(%rbp), %eax
    movl $2, %ebx
    imul %ebx, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    movl $11, %ebx
    cmpl %ebx, %eax
    setl %al
.LC1:
    cmpl $0, %eax
je .LC0
.LC2:
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
