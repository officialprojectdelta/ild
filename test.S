.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $4, %eax
    movl $3, %ebx
    movq %rax, %rcx
    movslq %eax, %rax
    cqo
    idivl %ebx
    movq %rbp, %rsp
    popq %rbp
    ret
