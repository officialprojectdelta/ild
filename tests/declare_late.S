.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $2, %eax
    movl $3, %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
