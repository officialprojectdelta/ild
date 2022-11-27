.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
