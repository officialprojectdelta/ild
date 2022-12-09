.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    cmpl $0, %eax
sete %al
    cmpl $0, %eax
je .LC0
    movl $3, %eax
    movl %eax, -8(%rbp)
    cmpl $0, %eax
setne %al
.LC0:
    cmpl $0, %eax
sete %al
    cmpl $0, %eax
je .LC1
    movl $4, %eax
    movl %eax, -8(%rbp)
    cmpl $0, %eax
setne %al
.LC1:
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
