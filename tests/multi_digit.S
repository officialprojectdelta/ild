.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $100, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
