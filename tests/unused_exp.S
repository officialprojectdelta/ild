.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $2, %eax
    movl $2, %ebx
    addl %ebx, %eax
    movl $0, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
