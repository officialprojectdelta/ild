.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl $2, %ebx
    cmpl %ebx, %eax
    setle %al
    movq %rbp, %rsp
    popq %rbp
    ret
