.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $1, %eax
    movl $0, %ebx
    cmpl %ebx, %eax
    setg %al
    movq %rbp, %rsp
    popq %rbp
    ret
