.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $2, %eax
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
