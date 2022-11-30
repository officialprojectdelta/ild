.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $0, %eax
    movl $0, %ebx
    cmpl %ebx, %eax
    setne %al
    movq %rbp, %rsp
    popq %rbp
    ret
