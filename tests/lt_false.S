.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $2, %eax
    movl $1, %ebx
    cmpl %ebx, %eax
    setl %al
    movq %rbp, %rsp
    popq %rbp
    ret
