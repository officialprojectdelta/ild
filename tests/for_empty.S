.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl %eax, -8(%rbp)
.LC0:
    movl -8(%rbp), %eax
    movl $1, %ebx
    addl %ebx, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    movl $3, %ebx
    cmpl %ebx, %eax
    setg %al
    cmpl $0, %eax
je .LC3
    jmp .LC1
.LC3:
.LC2:
    jmp .LC0
.LC1:
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
