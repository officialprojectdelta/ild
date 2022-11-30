.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $1, %eax
    movl $2, %ebx
    cmpl %ebx, %eax
    setg %al
    cmpl $0, %eax
je .LC0
    movl $3, %eax
    jmp .LC1
.LC0:
    movl $4, %eax
.LC1:
    movl %eax, -8(%rbp)
    movl $1, %eax
    movl $2, %ebx
    cmpl %ebx, %eax
    setg %al
    cmpl $0, %eax
je .LC2
    movl $5, %eax
    jmp .LC3
.LC2:
    movl $6, %eax
.LC3:
    movl %eax, -12(%rbp)
    movl -8(%rbp), %eax
    movl -12(%rbp), %ebx
    addl %ebx, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
