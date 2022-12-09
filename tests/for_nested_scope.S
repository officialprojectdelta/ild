.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl %eax, -12(%rbp)
    movl $100, %eax
    movl %eax, -16(%rbp)
.LC0:
    movl -16(%rbp), %eax
    movl $0, %ebx
    cmpl %ebx, %eax
    setg %al
    cmpl $0, %eax
je .LC1
    movl $0, %eax
    movl %eax, -20(%rbp)
    movl -0(%rbp), %eax
    movl $2, %ebx
    imul %ebx, %eax
    movl -20(%rbp), %ebx
    addl %ebx, %eax
    movl %eax, -0(%rbp)
.LC2:
    movl -16(%rbp), %eax
    movl $1, %ebx
    subl %ebx, %eax
    movl %eax, -16(%rbp)
    jmp .LC0
.LC1:
    movl $3, %eax
    movl %eax, -28(%rbp)
    movl -12(%rbp), %eax
    movl -28(%rbp), %ebx
    addl %ebx, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
