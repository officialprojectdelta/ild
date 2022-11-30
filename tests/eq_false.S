.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $1, %eax
    movl $2, %ebx
    cmpl %ebx, %eax
    sete %al
    movq %rbp, %rsp
    popq %rbp
    ret
