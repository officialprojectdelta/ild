.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $1, %eax
    movl %eax, -8(%rbp)
    movl $2, %eax
    movl %eax, -12(%rbp)
    movl -8(%rbp), %eax
    movl -12(%rbp), %ebx
    addl %ebx, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
