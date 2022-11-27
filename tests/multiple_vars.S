.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $1, %eax
    movl $2, %eax
    movl -8(%rbp), %eax
    movl -12(%rbp), %edi
    addl %edi, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
