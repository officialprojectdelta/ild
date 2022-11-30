.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    cmpl $0, %eax
je .LC0
    movl $2, %eax
    movl %eax, -12(%rbp)
    movl -12(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
    jmp .LC1
.LC0:
    movl $3, %eax
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    movl -16(%rbp), %edx
    cmpl %edx, %eax
    setl %al
    cmpl $0, %eax
je .LC2
    movl $4, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
    jmp .LC3
.LC2:
    movl $5, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
.LC3:
.LC1:
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
