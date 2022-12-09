.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $2, %eax
    movl %eax, -8(%rbp)
    movl $1, %eax
    movl %eax, -12(%rbp)
    movl -8(%rbp), %eax
    movl -12(%rbp), %ebx
    cmpl %ebx, %eax
    setg %al
    cmpl $0, %eax
je .LC0
    movl $1, %eax
    movl %eax, -8(%rbp)
    jmp .LC1
.LC0:
    movl $0, %eax
    movl %eax, -8(%rbp)
.LC1:
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
