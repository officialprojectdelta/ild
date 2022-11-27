.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl $1, %eax
    movl -12(%rbp), %eax
    movl %eax, -8(%rbp)
    movl $2, %eax
    movl -8(%rbp), %eax
    movl -16(%rbp), %edx
    addl %edx, %eax
    movl %eax, -8(%rbp)
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
