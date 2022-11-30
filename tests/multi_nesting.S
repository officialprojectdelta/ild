.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $2, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    movl $3, %ebx
    cmpl %ebx, %eax
    setl %al
    cmpl $0, %eax
je .LC0
    movl $3, %eax
    movl %eax, -12(%rbp)
    movl -12(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
.LC0:
    movq %rbp, %rsp
    popq %rbp
    ret
