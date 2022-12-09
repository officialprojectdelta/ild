.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $1, %eax
    movl %eax, -8(%rbp)
    movl $2, %eax
    movl %eax, -12(%rbp)
    movl $0, %eax
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    movl -12(%rbp), %ebx
    cmpl %ebx, %eax
    setg %al
    cmpl $0, %eax
je .LC0
    movl $5, %eax
    jmp .LC1
.LC0:
    movl -16(%rbp), %eax
    cmpl $0, %eax
je .LC2
    movl $6, %eax
    jmp .LC3
.LC2:
    movl $7, %eax
.LC3:
.LC1:
    movq %rbp, %rsp
    popq %rbp
    ret
