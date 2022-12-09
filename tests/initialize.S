.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $2, %eax
    movl %eax, -8(%rbp)
    movl $0, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
